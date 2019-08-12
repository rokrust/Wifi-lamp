#include "wifimodule.h"
#include <ESP8266WiFi.h>


void WifiModule::connectWifi()
{
    if (!WiFi.isConnected() && hasCredentials)
    {
        WiFi.begin(credentials.ssid, credentials.password);
    }
}

void test(WifiInfo* msg)
{
    Serial.println("Wifi credentials received");
    Serial.println("SSID: " + msg->ssid);
    Serial.println("Password: " + msg->password);
}

void WifiModule::setup()
{
    IPAddress ip = IPAddress(10, 0, 0, 10);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Lamp Config");

    subscribe(WifiInfo::id, test);
    /*
    subscribe(WifiInfo::id, [this](WifiInfo* message){
        credentials.ssid = message->ssid;
        credentials.password = message->password;

        hasCredentials = true;
    });
    */
}

void WifiModule::loop()
{
    connectWifi();
}