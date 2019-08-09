//#include "button.h"
#include "iot.h"
#include "credentials.h"
//#include "webInterface.h"
#include "webservermodule.h"
#include "wifimodule.h"
#include "buttonmodule.h"
#include <ESP8266WiFi.h>
#include <Arduino.h>


iot::IotDevice device;
/*
const byte interruptPin = 13;
volatile byte val = 255;

Button button(13, 500, 1000);
//WebInterface web;


void singleClick() {
  Serial.println("Event: Single click");
  Dir dir = SPIFFS.openDir("/");

  while(dir.next())
  {
    Serial.println(dir.fileName());
    //SPIFFS.remove(dir.fileName());
  }
}

void doubleClick() {
  Serial.println("Event: Double click");
  //web.startServer();
}
*/
void setup() {
  delay(3000);
  Serial.begin(9600);

  device.addModule(new WebServerModule());
  device.addModule(new WifiModule());
  device.addModule(new ButtonModule());
  
  device.setup();
}

void loop() {
  device.loop();
}