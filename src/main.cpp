#include <Arduino.h>
#include <SPI.h>
#include <WiFiS3.h>
#include "secrets.h"

// Variables
int sensorPin = A0;
int sensorValue;

// put function declarations here:
// 

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

    Serial.println("Connected to wifi.");

}

void loop() {
  int sensorValue = analogRead(sensorPin); 
  Serial.print("Analog Sensor Value: ");
  Serial.println(sensorValue); 
  delay(1000);  
}

// put function definitions here:
