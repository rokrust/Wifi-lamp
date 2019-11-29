#pragma once

#include "iot/iot.h"
#include "messages.h"
class StateMachine : public iot::Interceptor
{
    private:

    public:
        void setup();
        bool translateButtonEvent(ButtonEvent* buttonEvent);
        bool filterWifiAp(WifiAp *ap);
};