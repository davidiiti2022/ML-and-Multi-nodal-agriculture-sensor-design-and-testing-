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

# 🎯 Objectives

The major objectives of this project are:

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

# 🌾 Parameters Monitored

The agricultural sensor provides multiple parameters related to soil and environmental conditions.

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

# 🏗️ System Architecture

```text
                    ┌──────────────────────────┐
                    │   Multi-Parameter        │
                    │   Agricultural Sensor    │
                    └────────────┬─────────────┘
                                 │
                                 │ RS485
                                 │ Modbus RTU
                                 ▼
                    ┌──────────────────────────┐
                    │          ESP32           │
                    │      Sensor Node         │
                    └────────────┬─────────────┘
                                 │
                                 │ Data Processing
                                 ▼
                    ┌──────────────────────────┐
                    │  ML Validation Layer     │
                    │  Random Forest           │
                    │      (Planned)           │
                    └────────────┬─────────────┘
                                 │
                           Valid Data
                                 │
                                 ▼
                    ┌──────────────────────────┐
                    │      LoRa 433 MHz        │
                    │      Transmitter         │
                    └────────────┬─────────────┘
                                 │
                                 │ Wireless Link
                                 ▼
                    ┌──────────────────────────┐
                    │      LoRa 433 MHz        │
                    │        Receiver          │
                    └────────────┬─────────────┘
                                 │
                                 ▼
                    ┌──────────────────────────┐
                    │          ESP32           │
                    │      Receiver Node       │
                    └────────────┬─────────────┘
                                 │
                       ┌─────────┴─────────┐
                       │                   │
                       ▼                   ▼
              ┌────────────────┐   ┌─────────────────┐
              │ OLED Display   │   │ Firebase /      │
              │                │   │ Firestore       │
              └────────────────┘   └────────┬────────┘
                                             │
                                             ▼
                                   ┌─────────────────┐
                                   │ Python Data     │
                                   │ Logger          │
                                   └────────┬────────┘
                                            │
                                            ▼
                                   ┌─────────────────┐
                                   │ Excel Dataset   │
                                   └────────┬────────┘
                                            │
                                            ▼
                                   ┌─────────────────┐
                                   │ Machine         │
                                   │ Learning        │
                                   └─────────────────┘
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

This system can be applied to:

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

This project demonstrates practical experience in:

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

# 👨‍💻 Author

## David Kumar

**B.Tech — Electrical Engineering**  
**Indian Institute of Technology Indore**

---

# ⭐ Acknowledgement

This project was developed as an academic/research project focused on **agricultural sensor monitoring, IoT communication, data collection, and Machine Learning-based sensor validation**.

---

# 🔑 Keywords

```text
ESP32
Agriculture IoT
Smart Agriculture
Precision Agriculture
LoRa
LoRa 433 MHz
RS485
Modbus RTU
Firebase
Firestore
Python
OpenPyXL
Excel
Machine Learning
Random Forest
Soil Monitoring
Agricultural Sensors
Embedded Systems
IoT
Sensor Validation
