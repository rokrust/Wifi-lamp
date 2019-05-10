#include<Arduino.h>
#include "button.h"

std::vector<Button*> Button::buttons;

Button::Button() 
{
    singleClickCallback = NULL;
    doubleClickCallback = NULL;
    buttonPin = 0;
    clickTimeOut = 500;

    previousValue = LOW;
    previousValueTimeStamp = 0;

    Button::buttons.push_back(this);
}

Button::Button(unsigned char pin): Button()
{
    buttonPin = pin;
    pinMode(buttonPin, INPUT);
}

void Button::onEvent(std::string e, void (*callback)())
{
    if(!e.compare("click"))
    {
        singleClickCallback = callback;
    }

    else if (!e.compare("double-click"))
    {
        doubleClickCallback = callback;        
    }
    //void (Button::*pt)() = NULL;
    //pt = &Button::handleButtonPress;

    //attachInterrupt(digitalPinToInterrupt(buttonPin), &Button::handleButtonPress, CHANGE);

}

void Button::activate()
{
    attachInterrupt(digitalPinToInterrupt(buttonPin), &Button::handleButtonPress, CHANGE);
}

//When button is pressed:
    //Identify source


    //If state is idle: Measure click time
        //If short click: start timer
        //Else: state = long press

//Input button state directly into the FSM
//The FSM will contain the timer and trigger
//the appropriate callback functions
// void Button::handleButtonPress()
// {
//     //Determine interrupt source
//     Button* interruptSource;
//     for(int i = 0; i < buttons.size(); i++)
//     {
//         interruptSource = buttons[i];
//         unsigned char value = digitalRead(interruptSource->buttonPin);

//         //Source of the interrupt located
//         if(interruptSource->previousValue != value)
//         {
//             interruptSource.previousValue = value;

//             //From low to high
//             if(value == HIGH)
//             {
//                 //If timeout since last click, then restart sequence

//                 interruptSource.timer.start();
//             }

//             //From high to low
//             else
//             {
//                 if(interruptSource.timer.time() <= shortClickDuration)
//                 {
//                     //Add short click to sequence
//                 }

//                 else
//                 {
//                     //Add long click to sequence
//                 }
                
//                 //Start timer to measure sequence timeout
//                 interruptSource.timer.start();
//             }
            
//         }
//     }

// }

void Button::handleButtonPress()
{
    //Determine interrupt source
    Button* interruptSource;
    for(int i = 0; i < buttons.size(); i++)
    {
        interruptSource = buttons[i];
        unsigned char value = digitalRead(interruptSource->buttonPin);
        interruptSource.stateMachine.input(value);
    }

}