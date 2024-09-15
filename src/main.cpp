#include <Arduino.h>
#include <SPI.h>
#include <WiFiS3.h>
#include <ArduinoHttpClient.h> 
#include <Base64.h> 
#include "secrets.h"

// Variables
int sensorPin = A0;
int sensorValue;

// put function declarations here:
int send_to_grafana(int value, char* auth);
char* base64_encoding();


void setup() {
  // setup serial communication, 9600 set as a default value
  Serial.begin(9600);
  // sets the above defined sensor pin in input mode
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

      // RSSI (Signalstärke) abrufen
      long rssi = WiFi.RSSI();
      Serial.print("Signalstärke (RSSI): ");
      Serial.println(rssi);
    } else {
      Serial.print("error");
    }
}

void loop() {
  Serial.print("enter void");
  int sensorValue = analogRead(sensorPin); 
  Serial.print("Analog Sensor Value: ");
  Serial.println(sensorValue); 
  Serial.print("IP Adresse: ");
  Serial.println(WiFi.localIP());
  long rssi = WiFi.RSSI();
  Serial.print("Signalstärke (RSSI): ");
  Serial.println(rssi);
  delay(1000);  
}

// put function definitions here:

