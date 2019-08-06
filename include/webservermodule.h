#pragma once

#include "iot.h"
#include "messages.h"
#include <functional>
#include <Arduino.h>


void test(iot::Message* message);

class WebServerModule : public iot::NetworkModule
{
    public:
        void setup()
        {
            std::function<void(iot::Message* message)> f_ptr = test;
            subscribe(WifiInfo::id, f_ptr);
        }


        void loop()
        {

        }

};

void test(iot::Message* message) {}