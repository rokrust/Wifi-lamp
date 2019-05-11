#pragma once

#include <Arduino.h>

class Timer
{
    private:
        unsigned long tic;

    public:
        void start() { tic = millis(); }
        unsigned long time() { return millis() - tic; }
};