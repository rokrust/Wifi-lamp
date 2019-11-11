#pragma once

#include "iot.h"
#include "messages.h"
#include "credentials.h"
#include "timer.h"

class WifiModule : public iot::Module
{

    private:
        WifiCredentials credentials;
        bool hasCredentials = false, isConnecting = false;
        Timer connectionTimer, scanTimer;
        unsigned int timeout;

        bool connectionTimeout();
        void handleConnectionTimeout();
        void scanWifi(WifiAp* ap);


    public:
        void setup();
        void loop();

        void connectBlocking(unsigned int timeout = 10);
        void connect(unsigned int timeout=10);
        void setupAp();
        void disconnect();
        bool canConnect();

        void setCredentials(WifiInfo* msg);
};