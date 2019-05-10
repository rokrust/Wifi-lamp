#include "timer.h"

enum States { NONE, S, SS, L, LS, SL, LL };
enum ClickType { SHORT, LONG };

class Fsm
{
    private:
        States currentState;
        Timer timer;
        unsigned char currentValue;
        unsigned int shortClickDuration, clickTimeOut;
        handleInputChange(unsigned char val);
        transition(ClickType clickType);

    public:
        Fsm() { currentValue = LOW; }
        Fsm(unsigned char startValue) { currentValue = startValue; }
        void input(unsigned char val);
};