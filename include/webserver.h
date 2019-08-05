#include <ESP8266WebServer.h>
#include <FS.h>
#include <Arduino.h>

class WebServer
{

    private:
        ESP8266WebServer server;
        File fileHandle;

        void onFileUpload();
        void onWifiCredentialsReceived();
        void onResourceRequested();
        String getContentType(String path);
        String removePath(String path);
        void uploadFile(String path);

    public:
        WebServer();
        void begin() { server.begin(); }

        void setup();
        void loop();
};