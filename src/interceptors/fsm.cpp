#include "interceptors/fsm.h"
#include "messages.h"

void StateMachine::setup()
{
    Serial.println("Setting up FSM interceptor");
    filter<WifiAp>(true);
    translate<ButtonEvent>(&StateMachine::translateButtonEvent);
}

bool StateMachine::translateButtonEvent(ButtonEvent *buttonEvent)
{
    switch (buttonEvent->clickType)
    {
    case SINGLE_CLICK:
        send<WifiInfo>("ssid", "password");
        break;
    case DOUBLE_CLICK:
        break;
    }

    return false;
}