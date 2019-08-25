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

    class Message
    {
        public:
        static const unsigned int id = 0;
        virtual unsigned int getId() { return Message::id; };
        virtual Message* clone() = 0;
        virtual ~Message() = 0;
    };


    class NetworkModule
    {

        public:
            virtual void setup() = 0;
            virtual void loop() = 0;

            template <class Callback> void subscribe(Callback callback) { IotDevice::_broadCaster.subscribe(callback); }
            template <class Msg> void send(Msg *message) { IotDevice::_broadCaster.send(message); };
    };

    template<class ...modules_t>
    class ModulePack
    {
        private:
            vector<NetworkModule*> _modules;

        public:
            ModulePack() { _modules = { new modules_t... }; }

            void setup();
            void loop();

            void removeModule(NetworkModule* module);
            void addModule(NetworkModule* module);
    };

    class IotDevice
    {
        friend class NetworkModule;

        private:
            //Messages are sent to this queue and broadcast to all subscribed modules
            static queue<Message*> _messageQueue;
            template<typename ...msg_t> static BroadCaster<msg_t...> _broadCaster;
            template <typename ...module_t> static ModulePack<module_t...> _modules;

        public:
            template<typename modules, typename messages>
            IotDevice(modules modulePack, messages messagePack);

            void setup();
            void loop();
            void addModule(NetworkModule* module);
            void removeModule(NetworkModule* module);
            void broadCastMessage(Message* message);

            ~IotDevice();
    };

    template <typename... Msg_t>
    class IotDevice::_broadCaster<Msg_t...> { };

    template <typename... Module_t>
    class IotDevice::_modules<Module_t...> { };
}