#pragma once

#include "iot.h"
#include "button-fsm.h"
#include "string"
#include <Arduino.h>

struct WifiInfo : public iot::Message
{
    //data fields
    String ssid;
    String password;

};

struct ButtonEvent : public iot::Message
{
    //data fields
    uint8_t buttonNumber;
    State clickType;

    static const unsigned int id = 2;
    unsigned int getId() { return ButtonEvent::id; }

    ButtonEvent* clone() { return this; }
};