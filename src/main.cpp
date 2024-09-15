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
  char* auth = base64_encoding();
  send_to_grafana(sensorValue, auth);
  delay(1000);  
}

// put function definitions here:
int send_to_grafana(int value, char* auth) {
    char buffer[20];  // Puffer für den konvertierten String
    // Konvertiere int in char*
    sprintf(buffer, "test,bar_label=abc,source=grafana_cloud_docs metric=%d", value);

    const char* serverAddress = "https://influx-prod-24-prod-eu-west-2.grafana.net/api/v1/push/influx/write"; 
    int port = 443; 
    WiFiClient wifi;
    HttpClient http = HttpClient(wifi, serverAddress, port);

    String postData = buffer;

    // HTTP POST-Anfrage vorbereiten
    Serial.println("begin request");
    http.beginRequest();
    http.post(serverAddress); 
    Serial.println("send header");
    http.sendHeader("Content-Type", "text/plain");
    http.sendHeader("Authorization", "Basic ");
    Serial.println("send ath");
    http.sendHeader("Authorization", auth);
    http.sendHeader("Content-Length", postData.length());
    http.beginBody();
    Serial.println("post body");
    http.print(postData);
    http.endRequest();
    Serial.println("end request");
   
    Serial.println("wait for status");
    int statusCode = http.responseStatusCode();

    String response = http.responseBody();


    Serial.print("Statuscode: ");
    Serial.println(statusCode);
    Serial.print("Antwort: ");
    Serial.println(response);

};

char* base64_encoding() {
    // Base64-Kodierung von "USER_ID:API_KEY"
    char auth[256];
    snprintf(auth, sizeof(auth), "%s:%s", USER_ID, API_KEY);
    
    int inputStringLength = strlen(auth);
    int encodedLength = Base64.encodedLength(inputStringLength);
    
    // Dynamischen Speicher für den kodierten String anfordern
    char* encodedString = (char*)malloc(encodedLength + 1);  // +1 für Null-Terminierung
    if (encodedString == nullptr) {
        return nullptr;
    }
    
    Base64.encode(encodedString, auth, inputStringLength);
    encodedString[encodedLength] = '\0';  // Null-Terminierung sicherstellen
    
    return encodedString;
}

