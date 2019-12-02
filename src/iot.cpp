#include <Arduino.h>

#include "iot/iot.h"

namespace iot
{
    using namespace std;

    void ModulePack::setup()
    {
        for(int i = 0; i < _interceptors.size(); i++)
        {
            _interceptors[i]->setup();
        }

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
        module->setModuleBuffer(&_moduleBuffer);
        module->setRequestBuffer(&_requestBuffer);
        
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
        interceptor->setInterceptorBuffer(&_interceptorBuffer);
        interceptor->setModuleBuffer(&_moduleBuffer);

        _interceptors.push_back(interceptor);

    }

}