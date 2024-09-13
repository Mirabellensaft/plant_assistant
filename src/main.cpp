#include <Arduino.h>

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

}

void loop() {
  int sensorValue = analogRead(sensorPin); 
    Serial.print("Analog Sensor Value: ");
    Serial.println(sensorValue); 
    delay(1000);  
}

// put function definitions here:
