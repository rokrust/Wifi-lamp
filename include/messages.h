#pragma once

#include "iot.h"
#include "button-fsm.h"
#include "string"
#include <Arduino.h>

namespace{

    struct WifiInfo : public iot::Message
    {
        static const unsigned int id = 1;

        //data fields
        String ssid;
        String password;

        WifiInfo(String ssid, String password) : ssid(ssid), password(password) { }
    };


    struct ButtonEvent : public iot::Message
    {
        static const unsigned int id = 2;

        //data fields
        uint8_t buttonNumber;
        State clickType;

        ButtonEvent(uint8_t buttonNumber, State clickType) : buttonNumber(buttonNumber), clickType(clickType) {}
    };

}