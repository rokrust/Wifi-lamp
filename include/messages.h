#pragma once

#include "iot.h"
#include "string"
#include <Arduino.h>

struct WifiInfo : public iot::Message
{
    //data fields
    String ssid;
    String password;

    static const unsigned int id = 1;
    unsigned int getId() { return WifiInfo::id; }
    WifiInfo(String ssid, String password) : ssid(ssid), password(password) {}
    ~WifiInfo() {}
};

struct WifiInfo : public iot::Message
{
    //data fields
    String ssid;
    String password;

    static const unsigned int id = 2;
    unsigned int getId() { return WifiInfo::id; }
    WifiInfo(String ssid, String password) : ssid(ssid), password(password) {}
    ~WifiInfo() {}
};