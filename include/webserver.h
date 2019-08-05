#include <ESP8266WebServer.h>
#include <FS.h>

class WebServer
{

    private:
        ESP8266WebServer server;
        File fileHandle;

        void onFileUpload();
        void onWifiCredentialsReceived();
        void onResourceRequested();

    public:
        void setup();
        void loop();
};