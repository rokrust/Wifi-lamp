#include<Arduino.h>
#include "button.h"

std::vector<Button*> Button::buttons;

Button::Button() 
{
    singleClickCallback = NULL;
    doubleClickCallback = NULL;
    state = IDLE;
    buttonPin = 0;
    clickTimeOut = 500;

    unsigned char previousValue = LOW;
    unsigned long previousValueTimeStamp = 0;

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


//When button is pressed:
    //Identify source


    //If state is idle: Measure click time
        //If short click: start timer
        //Else: state = long press
void Button::handleButtonPress()
{
    //Identify interrupt source
    

    if(state == IDLE)
    {

    }
}