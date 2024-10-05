#ifndef PA_CONNECTIVITY_H
#define PA_CONNECTIVITY_H

#include <Arduino.h>
#include <SPI.h>
#include <WiFiS3.h>
#include <PubSubClient.h>

// function declarations:
void callback(char* topic, byte* payload, unsigned int length);
void mqtt_reconnect(PubSubClient* client, const char* user, const char* user_password);
void wifi_reconnect(const char* wifi_ssid, const char* wifi_password);

#endif