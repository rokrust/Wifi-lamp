#include <Arduino.h>
#include "button.h"
#include "webInterface.h"

const byte interruptPin = 13;
volatile byte val = 255;

Button button(13, 500, 1000);
WebInterface web;


void singleClick() {
  Serial.println("Event: Single click");
}

void doubleClick() {
  Serial.println("Event: Double click");
  web.configMode("<p>You have entered config mode</p>");
}

void setup() {
  delay(3000);
  Serial.begin(9600);

  button.onEvent("click", singleClick);
  button.onEvent("double-click", doubleClick);
  button.activate();

}

void loop() {
  web.loop();
}