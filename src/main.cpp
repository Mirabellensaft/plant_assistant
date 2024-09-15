#include <Arduino.h>
#include <SPI.h>
#include <WiFiS3.h>
#include <ArduinoHttpClient.h> 
#include <Base64.h> 
#include <secrets.h>

// Variables
int sensorPin = A0;
int sensorValue;
const char* serverAddress = "https://influx-prod-24-prod-eu-west-2.grafana.net/api/v1/push/influx/write"; 
int port = 443; 

// put function declarations here:
void sendPostRequest(const char* serverAddress, int port, const char* postData, int statusCode, String response);
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
  int statusCode;
  const char* response;
  
  int sensorValue = analogRead(sensorPin); 
  Serial.print("Analog Sensor Value: ");
  Serial.println(sensorValue); 
  Serial.print("IP Adresse: ");
  Serial.println(WiFi.localIP());
  long rssi = WiFi.RSSI();
  Serial.print("Signalstärke (RSSI): ");
  Serial.println(rssi);
  char buffer[20];  // Puffer für den konvertierten String
    // Konvertiere int in char*
  sprintf(buffer, "test,bar_label=abc,source=grafana_cloud_docs metric=%d", sensorValue);

  sendPostRequest(serverAddress, port, buffer, statusCode, response);

  Serial.print("Statuscode: ");
  Serial.println(statusCode);
  Serial.print("Antwort: ");
  Serial.println(response);

  delay(1000);  
}

// put function definitions here:
void sendPostRequest(const char* serverAddress, int port, const char* postData, int statusCode, String response) {
    
    char* auth = base64_encoding();
    WiFiClient wifi;
    HttpClient httpClient(wifi, serverAddress, port);
    
    httpClient.beginRequest();
    httpClient.post(serverAddress);  // Ersetze "/endpoint" durch deinen API-Endpunkt
    httpClient.sendHeader("Content-Type", "text/plain");
    httpClient.sendHeader("Authorization", "Basic ");
    httpClient.sendHeader("Authorization", auth);
    httpClient.sendHeader("Content-Length", strlen(postData));
    httpClient.beginBody();
    httpClient.print(postData);
    httpClient.endRequest();

    statusCode = httpClient.responseStatusCode();
    response = httpClient.responseBody();
};

char* base64_encoding() {
    // Base64-Kodierung von "USER_ID:API_KEY"
    char auth[256];
    snprintf(auth, sizeof(auth), "%s:%s", USER_ID, API_KEY);
    
    int inputStringLength = strlen(auth);
    int encodedLength = Base64.encodedLength(inputStringLength);
    char encodedString[encodedLength];
    
    Base64.encode(encodedString, auth, inputStringLength);
    encodedString[encodedLength] = '\0';  // Null-Terminierung sicherstellen
    
    return encodedString;
}