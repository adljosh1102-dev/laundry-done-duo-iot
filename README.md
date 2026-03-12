--------------------------------------------------Laundry Done Duo (IoT)--------------------------------------------------

Laundry Done Duo is a two-node IoT system designed to monitor a washer/dryer cycle and track laundry removal from a hamper.

The system combines vibration detection and distance sensing to determine when laundry is finished and when clothes are removed.

-------------------------------------------------- System Overview --------------------------------------------------

The system uses two ESP32 microcontroller nodes.

Node 1 monitors the laundry hamper using an ultrasonic sensor to measure how full the hamper is.

Node 2 monitors the washer or dryer using an accelerometer to detect vibration. This allows the system to determine when a laundry cycle starts and finishes.

Both nodes communicate wirelessly over WiFi.

-------------------------------------------------- Node Descriptions--------------------------------------------------

### Node 1 – Hamper Sensor Node
Uses an HC-SR04 ultrasonic sensor mounted at the top of the hamper to measure the distance to the clothing pile.
Distance decreases as clothes are added and increases when clothes are removed.

### Node 2 – Washer/Dryer Detection Node
Uses a 3-axis accelerometer attached to the washer or dryer.
When the machine is running, vibration is detected. When vibration stops for a period of time, the system assumes the laundry cycle has finished.

-------------------------------------------------- Hardware Used --------------------------------------------------

ESP32 Development Boards  
HC-SR04 Ultrasonic Sensor  
3-Axis Accelerometer (MPU6050 or ADXL345)  
Breadboard  
Jumper Wires  

--------------------------------------------------Software / Tools --------------------------------------------------

Arduino IDE  
ESP32 WiFi libraries  
C++ (Arduino framework)

-------------------------------------------------- System Flow -------------------------------------------------------

1. Washer or dryer begins running → accelerometer detects vibration.
2. Node 2 sends a "machine running" state.
3. When vibration stops for a period of time, Node 2 signals that the cycle is complete.
4. Node 1 monitors the hamper level using the ultrasonic sensor.
5. When clothes are removed from the hamper, the system confirms the laundry cycle has been handled.

--------------------------------------------------Future Improvements--------------------------------------------------

Mobile notifications  
Web dashboard  
Smart home integration  
Machine learning for cycle detection

-------------------------------------------------- Author --------------------------------------------------

Joshua Yero  
Computer Engineering – UNLV
