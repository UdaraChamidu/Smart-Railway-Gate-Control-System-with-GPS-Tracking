
# 🚦 GPS-Based Railway Gate Control System with Real-Time GPS Tracking

## 📌 Overview  
The **Smart Railway Gate Control System** is a safety-focused embedded solution that automates railway gate operations using **GPS tracking** and **real-time mobile alerts**. Designed for scalability and cost-efficiency, this system enhances safety at railway crossings for both urban and rural environments.

---

## 🎯 Key Features

- ✅ **Automated Gate Control**  
  Real-time train tracking via GPS and servo-controlled gates.

- 📲 **Real-Time Mobile Alerts**  
  Notifies users of approaching trains using a mobile app.

- ✋ **Emergency Manual Override**  
  Allows manual control of the gate in special scenarios.

- 🔊 **Audio-Visual Alerts**  
  Buzzer and LED warnings for pedestrian and vehicle safety.

- 💸 **Low-Cost and Scalable Design**  
  Affordable for wide-scale deployment.

---

## 🛠️ Technologies & Components

### 🔌 Hardware  
| Component            | Description                                  |
|---------------------|----------------------------------------------|
| **ESP32**           | Main microcontroller for processing and control |
| **Neo-6M GPS Module**| Provides real-time train location             |
| **Servo Motors**     | Operates gate barriers                        |
| **Buzzer & LED**     | Alerts for incoming trains                    |

### 💻 Software & Tools  
| Tool                | Purpose                                      |
|---------------------|----------------------------------------------|
| **C (ESP32)**        | Embedded programming                         |
| **Kotlin**           | Android mobile app development               |
| **Firebase**         | Real-time backend communication              |
| **MQTT Protocol**    | Lightweight communication for IoT devices   |
| **APIs**             | GPS data parsing and handling                |

---

## 🏗️ System Architecture

1. **Train Detection** – GPS module continuously tracks location.  
2. **Data Processing** – ESP32 evaluates GPS data and decides gate action.  
3. **Gate Operation** – Gate opens/closes via servo motors.  
4. **Real-Time Alerts** – Mobile app receives updates from Firebase/MQTT.  
5. **Manual Override** – Emergency control via the app.

---

## 🔁 Workflow Diagram

```
[ Train Approaching ]
          ↓
[ GPS Module Detects ]
          ↓
[ ESP32 Processes Data ]
          ↓
[ Alert Sent to App ]
          ↓
[ Gate Closes or Opens ]
```

---

## 📱 Mobile Application (Kotlin + Firebase)

<p align="center">
  <img src="https://github.com/user-attachments/assets/4009b631-74f2-4b07-ab06-ad41bc95deaa" width="200" />
  <img src="https://github.com/user-attachments/assets/389abb18-ecd8-462c-ad5a-d538e082ab3a" width="200" />
  <img src="https://github.com/user-attachments/assets/0b6d6800-0d0b-4cae-98e4-29e78a267bcc" width="200" />
</p>

---

## 🔧 TinkerCAD Hardware Simulation

<p align="center">
  <img src="https://github.com/user-attachments/assets/9d5c4c34-8ea7-4c79-a931-f1b1efafdccf" width="300" />
</p>

---

## 👨‍💻 Team Members  

| Name                   | Registration Number |
|------------------------|---------------------|
| G.A.S. De Silva        | 2021/E/037          |
| H.M.U. Cherath         | 2021/E/049          |
| H.K.H.T.K. Hemal       | 2021/E/065          |
| P.G.I.D. Madhushani    | 2021/E/107          |

---

## 🚀 How to Run the Project

### 🔁 Clone the Repository

```bash
git clone https://github.com/UdaraChamidu/Smart-Railway-Gate-Control-System-with-GPS-Tracking.git
```

---

### 🧠 Microcontroller Setup (ESP32)

1. Open `main.ino` in **ESP-IDF** or Arduino IDE.
2. Install required libraries:
   - `WiFi.h`
   - `Servo.h`
   - Other relevant dependencies.
3. Connect ESP32 to your PC via USB.
4. Compile and upload the code to ESP32.

---

### 📲 Mobile App Setup

1. Navigate to the `SmartRail-main` folder.
2. Open the project in **Android Studio**.
3. Run the app on an emulator or Android device.

---

## 📩 License

This project is developed for academic and research purposes. Feel free to reuse or extend it with proper credits to the authors.
