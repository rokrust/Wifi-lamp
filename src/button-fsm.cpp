#include <Arduino.h>
#include <functional>
#include "button-fsm.h"

Fsm::Fsm(unsigned int clickDuration, unsigned int timeout)
{
    shortClickDuration = clickDuration;
    sequenceTimeout = timeout;
    currentValue = LOW;
    currentState = NONE;
    
    for(int i = 0; i < NSTATES; i++)
    {
        callbacks[i] = NULL;
    }
}

void Fsm::invokeEventCallback(std::function<void()> callback)
{
    currentState = NONE; 
    if(callback)
    {
        callback(); 
        Serial.println("State reset");
    } 
} 

void Fsm::handleInputChange(unsigned char val)
{   
    //From low to high. Click initiated
    if(val == HIGH)
    {
        timer.start(); //start measuring click length
    }

    //From High to low. Click stopped
    else
    {
        //Identify click type
        if(timer.time() <= shortClickDuration)
        {
            transition(SHORT);
        }

        else
        {
            Serial.println("Long click");
            transition(LONG);
        }
    }
    
}


// TODO: reset state to NONE after callback is called

void Fsm::transition(ClickType clickType)
{
    Serial.print("Current state: ");
    Serial.print(currentState);
    Serial.print("\n");
    switch(currentState)
    {
        case NONE:
            currentState = (State)(clickType == SHORT ? SINGLE_CLICK : LONG_CLICK);
            break;
        case SINGLE_CLICK:
            currentState = (State)(clickType == SHORT ? DOUBLE_CLICK : SHORT_LONG_CLICK);
            break;
        case LONG_CLICK:
            currentState = (State)(clickType == SHORT ? LONG_SHORT_CLICK : DOUBLE_LONG_CLICK);
            break;
        case DOUBLE_CLICK:
            currentState = (State)(clickType == SHORT ? DOUBLE_CLICK : SHORT_LONG_CLICK);
            break;
        case LONG_SHORT_CLICK:
            currentState = (State)(clickType == SHORT ? DOUBLE_CLICK : SHORT_LONG_CLICK);
            break;
        case SHORT_LONG_CLICK:
            currentState = (State)(clickType == SHORT ? LONG_SHORT_CLICK : DOUBLE_LONG_CLICK);
            break;
        case DOUBLE_LONG_CLICK:
            currentState = (State)(clickType == SHORT ? LONG_SHORT_CLICK : DOUBLE_LONG_CLICK);
            break;
        default:
            currentState = currentState;
            break;
    }

    //Invoke callback after button sequence timeouts
    if(currentState == SINGLE_CLICK || currentState == LONG_CLICK)
    {
        schedule.detach();
        schedule.once_ms(sequenceTimeout, std::bind(&Fsm::invokeEventCallback, this, callbacks[(int)currentState]));
    }

    //Invoke callback immediately
    else
    {
        schedule.detach();
        invokeEventCallback(callbacks[(int)currentState]);
    }
    
}

void Fsm::input(unsigned char val)
{
    //Input has changed. Run state machine
    if(val != currentValue)
    {
        currentValue = val;
        handleInputChange(val);
    }
    
}
