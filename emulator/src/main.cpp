#include <Arduino.h>

#define LED_PIN 2

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);  // turn LED on
  delay(500);                   // wait 500ms
  digitalWrite(LED_PIN, LOW);   // turn LED off
  delay(500);
}
