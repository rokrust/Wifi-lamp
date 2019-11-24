#include "iot.h"
#include "modules/webservermodule.h"
#include "modules/wifimodule.h"
#include "modules/test.h"
#include "interceptors/fsm.h"
#include <Arduino.h>


iot::ModulePack modules;
iot::IotDevice device(&modules);

void setup() {
  Serial.begin(9600);
  //modules.add(new WifiModule());
  //modules.add(new WebServerModule());
  modules.add(new SendTest());
  modules.add(new ReceiveTest());
  modules.add(new InterceptorTest());

  device.setup();
}

void loop() {
  device.loop();
}