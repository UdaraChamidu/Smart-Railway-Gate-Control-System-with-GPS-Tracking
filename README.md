# 🚦 GPS-Based Railway Gate Control System

## 📌 Project Overview
The **GPS-Based Railway Gate Control System** is an embedded system project designed to automate railway gate operations using GPS based train tracking. The system provides real time user alerts via a mobile application, ensuring enhanced safety and efficiency at railway crossings.

## 🎯 Features
- **Automated Gate Control**: Uses GPS data to detect train arrival and control gate operation via servo motors.
- **Real Time Alerts**: Sends notifications to users through a mobile app.
- **User Controlled Operation**: Allows manual gate control for emergency situations.
- **Audio Visual Alerts**: Buzzer and LED signals warn pedestrians and vehicles in advance.
- **Low Cost Implementation**: Designed to be affordable and scalable for both urban and rural areas.

## 🛠️ Technologies & Components
### 🖥️ Hardware Components
- **ESP32 Microcontroller**: Handles GPS data processing and controls gate operation.
- **Neo M6 GPS Module**: Tracks train location in real time.
- **Servo Motors**: Operate railway gates based on GPS signals.
- **Buzzer & LED**: Provide audio and visual alerts.

### 💻 Software & Development Tools
- **C**: Programming the ESP32 microcontroller.
- **Kotlin**: Developing the mobile application.
- **FireBase**: Enables real time communication.
- **APIs**: Used for GPS data parsing and processing.
- **MQTT Protocol**: Enables real time communication.

## 🏗️ System Architecture
1. **Train Detection**: GPS module continuously tracks train movement.
2. **Gate Operation**: ESP32 processes GPS data and triggers gate control.
3. **User Alerts**: Mobile app receives real time updates.
4. **Manual Control**: Users can override the automatic gate operation if necessary.

## 📸 System Workflow
```
[ Train Approaching ] → [ GPS Module Detects ] → [ ESP32 Processes ] → [ Alert Sent to App ] → [ Gate Closes or Opens] 
```

## 📂 Code Implementation
### 🚦 ESP32 Code (main.ino)
```cpp

````
###  📱 Mobile App

<p align="center">
  <img src="https://github.com/user-attachments/assets/4009b631-74f2-4b07-ab06-ad41bc95deaa" width="200" />
  <img src="https://github.com/user-attachments/assets/389abb18-ecd8-462c-ad5a-d538e082ab3a" width="200" />
  <img src="https://github.com/user-attachments/assets/0b6d6800-0d0b-4cae-98e4-29e78a267bcc" width="200" />
</p>

```

````
###  💡 TinkerCad Design

<p align="center">
  <img src="https://github.com/user-attachments/assets/9d5c4c34-8ea7-4c79-a931-f1b1efafdccf" width="300" />
</p>

````
````


## 👨‍💻 Group Members
| Name | Registration Number |
|---------------------------|----------------|
| G.A.S.DE SILVA | 2021/E/037 |
| H.M.U.C.HERATH | 2021/E/049 |
| HEMAL K.H.T.K. | 2021/E/065 |
| MADHUSHANI P.G.I.D. | 2021/E/107 |

## 📂 Repository Structure
```
📁 GPS-Based-Railway-Gate-Control-System
 ├── 📁 Code
 │   ├── 📄 main.ino (ESP32 Code)
 │   ├── 📄 config.h (Configuration Settings)
 ├── 📁 Mobile-App
 │   ├── 📄 app.js (Mobile App Code)
 │   ├── 📄 package.json (Dependencies)
 ├── 📁 Documentation
 │   ├── 📄 Project_Report.pdf
 ├── 📄 README.md (This File)
```

## 🚀 How to Use
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/UdaraChamidu/Smart-Railway-Gate-Control-System-with-GPS-Tracking.git
   ```
2. **Upload the Code to ESP32**:
   - Open `main.ino` in ESP IDF.
   - Install required libraries (`WiFi.h`, `Servo.h`, etc.).
   - Compile and upload to ESP32.
3. **Run the Mobile App**:
   - Navigate to the `SmartRail-main` folder.
   - Install dependencies using `npm install`.
   - Run the app with `npm start`.

## 📝 Contact
For queries or contributions, reach out to:
- **Udara Chamidu** - [GitHub Profile](https://github.com/UdaraChamidu)
