#pragma once

#include <Arduino.h>

class Timer
{
    protected:
        unsigned long tic;
        bool isStarted = false;

    public:
        void start() { tic = millis(); isStarted = true;}
        bool started() { return isStarted; }
        void stop() { isStarted = false; }
        unsigned long time() { return millis() - tic; }

};