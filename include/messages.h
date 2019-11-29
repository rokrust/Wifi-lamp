#pragma once

#include "iot/iot.h"
#include "button-fsm.h"
#include "string"
#include <Arduino.h>

namespace
{

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

    struct TestMessage : public iot::Message
    {
        int val;
        TestMessage(int val=0) : val(val) {}
        
        void serialize() {}
    };

    struct ReceiveTestMessage : public iot::Message
    {
        int val;
        ReceiveTestMessage(int val=0) : val(val) {}
    };

}