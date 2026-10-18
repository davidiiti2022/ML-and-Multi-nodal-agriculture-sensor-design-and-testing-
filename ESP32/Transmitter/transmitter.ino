
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <LoRa.h>
#include <SoftwareSerial.h>

// =====================================================
// HARDWARE PINS - UNCHANGED
// =====================================================

#define ss      5
#define rst     14
#define dio0    2

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// RS485 pins - UNCHANGED
#define RE       26
#define DE       27

#define RED_LED  32
#define GREEN_LED 35
#define BUZZ     34

// =====================================================
// RS485 MODBUS REQUESTS - UNCHANGED
// =====================================================

const byte temp[]  = {0x01, 0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xcf};
const byte mois[]  = {0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0F};
const byte econ[]  = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xce};
const byte ph[]    = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0b};

const byte nitro[] = {0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C};
const byte phos[]  = {0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC};
const byte pota[]  = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0};

// =====================================================
// RS485 SETTINGS
// =====================================================

// Same hardware pins: RX = 25, TX = 33
SoftwareSerial mod(25, 33);

// Maximum time to wait for a Modbus response
// 150 ms is safer than immediately reducing to a very small value.
#define MODBUS_TIMEOUT 150

// Invalid sensor return value
#define INVALID_VALUE -1

// =====================================================
// SENSOR VARIABLES
// =====================================================

// int16_t prevents 16-bit sensor values from being truncated.
int16_t val1 = 0;       // Nitrogen
int16_t val2 = 0;       // Phosphorous
int16_t val3 = 0;       // Potassium
int16_t val4 = 0;       // Moisture
int16_t val5 = 0;       // Conductivity
int16_t val6 = 0;       // pH
float   soil_temp = 0;  // Temperature

float soil_ph   = 0.0;
float soil_mois = 0.0;

// =====================================================
// AVERAGING VARIABLES
// =====================================================

float sum_n    = 0.0;
float sum_p    = 0.0;
float sum_k    = 0.0;
float sum_mois = 0.0;
float sum_con  = 0.0;
float sum_ph   = 0.0;
float sum_te   = 0.0;

float avg_n    = 0.0;
float avg_p    = 0.0;
float avg_k    = 0.0;
float avg_mois = 0.0;
float avg_con  = 0.0;
float avg_ph   = 0.0;
float avg_te   = 0.0;

// Two samples are averaged
uint8_t counter = 1;

// Connection / buzzer state
int c = 0;

// Sensor ID - unchanged
int Sensor_id = 123456;

// Packet counter
int counter1 = 0;

// =====================================================
// MODBUS RESPONSE BUFFER
// =====================================================

byte response[7];

// =====================================================
// MODBUS CRC16
// =====================================================

uint16_t modbusCRC(const byte *buffer, uint8_t length)
{
  uint16_t crc = 0xFFFF;

  for (uint8_t pos = 0; pos < length; pos++)
  {
    crc ^= (uint16_t)buffer[pos];

    for (uint8_t i = 8; i != 0; i--)
    {
      if (crc & 0x0001)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      {
        crc >>= 1;
      }
    }
  }

  return crc;
}

// =====================================================
// GENERIC MODBUS SENSOR READ
// =====================================================

int16_t readModbusRegister(const byte *request, uint8_t requestLength)
{
  // ---------------------------------------------------
  // 1. Remove old bytes from RX buffer
  // ---------------------------------------------------

  while (mod.available())
  {
    mod.read();
  }

  // ---------------------------------------------------
  // 2. RS485 TRANSMIT MODE
  // ---------------------------------------------------

  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);

  delay(1);

  // ---------------------------------------------------
  // 3. Send Modbus request
  // ---------------------------------------------------

  mod.write(request, requestLength);

  // Wait until complete request has physically left UART
  mod.flush();

  // ---------------------------------------------------
  // 4. RS485 RECEIVE MODE
  // ---------------------------------------------------

  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // ---------------------------------------------------
  // 5. Wait for exactly 7 response bytes
  //    Instead of blindly waiting 200 ms
  // ---------------------------------------------------

  uint8_t received = mod.readBytes(response, 7);

  // ---------------------------------------------------
  // 6. Check response length
  // ---------------------------------------------------

  if (received != 7)
  {
    Serial.println("RS485 timeout / incomplete response");
    return INVALID_VALUE;
  }

  // ---------------------------------------------------
  // 7. Check Modbus device address
  // ---------------------------------------------------

  if (response[0] != 0x01)
  {
    Serial.println("RS485 invalid device address");
    return INVALID_VALUE;
  }

  // ---------------------------------------------------
  // 8. Check function code
  // ---------------------------------------------------

  if (response[1] != 0x03)
  {
    Serial.println("RS485 invalid function code");
    return INVALID_VALUE;
  }

  // ---------------------------------------------------
  // 9. Check byte count
  // ---------------------------------------------------

  if (response[2] != 0x02)
  {
    Serial.println("RS485 invalid byte count");
    return INVALID_VALUE;
  }

  // ---------------------------------------------------
  // 10. Check CRC
  // ---------------------------------------------------

  uint16_t calculatedCRC = modbusCRC(response, 5);

  uint16_t receivedCRC =
      (uint16_t)response[5] |
      ((uint16_t)response[6] << 8);

  if (calculatedCRC != receivedCRC)
  {
    Serial.println("RS485 CRC error");
    return INVALID_VALUE;
  }

  // ---------------------------------------------------
  // 11. Combine HIGH + LOW byte
  // ---------------------------------------------------

  uint16_t sensorValue =
      ((uint16_t)response[3] << 8) |
      response[4];

  return (int16_t)sensorValue;
}

