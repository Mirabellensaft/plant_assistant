#include <Arduino.h>
#include <SPI.h>
#include <WiFiS3.h>
#include <PubSubClient.h>

int trials = 0;
// function declarations:
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void mqtt_reconnect(PubSubClient* client, const char* user, const char* user_password) {
  // Loop until we're reconnected
  while (!client->connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client->connect("arduinoClient", user, user_password)) {
      Serial.println("connected.");
      client->subscribe("channel");
      trials = 0;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client->state());

      if (trials < 6){
        
        Serial.println(" try again in 10 seconds");
        trials += 1;
        delay(10000);
      } else {
        Serial.println(" try again in 1 Minute");
        delay(60000);
        trials = 0;
      }
    }
  }
}

void wifi_reconnect(const char* wifi_ssid, const char* wifi_password) {
  Serial.print("Attempting Wifi connection...");
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
    
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("connected.");
    // Serial.print("IP Adresse: ");
    // Serial.println(WiFi.localIP());

    // RSSI (Signalstärke)
    long rssi = WiFi.RSSI();
    // Serial.print("Signalstärke (RSSI): ");
    // Serial.println(rssi);
  } else {
    Serial.print("error");
  } 
}