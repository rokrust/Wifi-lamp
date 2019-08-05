#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <FS.h>
#include "credentials.h"
#include "webserver.h"

class WebInterface
{
    private:
        //For configuration through AP
        IPAddress ip;
        WebServer _webServer;

        //For MQTT / communication
        WiFiClient espClient;
        PubSubClient client;

    public:
        WebInterface();
        ~WebInterface();

        void startServer();

        void connectToWifi(WifiCredentials wifiAp);
        void connectToMqtt(MqttCredentials mqtt, String topic);
        void reconnectServices(Credentials credentials, String topic);
        
        void publish(char* topic, char* msg) { client.publish(topic, msg); }
        void loop() { reconnectServices(credentials, "wifi-lamp"); client.loop(); _webServer.loop();}
};