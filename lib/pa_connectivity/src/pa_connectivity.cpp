#include <Arduino.h>
#include <SPI.h>
#include <WiFiS3.h>
#include <PubSubClient.h>

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


void mqtt_reconnect(PubSubClient client, const char* user, const char* user_password) {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient", user, user_password)) {
      Serial.println("connected.");
      client.subscribe("channel");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void wifi_reconnect(const char* wifi_ssid, const char* wifi_password) {
  Serial.print("Attempting Wifi connection...");
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
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