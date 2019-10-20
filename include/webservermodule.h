#pragma once

#include "iot.h"
#include "messages.h"
#include <functional>
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <FS.h>

template <class... Messages_t>
class WebServerModule : public iot::Module
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
        void setup();
        void loop();
        WebServerModule* clone() { return this; }

};