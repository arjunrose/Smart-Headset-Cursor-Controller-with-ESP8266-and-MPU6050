#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <MPU6050.h>

MPU6050 mpu;
ESP8266WebServer server(80);

const char* ssid = "//your laptop hotspot name";
const char* password = "//password";

float accelX, accelY;
int sensitivity = 10;
const int minThreshold = 200, maxThreshold = 2000;

#define BTN_INC D5  // Sensitivity increase button
#define BTN_DEC D6  // Sensitivity decrease button
#define BTN_RESET D7 // Reset (set current position as center)

bool resetPressed = false;

void setup() {
    Serial.begin(115200);
    Wire.begin(D2, D1);
    
    pinMode(BTN_INC, INPUT_PULLUP);
    pinMode(BTN_DEC, INPUT_PULLUP);
    pinMode(BTN_RESET, INPUT_PULLUP);

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected!");

    mpu.initialize();
    
    server.on("/", HTTP_GET, []() {
        getMotionData();
        int buttonState = digitalRead(BTN_RESET) == LOW ? 1 : 0;  // Detect reset press
        String response = String(accelX) + "," + String(accelY) + "," + String(sensitivity) + "," + String(buttonState);
        server.send(200, "text/plain", response);
    });

    server.begin();
    Serial.println("Server started");
}

void loop() {
    server.handleClient();

    if (digitalRead(BTN_INC) == LOW) {
        sensitivity += 2;
        delay(200);
    }
    if (digitalRead(BTN_DEC) == LOW) {
        sensitivity = max(2, sensitivity - 2);
        delay(200);
    }
}

void getMotionData() {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    // Filtering small noise
    accelX = (abs(ax) > minThreshold) ? ax / 16384.0 : 0;
    accelY = (abs(ay) > minThreshold) ? ay / 16384.0 : 0;
    
    // Scaling based on sensitivity
    accelX *= sensitivity;
    accelY *= sensitivity;
}
