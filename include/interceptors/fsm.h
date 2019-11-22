#include "iot.h"
#include "messages.h"
class StateMachine : public iot::Interceptor
{
    private:

    public:
        void setup() 
        {
            filter<WifiAp>(true);
            translate<ButtonEvent>(&(StateMachine::translateButtonEvent));
        }

        void translateButtonEvent(ButtonEvent* buttonEvent)
        {
            switch(buttonEvent->clickType)
            {
                case SINGLE_CLICK:
                    send<WifiInfo>("ssid", "password");
                    break;
                case DOUBLE_CLICK:
                    break;
            }
        }
};