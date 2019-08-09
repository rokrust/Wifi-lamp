#include "buttonmodule.h"


void ButtonModule::addButton(const Button& button)
{
    buttonList.push_back(button);
}

void ButtonModule::setup()
{
    addButton(Button(13, 500, 1000));
}

void ButtonModule::loop()
{

}