#pragma once

#include "iot.h"
#include "messages.h"
#include "credentials.h"
#include <Arduino.h>

class WifiModule : public iot::Module
{
private:
    WifiCredentials credentials;
    bool hasCredentials = false;

    void connectWifi();

public:
    void setup();
    void loop();


    void receive(WifiInfo* message);
};