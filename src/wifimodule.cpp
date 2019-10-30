#include "wifimodule.h"
#include <ESP8266WiFi.h>


void WifiModule::connectWifi()
{
    /*if (!WiFi.isConnected() && hasCredentials)
    {
        WiFi.begin(credentials.ssid, credentials.password);
    }*/
}

void WifiModule::setup()
{
    IPAddress ip = IPAddress(10, 0, 0, 13);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Lamp Config");
    
    subscribe<WifiInfo>(&WifiModule::test);
}

void WifiModule::loop()
{
    //connectWifi();
}

void WifiModule::test(WifiInfo *msg)
{
    Serial.println("Wifi credentials received");
    Serial.print("SSID: ");
    Serial.println(msg->ssid);
    Serial.print("Password: ");
    Serial.println(msg->password);
}