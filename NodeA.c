#include <WiFi.h>
#include <PubSubClient.h>

// WIFI CREDENTIALS 

const char* ssid = "my qf";
const char* password = "Password25";

//  MQTT BROKER 
const char* mqtt_server = "192.168.0.12";     // <--- FIXED
const char* topic_level = "hamper/level";

WiFiClient espClient;
PubSubClient client(espClient);

//  ULTRASONIC PINS
#define TRIG 5
#define ECHO 18

long duration;
float distance;

void setup_wifi() {
  Serial.print("Connecting to WiFi...");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  int tries = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
    tries++;
   
    if (tries > 40) {
      Serial.println("\nFAILED TO CONECT");
      return;
    }
  }
  Serial.println("\nWiFi connected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("ESP32 Wifi RSSI: ");
  Serial.println(WiFi.RSSI());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("NodeA_Hamper", "admin", "Butface11")) {
      Serial.println("connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();

  client.setServer(mqtt_server, 1883);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

float readUltrasonic() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = duration * 0.034 / 2; // cm

  return distance;
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  float level = readUltrasonic();

  char msg[20];
  sprintf(msg, "%.1f", level);

  client.publish(topic_level, msg);

  Serial.print("Published hamper level: ");
  Serial.println(msg);

  delay(2000);
}


