#pragma once
#include <Arduino.h>
#include <ticker.h>
#include "timer.h"

enum State { NONE, SINGLE_CLICK, DOUBLE_CLICK, LONG_CLICK, LONG_SHORT_CLICK, SHORT_LONG_CLICK, DOUBLE_LONG_CLICK, NSTATES };
enum ClickType { SHORT, LONG };

class Fsm
{
    private:
        State currentState;
        Timer timer;
        Ticker ticker;
        unsigned char currentValue;
        unsigned int shortClickDuration, sequenceTimeout;
        void (*callbacks [NSTATES])();

        void handleInputChange(unsigned char val);
        void transition(ClickType clickType);
        void invokeEventCallback(void (*callback)());

    public:
        Fsm(unsigned int clickDuration=500, unsigned int timeout=1000);
        
        void setCallback(State state, void (*callback)()) { callbacks[(int)state] = callback; }
        void input(unsigned char val);
};