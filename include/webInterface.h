#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
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

    public:
        WebInterface();
        void connectToWifi(WifiCredentials wifiAp);
        void connectToMqtt(MqttCredentials mqtt, String topic);
        void reconnectServices(Credentials credentials, String topic);
        
        void configMode(const String html);
        void publish(char* topic, char* msg) { client.publish(topic, msg); }
        void loop() { reconnectServices(credentials, "wifi-lamp"); client.loop(); }
};