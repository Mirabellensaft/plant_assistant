#include <Arduino.h>
#include <SPI.h>
#include <WiFiS3.h>
#include <PubSubClient.h>
#include <secrets.h>

// Variables:
int sensorPin = A0;
int led_green = D4;
int led_blue = D5;
int led_red = D6;

int sensorValue;
const char* broker = MQTT_BROKER_IP;
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

WiFiClient wifi;
PubSubClient client(wifi);

void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient", USER, USER_PASSWORD)) {
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

void wifi_reconnect() {
  Serial.print("Attempting Wifi connection...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

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
  wifi_reconnect();

  client.setServer(broker, 1883);
  client.setCallback(callback);

  // digitalWrite(led_green, HIGH);
};

void loop() {
  int statusCode;
  const char* response;
  char buffer[20];  // Puffer für den konvertierten String
  String payload = buffer;
  int sensorValue = 0;

  switch (state) {
    case WAKE_UP:
      Serial.println("Waking up...");
      wifi_reconnect();
      if (!client.connected()) {
        mqtt_reconnect();
      };
      state = AWAKE;
      break;

    case ONLY_WIFI:
      if (!client.connected()) {
        mqtt_reconnect();
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
  