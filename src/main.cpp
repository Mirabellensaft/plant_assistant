#include <Arduino.h>
#include <SPI.h>
#include <WiFiS3.h>
#include <PubSubClient.h>
#include <secrets.h>

// Variables
int sensorPin = A0;
int sensorValue;
const char* broker = MQTT_BROKER_IP;


// put function declarations here:

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

WiFiClient wifi;
PubSubClient client(wifi);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // setup serial communication, 9600 set as a default value
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);

  // setup wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("wifi connected");
      Serial.print("IP Adresse: ");
      Serial.println(WiFi.localIP());

      // RSSI (Signalstärke)
      long rssi = WiFi.RSSI();
      Serial.print("Signalstärke (RSSI): ");
      Serial.println(rssi);
    } else {
      Serial.print("error");
    } 

  client.setServer(broker, 1883);
  client.setCallback(callback);
};

void loop() {
  int statusCode;
  const char* response;
  {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  int sensorValue = analogRead(sensorPin); 
  Serial.print("Analog Sensor Value: ");
  Serial.println(sensorValue); 
  Serial.print("IP Adresse: ");
  Serial.println(WiFi.localIP());
  long rssi = WiFi.RSSI();
  Serial.print("Signalstärke (RSSI): ");
  Serial.println(rssi);
  char buffer[20];  // Puffer für den konvertierten String

  sprintf(buffer, "{\"value\": %d}", sensorValue);

  String payload = buffer;
  client.publish("plants/plant_01", payload.c_str());
  delay(5000);  // Daten alle 5 Sekunden senden
  };
};
  
  

 

