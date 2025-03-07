# 🚦 GPS-Based Railway Gate Control System

## 📌 Project Overview
The **GPS-Based Railway Gate Control System** is an embedded system project designed to automate railway gate operations using GPS-based train tracking. The system provides real-time user alerts via a mobile application, ensuring enhanced safety and efficiency at railway crossings.

## 🎯 Features
- **Automated Gate Control**: Uses GPS data to detect train arrival and control gate operation via servo motors.
- **Real-Time Alerts**: Sends notifications to users through a mobile app.
- **User-Controlled Operation**: Allows manual gate control for emergency situations.
- **Audio-Visual Alerts**: Buzzer and LED signals warn pedestrians and vehicles in advance.
- **Low-Cost Implementation**: Designed to be affordable and scalable for both urban and rural areas.

## 🛠️ Technologies & Components
### 🖥️ Hardware Components
- **ESP32 Microcontroller**: Handles GPS data processing and controls gate operation.
- **Neo M6 GPS Module**: Tracks train location in real time.
- **Servo Motors**: Operate railway gates based on GPS signals.
- **Buzzer & LED**: Provide audio-visual alerts.

### 💻 Software & Development Tools
- **Arduino C/C++**: Programming the ESP32 microcontroller.
- **Flutter & React**: Developing the mobile application.
- **MQTT Protocol**: Enables real-time communication.
- **APIs**: Used for GPS data parsing and processing.

## 🏗️ System Architecture
1. **Train Detection**: GPS module continuously tracks train movement.
2. **Gate Operation**: ESP32 processes GPS data and triggers gate control.
3. **User Alerts**: Mobile app receives real-time updates.
4. **Manual Control**: Users can override the automatic gate operation if necessary.

## 📸 System Workflow
```
[ Train Approaching ] → [ GPS Module Detects ] → [ ESP32 Processes ] → [ Gate Closes ] → [ Alert Sent to App ]
```

## 📂 Code Implementation
### 🚦 ESP32 Code (main.ino)
```cpp
#include <WiFi.h>
#include <Servo.h>

#define SERVO_PIN 5
#define BUZZER_PIN 4
#define LED_PIN 2

Servo gateServo;

void setup() {
    Serial.begin(115200);
    gateServo.attach(SERVO_PIN);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    int trainDetected = detectTrain();
    if (trainDetected) {
        closeGate();
        alertUsers();
    } else {
        openGate();
    }
    delay(5000);
}

int detectTrain() {
    // Simulate train detection with GPS data
    return random(0, 2); // 0 = No Train, 1 = Train Approaching
}

void closeGate() {
    gateServo.write(90);
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
}

void openGate() {
    gateServo.write(0);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
}

void alertUsers() {
    Serial.println("🚨 Train Approaching! Gate Closing.");
}
```

### 📱 Mobile App Code (app.js)
```javascript
import React, { useState, useEffect } from 'react';
import { View, Text, Button } from 'react-native';
import mqtt from 'mqtt';

const App = () => {
    const [trainStatus, setTrainStatus] = useState('No Train Detected');
    
    useEffect(() => {
        const client = mqtt.connect('mqtt://broker.hivemq.com');
        client.on('connect', () => {
            client.subscribe('train/alert');
        });
        client.on('message', (topic, message) => {
            setTrainStatus(message.toString());
        });
        return () => client.end();
    }, []);

    return (
        <View>
            <Text>Train Status: {trainStatus}</Text>
            <Button title="Override Gate" onPress={() => alert('Manual Override Activated!')} />
        </View>
    );
};

export default App;
```

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
   git clone https://github.com/UdaraChamidu/Smart-Railway-Gate-Control-System.git
   ```
2. **Upload the Code to ESP32**:
   - Open `main.ino` in Arduino IDE.
   - Install required libraries (`WiFi.h`, `Servo.h`, etc.).
   - Compile and upload to ESP32.
3. **Run the Mobile App**:
   - Navigate to the `Mobile-App` folder.
   - Install dependencies using `npm install`.
   - Run the app with `npm start`.

## 📜 License
This project is licensed under the **MIT License**.

## 📝 Contact
For queries or contributions, reach out to:
- **Udara Chamidu** - [GitHub Profile](https://github.com/UdaraChamidu)
