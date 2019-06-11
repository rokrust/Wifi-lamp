#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <FS.h>
#include "credentials.h"

class WebInterface
{
    private:
        //For configuration through AP
        ESP8266WebServer server;
        IPAddress ip;

        //For MQTT / communication
        WiFiClient espClient;
        PubSubClient client;

        //SPIFFS file handling
        File fileHandle;

        String getContentType(String path);
        void uploadFile(String path);

    public:
        WebInterface();
        ~WebInterface();

        void setupServer();
        void connectToWifi(WifiCredentials wifiAp);
        void connectToMqtt(MqttCredentials mqtt, String topic);
        void reconnectServices(Credentials credentials, String topic);
        
        void publish(char* topic, char* msg) { client.publish(topic, msg); }
        void loop() { reconnectServices(credentials, "wifi-lamp"); client.loop(); }
};