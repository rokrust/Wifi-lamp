#pragma once
#include "iot/module.h"
#include "iot/interceptor.h"

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <Arduino.h>

namespace iot 
{
    using namespace std;

    class ModulePack
    {
        private:
            vector<Module*> _modules;
            vector<Interceptor*> _interceptors;

            Buffer _messageBuffer;
            Buffer _requestBuffer;
            InterceptorBuffer _interceptorBuffer;

        public:
            ModulePack() { }

            void setup();
            void loop();

            void add(Module* module);
            void add(Interceptor* interceptor);
            
            void remove(Module* module);
            void remove(Interceptor* interceptor);

            //~ModulePack() { for (unsigned int i = 0; i < _modules.size(); i++) delete _modules[i]; }
    };

    class IotDevice
    {
        private:
            ModulePack* _modules;

        public:
            IotDevice() {}
            IotDevice(ModulePack* modulePack) { _modules = modulePack; }

            void setup() { _modules->setup(); }
            void loop() { _modules->loop(); }

    };

}