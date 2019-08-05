#include "webInterface.h"
#include "credentials.h"

#include <ESP8266mDNS.h>
#include <Arduino.h>
#include <FS.h>

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

WebInterface::~WebInterface()
{
    _webServer.stop();
    WiFi.disconnect();
    espClient.stop();
    client.disconnect();
}

void WebInterface::startServer()
{
    ip = IPAddress(10, 0, 0, 10);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Lamp Config");

    _webServer.begin();

}

void WebInterface::connectToWifi(WifiCredentials wifiAp)
{
    //WiFi.mode(WIFI_STA);
    if(!WiFi.isConnected())
    {
        WiFi.begin(credentials.wifi.ssid, credentials.wifi.password);
    }

}

void WebInterface::connectToMqtt(MqttCredentials mqtt, String topic)
{
    if(!client.connected())
    {
        if(client.connect("Wifi-lamp", credentials.mqtt.user.c_str(), credentials.mqtt.password.c_str()))
        {
            client.subscribe(topic.c_str());
        }
    }

}

void WebInterface::reconnectServices(Credentials credentials, String topic)
{
    connectToWifi(credentials.wifi);
    connectToMqtt(credentials.mqtt, topic);
}