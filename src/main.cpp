#include <Arduino.h>
#include "button.h"

const byte interruptPin = 13;
const byte R = 16;
const byte G = 5;
const byte B = 4;
Button button(13);


void handleInterrupt() {
  Serial.println("Button pressed");
  digitalWrite(R, LOW);
  digitalWrite(G, HIGH);
  digitalWrite(B, HIGH);
}

void setup() {
  Serial.begin(9600);

  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);

  digitalWrite(R, LOW);
  digitalWrite(G, LOW);
  digitalWrite(B, LOW);

  button.onEvent("click", handleInterrupt);
}

void loop() {
}