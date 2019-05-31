#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>

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
        void setup(String topic);
        void configMode(const String html);
        void publish(char* topic, char* msg) { client.publish(topic, msg); }
        void loop() { setup("wifi-lamp"); client.loop(); }
};