#pragma once

#include "iot.h"
#include "button-fsm.h"
#include "string"
#include <Arduino.h>

namespace{

    struct WifiInfo : public iot::Message
    {
        String ssid;
        String password;

        WifiInfo(String ssid, String password) : ssid(ssid), password(password) { }
    };

    struct WifiAp : public iot::Message
    {
        String ssid;
        int signalStrength;

        WifiAp(String ssid="", int signalStrength=0) : ssid(ssid), signalStrength(signalStrength) { }
    };

    struct ButtonEvent : public iot::Message
    {
        uint8_t buttonNumber;
        State clickType;

        ButtonEvent(uint8_t buttonNumber, State clickType) : buttonNumber(buttonNumber), clickType(clickType) {}
    };

}