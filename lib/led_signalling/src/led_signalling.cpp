#include <Arduino.h>
#include <SPI.h>


void led_state(int value, const int GREEN, const int BLUE, const int RED) {
    switch (value) {
        // Soil is dry
        case 401 ... 500:
            digitalWrite(BLUE, LOW);
            digitalWrite(GREEN, LOW);

            if (digitalRead(RED) == LOW) {
                digitalWrite(RED, HIGH);
            } else {
                digitalWrite(RED, LOW);
            }
            break;

        // Soil is ok
        case 201 ... 400:
            digitalWrite(BLUE, LOW);
            digitalWrite(GREEN, HIGH);
            digitalWrite(RED, LOW);
            break;
        // Soil is wet
        case 0 ... 200:
            digitalWrite(BLUE, HIGH);
            digitalWrite(GREEN, LOW);
            digitalWrite(RED, LOW);
            break;
    };
}