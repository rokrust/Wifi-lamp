#pragma once 

#include <string>
#include <vector>
#include <functional>
#include "timer.h"
#include "button-fsm.h"


//When button is pressed:
    //If state is idle: Measure click time
        //If short click: start timer, state = click
        //Else: state = long press

class Button
{
    private:
        unsigned char buttonPin;
        static std::vector<Button*> buttons;
        Fsm stateMachine;

    public:
        Button();
        Button(unsigned char pin);
        Button(unsigned char pin, unsigned int shortClickDuration, unsigned int sequenceTimeout);

        void activate();
        void onEvent(std::string e, std::function<void()> callback);
        static void handleButtonPress();

};
