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
            if(_modules[i]->remove())
            {
                //_modules.erase(_modules.begin() + i--);
                //deallocate callback functions
                continue;
            }

            _modules[i]->loop();

        }
    }

    void ModulePack::add(Module *module)
    {
        module->setInterceptorBuffer(&_interceptorBuffer);
        module->setRequestBuffer(&_requestBuffer);
        module->setMessageBuffer(&_messageBuffer);
        
        _modules.push_back(module);
    }

    void ModulePack::remove(Module *module)
    {
        for(unsigned int i = 0; i < _modules.size(); i++)
        {
            if(_modules[i] == module)
                _modules.erase(_modules.begin() + i);
        }
    }

    void ModulePack::add(Interceptor* interceptor)
    {
        interceptor->setBuffer(&_messageBuffer);
    }

}