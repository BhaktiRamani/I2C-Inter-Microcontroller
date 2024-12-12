[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/9IMO547c)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=17298579&assignment_repo_type=AssignmentRepo)
# Mix & Match: I2C Inter-Microcontroller Communication Project

**Project Name:** Mix & Match
**Student:** [Your Name]
**Course:** Principles of Embedded Systems
**University:** University of Colorado Boulder
**Date:** Fall 2024

## Project Overview
Mix & Match is an advanced embedded systems project demonstrating inter-microcontroller communication using I2C protocol, temperature sensing, and display technologies.

## Hardware Components
- **Microcontrollers**:
  - STM32 Nucleo Board (Master)
  - Arduino Uno (Dual Role: Master and Slave)

- **Sensors**:
  - Si7021 Temperature and Humidity Sensor

- **Display**:
  - SSD1306 OLED Display

## Important Hardware Considerations
### Pull-Up Resistors
**CRITICAL:** 4 kΩ or higher pull-up resistors are REQUIRED for SDA and SCL lines to ensure proper I2C communication. These resistors:
- Prevent floating states
- Ensure signal integrity
- Provide a default logic high state
- Help manage signal rise times

## System Architecture
The project implements a complex communication ecosystem using I2C protocol where:
- STM32 Nucleo serves as the primary master controller
- Arduino Uno operates in dual mode (master for sensor, slave for STM32)
- Temperature and humidity data is collected, transmitted, and displayed dynamically


## Pin Configuration

### STM32 Nucleo Board Connections
- **OLED Display (I2C1)**
  - SDA: PB8
  - SCL: PB9

- **Arduino Communication**
  - TBD specific communication pins

### Arduino Uno Connections
- **Si7021 Sensor**
  - SDA: PA4
  - SCL: PA5

## Communication Flow Diagram
```
[STM32 Nucleo (Master)]
         │
         ▼
[Request Temp/Humidity Data]
         │
         ▼
[Arduino Uno (Slave Mode)]
         │
         ▼
[Si7021 Sensor Reads Data]
         │
         ▼
[Arduino Sends Data to STM32]
         │
         ▼
[STM32 Receives Data]
         │
         ▼
[STM32 Displays on OLED]
```

## Detailed Communication Process
1. STM32 initiates communication by requesting data
2. Arduino reads Si7021 sensor data
3. Arduino transmits data as I2C slave
4. STM32 receives data
5. STM32 displays data on OLED screen

## Software Requirements
- Arduino IDE
- STM32 Development Environment
- Required Libraries:
  - Adafruit_Si7021 (Arduino)
  - Wire Library (Arduino)
  - Custom Bare Metal I2C Implementation (STM32)

## Hardware Setup Instructions

### Arduino Setup
1. Connect Si7021 Sensor:
   - VCC to 3.3V
   - GND to GND
   - SDA to PA4
   - SCL to PA5

2. Prepare Arduino Code
   - Install Adafruit_Si7021 Library
   - Upload slave-mode sketch

### STM32 Nucleo Setup
1. Connect OLED Display:
   - VCC to 3.3V
   - GND to GND
   - SDA to PB8
   - SCL to PB9

2. Connect to Arduino
   - Refer to detailed pin connection diagram

## Debugging and Monitoring
- **Arduino**: Use Arduino Serial Monitor
- **STM32**: Use Putty for Serial Communication

## Monitoring Communication
- Observe real-time temperature and humidity data transmission
- Verify I2C communication integrity
- Check OLED display updates



### A4 Note (440 Hz)
![Alt text](photos/sine_wave_440.PNG)
- Fundamental frequency: 440 Hz
- Musical note: A4 (Concert A)
- Sample size: 109 samples per period





## Contributing
1. Fork the repository
2. Create a feature branch
3. Commit changes
4. Push to the branch
5. Create a Pull Request



## Contact
Bhakti Ramani - BhaktiRajeshKumarRamani@Colorado.edu



