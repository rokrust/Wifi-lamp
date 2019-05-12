#pragma once
#include <Arduino.h>
#include <ticker.h>
#include <functional>
#include "timer.h"

enum State { NONE, SINGLE_CLICK, DOUBLE_CLICK, LONG_CLICK, LONG_SHORT_CLICK, SHORT_LONG_CLICK, DOUBLE_LONG_CLICK, NSTATES };
enum ClickType { SHORT, LONG };

class Fsm
{
    private:
        State currentState;
        Timer timer;
        Ticker schedule;
        unsigned char currentValue;
        unsigned int shortClickDuration, sequenceTimeout;
        std::function<void()> callbacks[NSTATES];

        void handleInputChange(unsigned char val);
        void transition(ClickType clickType);
        void invokeEventCallback(std::function<void()> callback);

    public:
        Fsm(unsigned int clickDuration=500, unsigned int timeout=1000);
        
        void setCallback(State state, std::function<void()> callback) { callbacks[(int)state] = callback;}
        void input(unsigned char val);
};