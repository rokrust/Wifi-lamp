#include "iot.h"
#include "modules/webservermodule.h"
#include "modules/wifimodule.h"
#include "interceptors/fsm.h"
#include <Arduino.h>


iot::ModulePack modules;
iot::IotDevice device(&modules);

void setup() {
  Serial.begin(9600);
  modules.add(new WifiModule());
  modules.add(new WebServerModule());
  modules.add(new StateMachine());

  device.setup();
}

void loop() {
  device.loop();
}