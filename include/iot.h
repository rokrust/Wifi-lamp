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

    template <typename modulePack_t, typename broadcaster_t>
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
            //IotDevice *device;

        public:
            virtual void setup() = 0;
            virtual void loop() = 0;

            //template <class Callback> void subscribe(Callback callback) { device->_broadCaster.subscribe(callback); }
            template <class Msg> void send(Msg *message) { /*device->_broadCaster.send(message);*/ };
    };

    template<class ...modules_t>
    class ModulePack
    {
        private:
            vector<Module*> _modules;

        public:
            ModulePack() { _modules = { new modules_t... }; }

            void setup();
            void loop();

            void removeModule(Module* module);
            void addModule(Module* module);
    };

    template<typename modulePack_t, typename broadCaster_t>
    class IotDevice
    {
        private:
            //Messages are sent to this queue and broadcast to all subscribed modules
            broadCaster_t _broadCaster;
            modulePack_t _modules;

        public:
            //template<typename modules, typename messages>
            IotDevice(modulePack_t modulePack, broadCaster_t messagePack);

            void setup();
            void loop();
            void addModule(Module* module);
            void removeModule(Module* module);
            void broadCastMessage(Message* message);

            ~IotDevice();
    };

    template <modulePack_t, broadCaster_t> class IotDevice(modulePack_t, broadCaster_t) -> IotDevice<modulePack_t, broadCaster_t>;
    //template <typename... Msg_t>
    //class IotDevice::_broadCaster<Msg_t...> { };

    //template <typename... Module_t>
    //class IotDevice::_modules<Module_t...> { };
}