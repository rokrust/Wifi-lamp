#include<Arduino.h>
#include "button.h"

Button::Button() 
{
    singleClickCallback = NULL;
    doubleClickCallback = NULL;
    state = IDLE;
    buttonPin = 0;
    clickTimeOut = 500;
}

Button::Button(unsigned char pin): Button()
{
    buttonPin = pin;
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

    attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonPress, RISING);

}


//When button is pressed:
    //If state is idle: Measure click time
        //If short click: start timer
        //Else: state = long press
void Button::handleButtonPress()
{
    if(state == IDLE)
    {
        unsigned int clickLength = pulseIn(buttonPin, HIGH);
        Serial.println(clickLength);

        //Short click
        if(clickLength <= shortClickDuration)
        {
            state = SINGLE_CLICK;
            //timer.attach_ms(clickTimeout);
        }

        //Long click
        else
        {
            state = LONG_PRESS;
        }
        
        //timer.once_ms(clickTimeOut,);
    }
}