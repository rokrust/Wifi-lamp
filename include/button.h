#pragma once 

#include <string>
#include <vector>
#include "Ticker.h"
#include "timer.h"
#include "button-fsm.h"

enum STATE { IDLE, SINGLE_CLICK, DOUBLE_CLICK, LONG_PRESS };
typedef void (*func_t)();

//When button is pressed:
    //If state is idle: Measure click time
        //If short click: start timer, state = click
        //Else: state = long press

class Button
{
    private:
        unsigned char buttonPin;
        unsigned int clickTimeOut, shortClickDuration;
        unsigned char previousValue;
        Timer timer;

        void (*singleClickCallback)(), (*doubleClickCallback)();

        static std::vector<Button*> buttons;

    public:
        Fsm stateMachine;
        Button();
        Button(unsigned char pin);

        void activate();
        void onEvent(std::string e, void (*callback)());
        static void handleButtonPress();

};
