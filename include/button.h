#include <string>
#include "Ticker.h"

enum STATE { IDLE, SINGLE_CLICK, DOUBLE_CLICK, LONG_PRESS };
typedef void (*func_t)();

//When button is pressed:
    //If state is idle: Measure click time
        //If short click: start timer, state = click
        //Else: state = long press

class Button
{
    private:
        unsigned char buttonPin;
        unsigned int clickTimeOut, shortClickDuration;
        void (*singleClickCallback)(), (*doubleClickCallback)();
        enum STATE state;
        Ticker timer;

    public:
        Button();
        Button(unsigned char pin);

        void onEvent(std::string e, void (*callback)());
        void handleButtonPress();

};
