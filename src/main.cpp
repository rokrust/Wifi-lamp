#include "button.h"
#include "iot.h"
#include "webInterface.h"
#include "webservermodule.h"
#include <Arduino.h>
#include <FS.h>

iot::IotDevice device;

void setup()
{
  device.addModule(new WebServerModule());
  device.setup();
}

void loop()
{
  device.loop();
}

/*
iot::IotDevice device;

const byte interruptPin = 13;
volatile byte val = 255;

Button button(13, 500, 1000);
WebInterface web;


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
  web.startServer();
}

void setup() {
  device.addModule(new WebServerModule());
  device.setup();


  delay(3000);
  Serial.begin(9600);
  if(!SPIFFS.begin())
  {
    Serial.println("Could not mount");
  }
  button.onEvent("click", singleClick);
  button.onEvent("double-click", doubleClick);
  button.activate();

}

void loop() {
  device.loop();
  web.loop();
  
}
*/