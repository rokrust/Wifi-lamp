#include <vector>
#include <string>
#include <map>
#include <queue>
#include <Arduino.h>
#include <type_traits>

#include "iot.h"

namespace iot
{
    using namespace std;

    /*
    void Module::send(Message* message) 
    {
        Serial.print("Sending message: ");
        Serial.println(message->getId());
        IotDevice::_messageQueue.push(message); 
    }
    */
    void ModulePack::setup()
    {
        for (unsigned int i = 0; i < _modules.size(); i++)
        {
            Serial.println("Setting up module");
            _modules[i]->setup();
        }
    }

    void ModulePack::loop()
    {
        for (unsigned int i = 0; i < _modules.size(); i++)
        {
            _modules[i]->loop();
            _modules[i]->receive();
        }
    }

    void ModulePack::addModule(Module *module)
    {
        module->setBuffer(&_buffer);
        _modules.push_back(module);
    }

    void ModulePack::removeModule(Module *module)
    {
        for(unsigned int i = 0; i < _modules.size(); i++)
        {
            if(_modules[i] == module)
                _modules.erase(_modules.begin() + i);
        }
    }

}