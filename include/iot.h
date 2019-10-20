#pragma once

#include "broadcaster.h"

#include <vector>
#include <string>
#include <map>
#include <queue>
#include <functional>
#include <utility>
#include <Arduino.h>


namespace iot 
{
    using namespace std;

    template <typename ...Messages_t>
    class IotDevice;

    class Message
    {
        public:
            static const unsigned int id = 0;
            virtual unsigned int getId() { return Message::id; };
            virtual Message* clone() = 0;
            virtual ~Message() = 0;
    };

    class Module
    {
        private:
            //BroadCaster<Messages_t...>* _broadCaster;
            //IotDevice *device;

        public:
            //Called once at the beginning of the program
            virtual void setup() = 0;

            //Called for each update of the system
            virtual void loop() = 0;

            //void setBroadcaster(BroadCaster<Messages_t...> *broadCaster) { _broadCaster = broadCaster; }

            //template <class Callback> void subscribe(Callback callback) { device->_broadCaster.subscribe(callback); }
            template <class Msg>
            void send(Msg *message){ /*_broadCaster->send(message);*/ }
    };

    class ModulePack
    {
        private:
            vector<Module/*<Messages_t...>*/*> _modules;

        public:
            ModulePack() { }

            void setup();
            void loop();

            void removeModule(Module* module);
            void addModule(Module* module);
    };

    template<typename ...Messages_t>
    class IotDevice
    {
        private:
            Broadcaster<Messages_t...> _broadcaster;
            ModulePack _modules;

        public:
            //template<typename modules, typename messages>
            IotDevice(ModulePack modulePack, Broadcaster<Messages_t...> broadcaster)
            {
                _modules = modulePack;
                setBroadcaster(broadcaster);
            }

            void setBroadcaster(Broadcaster<Messages_t...> broadcaster) 
            { 
                _broadcaster = broadcaster;
                for(int i = 0; i < _modules.size(); i++) 
                    _modules[i].setBroadcaster(broadcaster); 
            }

            void setup() { _modules.setup(); }
            void loop() { _modules.loop(); }

            ~IotDevice();
    };

    //template <typename... Msg_t>
    //class IotDevice::_broadCaster<Msg_t...> { };

    //template <typename... Module_t>
    //class IotDevice::_modules<Module_t...> { };
}