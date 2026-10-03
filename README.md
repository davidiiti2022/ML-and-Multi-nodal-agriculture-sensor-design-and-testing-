# 🌱 ML and Multi-Nodal Agriculture Sensor Design and Testing

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32-blue?style=for-the-badge&logo=espressif" />
  <img src="https://img.shields.io/badge/Communication-LoRa-orange?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Protocol-Modbus%20RTU-green?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Cloud-Firebase-yellow?style=for-the-badge&logo=firebase" />
  <img src="https://img.shields.io/badge/Language-C%2FC%2B%2B-blue?style=for-the-badge&logo=cplusplus" />
  <img src="https://img.shields.io/badge/ML-Random%20Forest-red?style=for-the-badge" />
</p>

<p align="center">
  <b>ESP32-Based Multi-Nodal Agriculture Monitoring and Sensor Validation System</b>
</p>

---

## 📌 Project Overview

The **ML and Multi-Nodal Agriculture Sensor Design and Testing** project is an IoT-based agricultural monitoring system designed to acquire, transmit, store, and analyze multiple soil and environmental parameters.

The system uses an **ESP32-based architecture** for sensor data acquisition and processing. A multi-parameter agricultural sensor communicates with the ESP32 through **RS485 using the Modbus RTU protocol**.

The acquired sensor data is transmitted over a **433 MHz LoRa wireless link** to a receiver node. The received data is displayed locally on an OLED display and stored in **Firebase/Firestore** for further processing and dataset generation.

A **Random Forest Machine Learning model is planned** for validating sensor readings and identifying potentially invalid or abnormal measurements.

---

## 🎯 Objectives

- Develop an ESP32-based agricultural sensing system.
- Acquire multiple soil and environmental parameters.
- Communicate with the agricultural sensor using RS485/Modbus RTU.
- Transmit sensor data wirelessly using LoRa.
- Develop a receiver node for reliable data reception.
- Display received data on an OLED screen.
- Store sensor readings in Firebase/Firestore.
- Automatically generate an Excel dataset using Python.
- Prepare a dataset for Machine Learning.
- Integrate a Random Forest model for sensor-data validation.

---

## 🌾 Parameters Monitored

| Parameter | Description | Unit |
|---|---|---|
| Soil Moisture | Soil water content | % / Sensor Unit |
| Temperature | Soil/environmental temperature | °C |
| Humidity | Environmental humidity | % |
| Nitrogen (N) | Nitrogen concentration | mg/kg |
| Phosphorus (P) | Phosphorus concentration | mg/kg |
| Potassium (K) | Potassium concentration | mg/kg |
| Electrical Conductivity (EC) | Soil conductivity | Sensor Unit |
| pH | Soil acidity/alkalinity | pH |

---

## 🏗️ System Architecture

<pre>
Agricultural Sensor
        |
        | RS485 / Modbus RTU
        v
+------------------------+
|        ESP32           |
|     Sensor Node        |
+-----------+------------+
            |
            | Data Processing
            v
+------------------------+
|   ML Validation Layer  |
|    Random Forest       |
|       (Planned)        |
+-----------+------------+
            |
       Valid Data
            |
            v
+------------------------+
|      LoRa 433 MHz      |
|      Transmitter       |
+-----------+------------+
            |
       Wireless Link
            |
            v
+------------------------+
|      LoRa 433 MHz      |
|       Receiver         |
+-----------+------------+
            |
            v
+------------------------+
|        ESP32           |
|    Receiver Node       |
+-----------+------------+
            |
       +----+----+
       |         |
       v         v
+----------+  +----------------+
|   OLED   |  | Firebase /     |
| Display  |  | Firestore      |
+----------+  +-------+--------+
                       |
                       v
                +-------------+
                | Python Data |
                |    Logger   |
                +------+------+
                       |
                       v
                +-------------+
                | Excel       |
                | Dataset     |
                +------+------+
                       |
                       v
                +-------------+
                | Machine     |
                | Learning    |
                +-------------+
</pre>

---

## 🔄 Data Flow

<pre>
Agricultural Sensor
        |
        v
RS485 / Modbus RTU
        |
        v
ESP32 Sensor Node
        |
        v
Data Processing
        |
        v
Random Forest Validation
        |
        +------ Invalid Data ------> Discard
        |
        v
