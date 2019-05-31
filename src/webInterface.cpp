#include "webInterface.h"
#include <ESP8266mDNS.h>
#include <Arduino.h>

//Move these into separate fs module
struct WifiCredentials
{
    String ssid;
    String password;
};

struct MqttCredentials
{
    String name;
    String password;
    String user;
    int port;
};

struct Credentials
{
    WifiCredentials wifi;
    MqttCredentials mqtt;
};

const WifiCredentials wifiCredentials = {
    .ssid = "",
    .password = ""
};

const MqttCredentials mqttCredentials = { 
    .name = "", 
    .password = "", 
    .user = "", 
    .port = 0 
};

const Credentials credentials = {
    .wifi = wifiCredentials,
    .mqtt = mqttCredentials
};

WebInterface::WebInterface() : client(credentials.mqtt.name.c_str(), credentials.mqtt.port, espClient)
{
    client.setCallback([](char* inTopic, uint8_t* payload, unsigned int length){
        for(int i = 0; i < length; i++)
        {
            Serial.print((char)payload[i]);
        }
        Serial.println();
    });
}

void WebInterface::configMode(const String html)
{
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Lamp Config");

    server.begin();

    server.on("/", HTTP_GET, [this, html](){
        server.send(200, "text/html", html);
    });

    if(!MDNS.begin("lampconfig"))
    Serial.println("MDNS error");

}

//Setup
    //Connect to wifi
    //if connected to wifi
        //authorize mqtt    client.connect("Wifi-lamp", mqttServer.user, mqttServer.password);
        //Subscribe callbacks


/*
setup
    connect to wifi
    connect to mqtt

*/

void WebInterface::setup(String topic)
{
    //WiFi.mode(WIFI_STA);
    if(!WiFi.isConnected())
    {
        WiFi.begin(credentials.wifi.ssid, credentials.wifi.password);
    }

    if(!client.connected())
    {
        if(client.connect("Wifi-lamp", credentials.mqtt.user.c_str(), credentials.mqtt.password.c_str()))
        {
            client.subscribe(topic.c_str());
        }
    }

}

