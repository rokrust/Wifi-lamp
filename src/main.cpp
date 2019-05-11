#include <Arduino.h>
#include "button.h"

const byte interruptPin = 13;
const byte rpin = 16;
const byte gpin = 5;
const byte bpin = 4;
volatile byte val = 255;
Button button(13, 500, 1000);


void singleClick() {
  Serial.println("Event: Single click");

  digitalWrite(rpin, val);
  digitalWrite(gpin, val);
  digitalWrite(bpin, val);
}

void doubleClick() {
  Serial.println("Event: Double click");

  val -= 20;
  Serial.println(val);
  analogWrite(rpin, val);
  analogWrite(gpin, val);
  analogWrite(bpin, val);
}

void setup() {
  Serial.begin(9600);

  pinMode(rpin, OUTPUT);
  pinMode(gpin, OUTPUT);
  pinMode(bpin, OUTPUT);

  digitalWrite(rpin, LOW);
  digitalWrite(gpin, LOW);
  digitalWrite(bpin, LOW);

  button.onEvent("click", singleClick);
  button.onEvent("double-click", doubleClick);
  button.activate();
}

void loop() {
}