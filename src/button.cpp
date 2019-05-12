#include<Arduino.h>
#include "button.h"

std::vector<Button*> Button::buttons;

Button::Button() 
{
    Button::buttons.push_back(this);
}

Button::Button(unsigned char pin): Button()
{
    buttonPin = pin;
    pinMode(buttonPin, INPUT);
}

Button::Button(unsigned char pin, unsigned int shortClickDuration, unsigned int sequenceTimeout)
{
    buttonPin = pin;
    stateMachine = Fsm(shortClickDuration, sequenceTimeout);
    buttons.push_back(this);
}

void Button::onEvent(std::string e, std::function<void()> callback)
{
    if(!e.compare("click"))
    {
        stateMachine.setCallback(SINGLE_CLICK, callback);
    }

    else if (!e.compare("double-click"))
    {
        stateMachine.setCallback(DOUBLE_CLICK, callback);
    }

    else if (!e.compare("long-click"))
    {
        stateMachine.setCallback(LONG_CLICK, callback);
    }
}

void Button::activate()
{
    buttons.push_back(this);
    attachInterrupt(digitalPinToInterrupt(buttonPin), &Button::handleButtonPress, CHANGE);
}

void Button::handleButtonPress()
{
    for(unsigned int i = 0; i < Button::buttons.size(); i++)
    {
        unsigned char value = digitalRead(buttons[i]->buttonPin);
        buttons[i]->stateMachine.input(value);
    }

}