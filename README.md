# Smart-Headset-Cursor-Controller-with-ESP8266-and-MPU6050

🧰 🔩 Components Used

1. ESP8266 NodeMCU (e.g., ESP-12E)
  Microcontroller with built-in Wi-Fi.
  Hosts a web server and sends accelerometer data to the PC.

2. MPU6050 (Accelerometer + Gyroscope Sensor)
   
  Used to detect motion and tilt in X and Y axes.
  Communicates with ESP8266 via I2C protocol.

3. Push Buttons (x3)
   
  Button 1: Increase cursor sensitivity.
  Button 2: Decrease cursor sensitivity.
  Button 3: Set current position as the initial (center) position.

✅ 🔌 Connection Diagram

Component	                     ESP8266 Pin	                    Notes

MPU6050 VCC	                    3.3V	                          Power supply (3.3V only!)
MPU6050 GND	                    GND	                            Common ground
MPU6050 SDA	                    D2 (GPIO4)	                    I²C Data
MPU6050 SCL	                    D1 (GPIO5)	                    I²C Clock
Button + (RESET CENTER)	        D7 (GPIO13)                    	Connect button between D7 and GND
Button + (Increase Sensitivity)	D5 (GPIO14)                     Connect button between D5 and GND
Button + (Decrease Sensitivity)	D6 (GPIO12)	                    Connect button between D6 and GND
  
