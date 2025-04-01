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

#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <ESP32Servo.h>
#include <math.h>

// Define GPS module serial connection
HardwareSerial gpsSerial(1); // Using UART1 on ESP32 (TX=16, RX=17)
TinyGPSPlus gps;
Servo myServo1, myServo2;  // Two servos

// Default manual target locations
float manualLat1 = 9.332632;  // Target Latitude for first servo
float manualLon1 = 80.414093; // Target Longitude for first servo

float manualLat2 = 9.332632;  // Target Latitude for second servo
float manualLon2 = 80.414093; // Target Longitude for second servo

// Servo setup
const int servoPin1 = 18;  // First servo connected to pin 18
const int servoPin2 = 19;  // Second servo connected to pin 19
int currentAngle1 = 0; // Tracks first servo position
int currentAngle2 = 0; // Tracks second servo position

// Buzzer setup
const int buzzerPin = 21;  // Buzzer connected to pin 18 (same as first servo)

// Function to calculate distance (Haversine formula)
float haversineDistance(float lat1, float lon1, float lat2, float lon2) {
    const float R = 6371000; // Earth radius in meters
    float dLat = radians(lat2 - lat1);
    float dLon = radians(lon2 - lon1);
    float a = sin(dLat / 2) * sin(dLat / 2) +
              cos(radians(lat1)) * cos(radians(lat2)) *
              sin(dLon / 2) * sin(dLon / 2);
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c; // Distance in meters
}

// Function to rotate the servo in 5° steps
void rotateServo(Servo &servo, int &currentAngle, int targetAngle) {
    while (currentAngle != targetAngle) {
        if (currentAngle < targetAngle) currentAngle += 5;
        else currentAngle -= 5;

        servo.write(currentAngle);
        delay(100); // Smooth transition
    }
}

void setup() {
    Serial.begin(115200);
    gpsSerial.begin(9600, SERIAL_8N1, 16, 17); // GPS module
    myServo1.attach(servoPin1); // Attach first servo
    myServo2.attach(servoPin2); // Attach second servo
    myServo1.write(0); // Start first servo at 0°
    myServo2.write(0); // Start second servo at 0°

    pinMode(buzzerPin, OUTPUT);  // Set buzzer pin as output
    digitalWrite(buzzerPin, LOW); // Initially turn off the buzzer

    Serial.println("Waiting for GPS signal...");
}

void loop() {
    while (gpsSerial.available()) {
        gps.encode(gpsSerial.read());

        if (gps.location.isUpdated()) {
            float sensorLat = gps.location.lat();
            float sensorLon = gps.location.lng();

            Serial.print("GPS Latitude: ");
            Serial.println(sensorLat, 6);
            Serial.print("GPS Longitude: ");
            Serial.println(sensorLon, 6);

            // Calculate distance to first target location
            float distance1 = haversineDistance(manualLat1, manualLon1, sensorLat, sensorLon);
            Serial.print("Distance to target 1: ");
            Serial.print(distance1);
            Serial.println(" meters");

            // Calculate distance to second target location
            float distance2 = haversineDistance(manualLat2, manualLon2, sensorLat, sensorLon);
            Serial.print("Distance to target 2: ");
            Serial.print(distance2);
            Serial.println(" meters");

            // For first servo: Rotate based on distance to first target
            if (distance1 > 1000) {
                int targetAngle1 = 90;  // Rotate to 90° if distance > 1000 meters
                Serial.print("Rotating first servo to ");
                Serial.print(targetAngle1);
                Serial.println("°...");
                rotateServo(myServo1, currentAngle1, targetAngle1);

                // Turn off the buzzer if distance > 1000 meters
                digitalWrite(buzzerPin, LOW);  // Turn off buzzer
            } else {
                int targetAngle1 = -90;  // Rotate to -90° if distance <= 1000 meters
                Serial.print("Rotating first servo to ");
                Serial.print(targetAngle1);
                Serial.println("°...");
                rotateServo(myServo1, currentAngle1, targetAngle1);

                // Turn on the buzzer if distance < 1000 meters
                digitalWrite(buzzerPin, HIGH);  // Turn on buzzer
            }

            // For second servo: Rotate based on distance to second target
            if (distance2 > 1000) {
                int targetAngle2 = 90;  // Rotate to 90° if distance > 1000 meters
                Serial.print("Rotating second servo to ");
                Serial.print(targetAngle2);
                Serial.println("°...");
                rotateServo(myServo2, currentAngle2, targetAngle2);

                digitalWrite(buzzerPin, LOW);  // Turn off buzzer
            } else {
                int targetAngle2 = -90;  // Rotate to -90° if distance <= 1000 meters
                Serial.print("Rotating second servo to ");
                Serial.print(targetAngle2);
                Serial.println("°...");
                rotateServo(myServo2, currentAngle2, targetAngle2);

                 digitalWrite(buzzerPin, HIGH); 
            }

            Serial.println("-----------------------");

            delay(2000); // Avoid flooding the serial monitor
        }
    }
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
