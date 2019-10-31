#include "modules/wifimodule.h"
#include <ESP8266WiFi.h>


void WifiModule::connectWifi()
{
    if (hasCredentials && !WiFi.isConnected())
    {
        WiFi.begin(credentials.ssid, credentials.password);
    }
}

void WifiModule::setup()
{
    IPAddress ip = IPAddress(10, 0, 0, 13);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Lamp Config");
    
    subscribe<WifiInfo>(&WifiModule::setCredentials);
}

void WifiModule::loop()
{
    connectWifi();
}

void WifiModule::setCredentials(WifiInfo *msg)
{
    credentials.ssid = msg->ssid;
    credentials.password = msg->password;
    hasCredentials = true;
}