Valid Sensor Data
        |
        v
LoRa 433 MHz
        |
        v
ESP32 Receiver
        |
        +------> OLED Display
        |
        v
Firebase / Firestore
        |
        v
Python
        |
        v
Excel Dataset
        |
        v
Machine Learning Analysis
</pre>

---

## 🔧 Hardware Components

- ESP32 Development Board
- Multi-parameter Agricultural Sensor
- RS485 Interface
- LoRa 433 MHz Module
- OLED Display
- Custom/Pre-designed PCB
- Power Supply
- Supporting electronic components

> The PCB has already been designed, so the ESP32 GPIO and communication configuration are maintained according to the existing hardware design.

---

## 💻 Software and Technologies

### Embedded Systems

- ESP32
- Arduino IDE
- C/C++
- UART
- SPI
- RS485
- Modbus RTU
- LoRa

### Cloud and Data Processing

- Firebase
- Firestore
- Python
- OpenPyXL
- Microsoft Excel

### Machine Learning

- Python
- Dataset preprocessing
- Random Forest
- Sensor-data validation

---

## 📡 Communication

### RS485 and Modbus RTU

The agricultural sensor communicates with the ESP32 through **RS485** using the **Modbus RTU protocol**.

### LoRa Communication

The processed sensor data is transmitted using a **433 MHz LoRa wireless communication link**.

---

## 🖥️ OLED Display

The receiver ESP32 displays the received agricultural sensor parameters on an OLED screen.

The monitored parameters include:

- Temperature
- Humidity
- Soil Moisture
- Nitrogen
- Phosphorus
- Potassium
- Electrical Conductivity
- pH

---

## ☁️ Firebase / Firestore Integration

The received sensor data is stored in **Firebase/Firestore**.

The cloud database provides centralized storage for continuously collected agricultural sensor readings.

---

## 🐍 Python Data Logging

A Python-based data logger retrieves sensor readings from Firestore and stores them in an Excel file.

The data pipeline is:

**Firebase/Firestore → Python → OpenPyXL → Excel Dataset**

---

## 📊 Dataset

The collected sensor readings are organized into a structured dataset for analysis and Machine Learning.

Typical dataset parameters include:

| Parameter |
|---|
| Timestamp |
| Temperature |
| Humidity |
| Soil Moisture |
| Nitrogen |
| Phosphorus |
| Potassium |
| Electrical Conductivity |
| pH |

---

## 🤖 Machine Learning Integration

A **Random Forest model is planned** for sensor-data validation.

The objective is to identify potentially invalid or abnormal sensor readings before they are transmitted through the LoRa network.

### Planned ML Pipeline

<pre>
Sensor Reading
      |
      v
Feature Extraction
      |
      v
Random Forest Model
      |
      v
Prediction
      |
   +--+--+
   |     |
 Valid  Invalid
   |     |
   v     v
 LoRa  Discard
   |
   v
Receiver
</pre>

> **Current Status:** Machine Learning integration is planned/in development. The current implementation focuses on sensor acquisition, communication, cloud storage, and dataset preparation.

---

# 🧪 Testing

The system can be tested at different stages.

## Sensor Testing

Verify that the agricultural sensor provides valid readings through RS485/Modbus.

## Communication Testing

Verify:

- RS485 communication
- Modbus data reading
- LoRa transmission
- LoRa reception

## Display Testing

Verify that the received parameters are correctly displayed on the OLED.

## Cloud Testing

Verify that new readings are correctly stored in Firebase/Firestore.

## Dataset Testing

Verify that the Python script continuously updates the Excel dataset.

## ML Testing

After Machine Learning model integration:

- Train the Random Forest model.
- Test validation accuracy.
- Evaluate abnormal-data detection.
- Test the model on new sensor readings.

---

# 📌 Current Project Status

| Module | Status |
|---|---|
| Agricultural Sensor | ✅ Implemented |
| ESP32 Sensor Node | ✅ Implemented |
| RS485 Communication | ✅ Implemented |
| Modbus RTU | ✅ Implemented |
| LoRa Communication | ✅ Implemented |
| ESP32 Receiver | ✅ Implemented |
| OLED Display | ✅ Implemented |
| Firebase/Firestore | ✅ Implemented |
| Python Data Logger | ✅ Implemented |
| Excel Dataset Generation | ✅ Implemented |
| Random Forest Integration | 🔄 Planned |
| ML Deployment on ESP32 | 🔄 Future Work |

