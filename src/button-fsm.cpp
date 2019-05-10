void Fsm::handleInputChange(unsigned char val)
{
    //From low to high. Click initiated
    if(val == HIGH)
    {
        //if timeout, reset sequence
        if(timer.time() >= clickTimeout)
        {
            currentState = NONE;
        }

        timer.start(); //start measuring click length
    }

    //From High to low. Click stopped
    else
    {
        if(timer.time() <= shortClickDuration)
        {
            transition(SHORT);
            Serial.println("Short click");
        }

        else
        {
            transition(LONG);
            Serial.println("Long click");
        }
        
        //Start measuring click timeout
        timer.start();
    }
    
}

void Fsm::transition(ClickType clickType)
{
    switch(currentState)
    {
        case NONE:
            currentState = clickType == SHORT ? S : L;
            break;
        case S:
            currentState = clickType == SHORT ? SS : SL;
            break;
        case L:
            currentState = clickType == SHORT ? LS : LL;
            break;
        case SS:
            currentState = clickType == SHORT ? SS : SL;
            break;
        case LS:
            currentState = clickType == SHORT ? SS : SL;
            break;
        case SL:
            currentState = clickType == SHORT ? LS : LL;
            break;
        case LL:
            currentState = clickType == SHORT ? LS : LL;
            break;
        default:
            currentState = currentState;
            break;
    }

    Serial.println("Transitioned to state" + currentState);
}

void Fsm::input(unsigned char val)
{
    //Input has changed. Run state machine
    if(val != currentValue)
    {
        handleInputChange(val);
    }
    
}

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