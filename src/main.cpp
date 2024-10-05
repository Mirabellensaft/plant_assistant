#include <Arduino.h>
#include <SPI.h>
#include <WiFiS3.h>
#include <PubSubClient.h>
#include <pa_connectivity.h>
#include <secrets.h>

// Variables:
// Pins:
int sensorPin = A0;
int led_green = D4;
int led_blue = D5;
int led_red = D6;

// Secrets:
const char* broker = MQTT_BROKER_IP;
const char* user = USER;
const char* user_password = USER_PASSWORD;
const char* wifi_ssid = WIFI_SSID;
const char* wifi_password = WIFI_PASSWORD;

// Values:
int sensorValue;
int sleep_duration = 5000;

// States:
typedef enum {
    SET_SLEEP, // disconnect everything 
    SLEEP, // everything disconnected
    AWAKE, // everything is connected
    ONLY_WIFI, // only wifi is connected, connect mqtt
    WAKE_UP //connect wifi and mqtt
  } state_t;

state_t state = SLEEP;

WiFiClient wifi;
PubSubClient client(wifi);

void setup() {
  // Setup serial communication, 9600 set as a default value
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_blue, OUTPUT);
  pinMode(led_red, OUTPUT);

  digitalWrite(led_green, HIGH);
  digitalWrite(led_blue, HIGH);
  digitalWrite(led_red, HIGH);

  // Setup wifi
  wifi_reconnect(wifi_ssid, wifi_password);

  // Setup MQTT
  client.setServer(broker, 1883);
  client.setCallback(callback);
};

void loop() {
  int statusCode;
  const char* response;
  char buffer[20];  // Puffer für den konvertierten String
  String payload = buffer;
  int sensorValue = 0;

  // State machine to gracefully disconnect and reconnect services
  switch (state) {
    case WAKE_UP:
      Serial.println("Waking up...");
      wifi_reconnect(wifi_ssid, wifi_password);
      if (!client.connected()) {
        mqtt_reconnect(client, user, user_password);
      };
      state = AWAKE;
      break;

    case ONLY_WIFI:
      if (!client.connected()) {
        mqtt_reconnect(client, user, user_password);
      };
      state = AWAKE;
      break;

    case SET_SLEEP:
      client.loop();
      Serial.println("Setting to sleep.");
      client.disconnect();
      wifi.stop();
      state = SLEEP;
      break;

    case AWAKE:
      Serial.println("Awake.");
      sensorValue = analogRead(sensorPin); 
      Serial.print("Analog Sensor Value: ");
      Serial.println(sensorValue); 

      sprintf(buffer, "{\"value\": %d}", sensorValue);

      client.publish("plants/plant_01", payload.c_str());
      delay(5000);
      client.loop();
      state = SET_SLEEP;
      
      break;

    case SLEEP:
      delay(sleep_duration);
      state = WAKE_UP;
      break;
  }
};
  