// =====================================================
// SENSOR FUNCTIONS
// =====================================================

int16_t nitrogen()
{
  return readModbusRegister(nitro, sizeof(nitro));
}

int16_t phosphorous()
{
  return readModbusRegister(phos, sizeof(phos));
}

int16_t potassium()
{
  return readModbusRegister(pota, sizeof(pota));
}

int16_t moisture()
{
  return readModbusRegister(mois, sizeof(mois));
}

int16_t econduc()
{
  return readModbusRegister(econ, sizeof(econ));
}

int16_t phydrogen()
{
  return readModbusRegister(ph, sizeof(ph));
}

int16_t temperature()
{
  return readModbusRegister(temp, sizeof(temp));
}

// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(115200);

  // RS485 baud rate - UNCHANGED
  mod.begin(115200);

  // Timeout used by readBytes()
  mod.setTimeout(MODBUS_TIMEOUT);

  Serial.println("LoRa Sender");

  // ---------------------------------------------------
  // LoRa - UNCHANGED
  // ---------------------------------------------------

  LoRa.setPins(ss, rst, dio0);

  while (!LoRa.begin(433E6))
  {
    Serial.println(".");
    delay(500);
  }

  LoRa.setSyncWord(0xF3);

  Serial.println("LoRa Initializing OK!");

  // ---------------------------------------------------
  // RS485 direction pins
  // ---------------------------------------------------

  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // ---------------------------------------------------
  // LED / Buzzer
  // ---------------------------------------------------

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  // ---------------------------------------------------
  // OLED
  // ---------------------------------------------------

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED initialization failed!");
    return;
  }

  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(20, 10);
  display.println("NPK Sensor");

  display.setCursor(20, 25);
  display.println("Initializing");

  display.display();

  delay(1000);

  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(30, 20);
  display.print("Data");

  display.setCursor(10, 40);
  display.print("Fetching");

  display.display();

  delay(500);
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop()
{
  // ===================================================
  // 1. READ ALL SENSORS
  // ===================================================

  val1 = nitrogen();

  val2 = phosphorous();

  val3 = potassium();

  val4 = moisture();

  if (val4 != INVALID_VALUE)
  {
    soil_mois = val4;
    val4 = soil_mois / 1.8;
  }

  val5 = econduc();

  val6 = phydrogen();

  if (val6 != INVALID_VALUE)
  {
    soil_ph = (float)val6 / 25.0;
    val6 = soil_ph;
  }

  int16_t tempRaw = temperature();

  if (tempRaw != INVALID_VALUE)
  {
    soil_temp = (float)tempRaw / 10.0;
  }
  else
  {
    soil_temp = INVALID_VALUE;
  }

  // ===================================================
  // 2. CHECK SENSOR VALIDITY
  // ===================================================

  bool sensorError =
      (val1 == INVALID_VALUE) ||
      (val2 == INVALID_VALUE) ||
      (val3 == INVALID_VALUE) ||
      (val4 == INVALID_VALUE) ||
      (val5 == INVALID_VALUE) ||
      (val6 == INVALID_VALUE) ||
      (tempRaw == INVALID_VALUE);

  if (sensorError)
  {
    Serial.println("Sensor / RS485 communication error!");

    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);

    c = 0;
    counter = 1;

    // Reset averaging
    resetAverages();

    displayDisconnected();

    // Don't continue with invalid data
    delay(100);
    return;
  }

  // ===================================================
  // 3. SENSOR IS WORKING
  // ===================================================

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  // ---------------------------------------------------
  // Beep only once after reconnection
  // ---------------------------------------------------

  if (c == 0)
  {
    tone(BUZZ, 5000);
    delay(250);
    noTone(BUZZ);

    c++;

    displayFetching();
  }

  // ===================================================
  // 4. SERIAL DEBUG
  // ===================================================

  Serial.print("N: ");
  Serial.println(val1);

  Serial.print("P: ");
  Serial.println(val2);

  Serial.print("K: ");
  Serial.println(val3);

  Serial.print("Moisture: ");
  Serial.println(val4);

  Serial.print("Conductivity: ");
  Serial.println(val5);

  Serial.print("pH: ");
  Serial.println(val6);

  Serial.print("Temperature: ");
  Serial.println(soil_temp);

  // ===================================================
  // 5. ACCUMULATE TWO READINGS
  // ===================================================

  if (counter <= 2)
  {
    sum_n    += val1;
    sum_p    += val2;
    sum_k    += val3;
    sum_mois += val4;
    sum_con  += val5;
    sum_ph   += val6;
    sum_te   += soil_temp;

    counter++;
  }

  // ===================================================
  // 6. AFTER TWO READINGS
  // ===================================================

  if (counter == 3)
  {
    // -------------------------------------------------
    // Calculate averages
    // -------------------------------------------------

    avg_n    = sum_n / 2.0;
    avg_p    = sum_p / 2.0;
    avg_k    = sum_k / 2.0;
    avg_mois = sum_mois / 2.0;
    avg_con  = sum_con / 2.0;
    avg_ph   = sum_ph / 2.0;
    avg_te   = sum_te / 2.0;

    // -------------------------------------------------
    // Send ONLY ONE LoRa packet
    // Receiver expects:
    //
    // N,P,K,Moisture,Conductivity,pH,Temperature,
    // -------------------------------------------------

    send_data_via_lora1(
      avg_n,
      avg_p,
      avg_k,
      avg_mois,
      avg_con,
      avg_ph,
      avg_te
    );

    Serial.print("Sending packet: ");
    Serial.println(counter1++);

    // -------------------------------------------------
    // Display result
    // No 2.5 second blocking delay
    // -------------------------------------------------

    display_oled1(
      avg_n,
      avg_p,
      avg_k,
      avg_mois,
      avg_con,
      avg_ph,
      avg_te
    );

    // -------------------------------------------------
    // IMPORTANT:
    // Reset averaging sums
    // -------------------------------------------------

    resetAverages();

    counter = 1;
  }

  // ===================================================
  // NO 1 SECOND DELAY HERE
  // ===================================================

  yield();
}

