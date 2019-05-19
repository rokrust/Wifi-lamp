#include <Arduino.h>
#include <functional>
#include <ESP8266WiFi.h>
#include "button.h"
#include "rgbled.h"

const byte interruptPin = 13;
const byte rpin = 16;
const byte gpin = 5;
const byte bpin = 4;
volatile byte val = 255;
RgbLed led(16, 5, 4);
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

  led.on(150, 60, 120);
  if(WiFi.softAP("ESP8266 test", "")) Serial.println("All ready to go");

  button.onEvent("click", [](){ led.dim(14); });
  button.onEvent("double-click", [](){ led.brighten(10); });
  button.activate();
}

void loop() {
}