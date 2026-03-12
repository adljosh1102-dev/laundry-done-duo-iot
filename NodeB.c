#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "SparkFunLIS3DH.h"

LIS3DH myIMU;

// WIFI
const char* ssid = "my qf";
const char* password = "Password25";

// MQTT
const char* mqtt_server = "192.168.0.12";  
const int mqtt_port = 1883;
const char* mqtt_user = "admin";
const char* mqtt_pass = "Butface11";

// MQTT Topics
const char* topic_start = "Washer/Start";
const char* topic_finish = "Washer/Finish";

WiFiClient espClient;
PubSubClient client(espClient);

// Washer state detection
bool washerRunning = false;
unsigned long stillnessStart = 0;

void setup_wifi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("NodeB_Washer", mqtt_user, mqtt_pass)) {
      Serial.println("connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(1500);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);

  if (myIMU.begin() == false) {
    Serial.println("LIS3DH not detected!");
    while (1);
  }

  Serial.println("LIS3DH ready!");
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  float x = myIMU.readFloatAccelX();
  float y = myIMU.readFloatAccelY();
  float z = myIMU.readFloatAccelZ();

  float magnitude = sqrt(x*x + y*y + z*z);

  // Washer motion threshold
  if (magnitude > 1.20) {
    if (!washerRunning) {
      washerRunning = true;
      Serial.println("EVENT → Washer Started");
      client.publish(topic_start, "Washer Started");
    }
    stillnessStart = millis(); 
  }

  // Washer finish detection when still for 5 seconds
  if (washerRunning && millis() - stillnessStart > 5000) {
    washerRunning = false;
    Serial.println("EVENT → Washer Finished");
    client.publish(topic_finish, "Washer Finished");
  }

  delay(200);
}