// =====================================================
// RESET AVERAGING
// =====================================================

void resetAverages()
{
  sum_n    = 0.0;
  sum_p    = 0.0;
  sum_k    = 0.0;
  sum_mois = 0.0;
  sum_con  = 0.0;
  sum_ph   = 0.0;
  sum_te   = 0.0;
}

// =====================================================
// OLED - DISCONNECTED
// =====================================================

void displayDisconnected()
{
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(25, 25);

  display.print("Disconnected");

  display.display();
}

// =====================================================
// OLED - FETCHING
// =====================================================

void displayFetching()
{
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(2);

  display.setCursor(30, 10);
  display.print("Data");

  display.setCursor(10, 35);
  display.print("Fetching");

  display.display();
}

// =====================================================
// OLED - SENSOR RESULTS
// =====================================================

void display_oled1(
    float v1,
    float v2,
    float v3,
    float moisture,
    float v5,
    float v6,
    float v7)
{
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(1);

  // N
  display.setCursor(0, 0);
  display.print("N:");
  display.print(v1, 0);
  display.print(" mg/kg");

  // P
  display.setCursor(0, 9);
  display.print("P:");
  display.print(v2, 0);
  display.print(" mg/kg");

  // K
  display.setCursor(0, 18);
  display.print("K:");
  display.print(v3, 0);
  display.print(" mg/kg");

  // Moisture
  display.setCursor(0, 27);
  display.print("SM:");
  display.print(moisture, 0);
  display.print("%");

  // Conductivity
  display.setCursor(0, 36);
  display.print("EC:");
  display.print(v5, 0);
  display.print(" uS/cm");

  // pH
  display.setCursor(0, 45);
  display.print("pH:");
  display.print(v6, 2);

  // Temperature
  display.setCursor(65, 45);
  display.print("T:");
  display.print(v7, 1);
  display.print("C");

  display.display();
}

// =====================================================
// LoRa TRANSMISSION
// =====================================================

void send_data_via_lora1(
    float v1,
    float v2,
    float v3,
    float v4,
    float v5,
    float v6,
    float v7)
{
  Serial.println("Sending data to LoRa...");

  LoRa.beginPacket();

  // IMPORTANT:
  // Packet format is unchanged:
  // N,P,K,Moisture,Conductivity,pH,Temperature,

  LoRa.print(v1, 0);
  LoRa.print(",");
  LoRa.print(v2, 0);
  LoRa.print(",");
  LoRa.print(v3, 0);
  LoRa.print(",");
  LoRa.print(v4, 0);
  LoRa.print(",");
  LoRa.print(v5, 0);
  LoRa.print(",");
  LoRa.print(v6, 2);
  LoRa.print(",");
  LoRa.print(v7, 1);
  LoRa.print(",");
  LoRa.endPacket();

  Serial.println("LoRa packet sent.");
}
