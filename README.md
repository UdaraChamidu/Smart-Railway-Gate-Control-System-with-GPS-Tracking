# Smart-Railway-Gate-Control-System-with-GPS-Tracking

## Project Overview
The **Smart Railway Gate Control System with GPS Tracking and User Alerts** aims to improve railway crossing safety by using GPS tracking to monitor trains in real-time. The system provides a low-cost, user-controlled solution for railway gate operations. Through a mobile app, users receive real-time alerts and can control the gates, which enhances safety by preventing accidents at railway crossings.

This project uses an **ESP32 microcontroller** to integrate multiple components, including a **Neo M6 GPS module**, **servo motors**, and a **mobile app**. The app allows users to receive alerts and control the gate mechanism manually, providing a user-centric solution.

## Features
- **Real-Time Train Location**: GPS tracking for train monitoring.
- **Mobile App Control**: Users can open/close the gates, control audio-visual alerts (buzzer, LED), and monitor train locations.
- **Audio-Visual Alerts**: Buzzer and LED lights provide notifications about train arrivals.
- **User-Centric**: Gives complete control to the user via the mobile app, unlike traditional automated systems.

## System Architecture
The system consists of the following components:
- **ESP32**: The central controller for managing the system's logic and communication.
- **Neo M6 GPS Module**: Provides train location data to the ESP32.
- **Servo Motors**: Control the railway gate arms to open and close the gates.
- **Mobile App**: Sends real-time alerts and allows users to control the gates.
- **Buzzer & LED Light**: Provide audio and visual alerts to the user.

### Communication Protocols
- **UART**: Communication between the GPS module and the ESP32.
- **Wi-Fi**: For connecting the ESP32 to the mobile app.
- **PWM**: To control the servo motors for gate movement.
- **GPIO**: To control the buzzer and LED lights.

## Hardware Components
- **ESP32 Microcontroller**: Handles GPS data processing, gate control, and communication with the mobile app.
- **Neo M6 GPS Module**: Tracks the train's location for real-time updates.
- **Servo Motors**: Operate the gates by opening/closing them as required.
- **Buzzer and LED Light**: Provide alerts for train arrivals and gate status.

## Software Technologies
- **Programming Languages**: Arduino C/C++ for ESP32 programming.
- **Mobile App Development**: Built with **Flutter** for cross-platform compatibility (iOS and Android).
- **Communication Protocol**: MQTT for efficient data exchange between the app and the ESP32.
- **UI/UX Design**: User-friendly mobile app interface using **React**.

## Mobile App UI/UX
The mobile app features an intuitive and easy-to-use interface, including:
- **Dashboard**: Displays real-time train location, gate status, and alert controls.
- **Control Panel**: Allows the user to manually control the gates and audio-visual alerts.
- **Notification System**: Push notifications for train arrivals and gate status updates.
- **Accessibility Features**: Color-coded alerts and clear icons for easy navigation.

## Circuit Design
- The **Neo M6 GPS module** is connected to the **ESP32** via **UART**.
- **Servo motors** are connected to **PWM pins** on the ESP32 to control gate operation.
- **Buzzer and LED lights** are connected to **GPIO pins** for alert signaling.
- The mobile app communicates with the **ESP32** via **Wi-Fi**.

## Budget Breakdown
| Component                              | Quantity | Unit Cost (LKR) | Total Cost (USD) |
|----------------------------------------|----------|-----------------|------------------|
| ESP32 Microcontroller                  | 1        | 1500            | 1500             |
| Neo M6 GPS Module                      | 1        | 1100            | 1100             |
| Servo Motors                           | 2        | 350             | 700              |
| Buzzer                                 | 2        | 150             | 300              |
| Miscellaneous (wires, resistors, etc.) | -        | -               | 750              |
| Total Estimated Cost                   |          |                 | 4350             |

## Timeline
| Phase                       | Task                               | Milestone                    | Duration |
|-----------------------------|------------------------------------|------------------------------|----------|
| Week 1                      | Research and component procurement | Finalized system design      | 1 week   |
| Week 2-3                    | Circuit setup and GPS integration  | Working hardware prototype   | 2 weeks  |
| Week 4-6                    | Mobile app development             | Functional app interface     | 2 weeks  |
| Week 7                      | System testing and debugging       | Fully operational system     | 1 week   |
| Week 8                      | Final report and presentation      | Project completion           | 1 week   |

## Installation & Setup

 
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/UdaraChamidu/Smart-Railway-Gate-Control-System-with-GPS-Tracking.git
