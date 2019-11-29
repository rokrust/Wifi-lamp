#pragma once

#include "iot/iot.h"
#include "messages.h"
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <FS.h>

class WebServerModule : public iot::Module
{
    private:
        ESP8266WebServer server;
        File fileHandle;

        void onFileUpload();
        void onWifiCredentialsReceived();
        void onResourceRequested();
        void onEventStreamRequested();

        String getContentType(String path);
        String removePath(String path);
        void uploadFile(String path);
        void setupWebRequests();

        bool sendWifi = false;

    public:
        void setup();
        void loop();

        void sendWifiApList(WifiAp* ap);

};