---

# 🔮 Future Work

- Integrate the trained Random Forest model with ESP32.
- Perform real-time sensor-data validation.
- Detect abnormal sensor readings.
- Optimize the ML model for embedded deployment.
- Increase the agricultural dataset size.
- Evaluate model accuracy and reliability.
- Perform extensive LoRa range testing.
- Improve multi-node synchronization.
- Develop a complete real-time agricultural monitoring dashboard.

---

# 🌍 Applications

- Smart Agriculture
- Precision Agriculture
- Soil Monitoring
- Agricultural Sensor Testing
- Remote Farm Monitoring
- IoT-Based Farming
- Environmental Monitoring
- Agricultural Data Collection
- ML-Based Sensor Validation

---

# 📚 Skills Demonstrated

- Embedded Systems
- ESP32
- IoT
- Sensor Interfacing
- RS485
- Modbus RTU
- LoRa Communication
- UART/SPI
- OLED Display
- Firebase
- Firestore
- Python
- Excel Automation
- Dataset Preparation
- Machine Learning
- Random Forest
- Hardware-Software Integration

---

# 📁 Project Structure

<pre>
ML-and-Multi-nodal-agriculture-sensor-design-and-testing/
|
+-- ESP32/
|   +-- Transmitter/
|   |   +-- transmitter.ino
|   |
|   +-- Receiver/
|       +-- receiver.ino
|
+-- Python/
|   +-- Firebase_to_Excel/
|       +-- firebase_to_excel.py
|
+-- Dataset/
|   +-- Agricultural_Sensor_Data.xlsx
|
+-- Documentation/
|
+-- Images/
|
+-- README.md
</pre>

---

# ⚙️ Installation and Setup

## Arduino IDE

Install the Arduino IDE and add ESP32 board support.

Select the appropriate ESP32 development board before uploading the firmware.

## Required Arduino Libraries

Typical libraries include:

- LoRa
- ArduinoJson
- Wire
- SPI
- Adafruit GFX
- Adafruit SSD1306

The exact libraries depend on the hardware configuration used in the project.

---

# 🐍 Python Setup

Install the required Python packages:

    pip install firebase-admin
    pip install openpyxl

Run the Python data logger:

    python firebase_to_excel.py

---

# ▶️ How to Run

## Step 1 — Sensor Node

Upload the transmitter code to the ESP32 sensor node.

The transmitter:

1. Initializes the agricultural sensor.
2. Communicates with the sensor through RS485/Modbus.
3. Reads the sensor parameters.
4. Processes the readings.
5. Transmits the data through LoRa.

## Step 2 — Receiver Node

Upload the receiver code to the second ESP32.

The receiver:

1. Initializes the LoRa module.
2. Receives the sensor packet.
3. Extracts the sensor parameters.
4. Displays the readings on the OLED.
5. Stores the data through the Firebase/Firestore pipeline.

## Step 3 — Firebase

Configure the required Firebase/Firestore credentials for the data-storage system.

## Step 4 — Python Data Logger

Run:

    python firebase_to_excel.py

The script retrieves stored sensor readings and updates the Excel dataset.

---

# 📈 Example Sensor Output

    Temperature : 28.5 °C
    Humidity    : 62 %
    Moisture    : 45 %
    Nitrogen    : 70 mg/kg
    Phosphorus  : 98 mg/kg
    Potassium   : 197 mg/kg
    EC          : 197
    pH          : 7.0

---

# 👨‍💻 Author

## David Kumar

**B.Tech — Electrical Engineering**  
**Indian Institute of Technology Indore**

---

# ⭐ Acknowledgement

This project was developed as an academic/research project focused on **agricultural sensor monitoring, IoT communication, data collection, and Machine Learning-based sensor validation**.

---

# 🔑 Keywords

ESP32, Agriculture IoT, Smart Agriculture, Precision Agriculture, LoRa, LoRa 433 MHz, RS485, Modbus RTU, Firebase, Firestore, Python, OpenPyXL, Excel, Machine Learning, Random Forest, Soil Monitoring, Agricultural Sensors, Embedded Systems, IoT, Sensor Validation

---

# 📜 License

This project is intended for educational and research purposes.
