#include "webInterface.h"
#include "credentials.h"

#include <ESP8266mDNS.h>
#include <Arduino.h>

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

//reconnectServicesIfDisconnected
    //Connect to wifi
    //if connected to wifi
        //authorize mqtt    client.connect("Wifi-lamp", mqttServer.user, mqttServer.password);
        //Subscribe callbacks


/*
reconnectServicesIfDisconnected
    connect to wifi
    connect to mqtt

*/

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

