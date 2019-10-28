// TODO
// Make it possible to subscribe to a message with member functions

#include "iot.h"
#include "webservermodule.h"
#include "wifimodule.h"
#include "buttonmodule.h"
#include <Arduino.h>


iot::ModulePack modules;
iot::IotDevice device(&modules);

void setup() {
  delay(3000);
  Serial.begin(9600);c
  modules.addModule(new WifiModule());
  modules.addModule(new WebServerModule());

  device.setup();
}

void loop() {
  device.loop();
}