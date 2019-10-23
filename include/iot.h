#pragma once

#include "broadcaster.h"

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <Arduino.h>


namespace iot 
{
    using namespace std;

    class IotDevice;

    struct Message { };

    class Buffer
    {
        using CallbackFunction = std::function<void(Message *)>;

        private:
            std::map<unsigned int, std::vector<CallbackFunction>> _messages;

        public:
            template<typename msg>
            void send(Message* message)
            {
                //Delete old message first
                //std::vector<CallbackFunction>& callbacks = _messages[msg::id];
                CallbackFunction test = _messages[msg::id][0];
                for (int i = 0; i < _messages[msg::id].size(); i++)
                {
                    _messages[msg::id][i](message);
                }
            }

            template<typename msg>
            void subscribe(std::function<msg*> callback)
            {
                _messages[msg::id].push_back([&](Message* message){ callback(static_cast<msg*>(message)); });
            }

    };

    class Module
    {
            //BroadCaster<Messages_t...>* _broadCaster;
            //IotDevice *device;
        protected:
            Buffer* _buffer;

        public:
            //Called once at the beginning of the program
            virtual void setup() = 0;

            //Called for each update of the system
            virtual void loop() = 0;

            virtual void receive() {}

            template<typename msg, typename... Args>
            void send(Args &&... args) 
            {     
                msg message = msg{args...};
                _buffer->send<msg>(&(message)); 
            }
            void setBuffer(Buffer* buffer) { _buffer = buffer; }
    };

    class ModulePack
    {
        private:
            vector<Module*> _modules;
            Buffer _buffer;

        public:
            ModulePack() { }

            void setup();
            void loop();

            void removeModule(Module* module);
            void addModule(Module* module);

            ~ModulePack() { for (unsigned int i = 0; i < _modules.size(); i++) delete _modules[i]; }
    };

    class IotDevice
    {
        private:
            ModulePack _modules;

        public:
            IotDevice() {}
            IotDevice(ModulePack modulePack)
            {
                _modules = modulePack;
            }

            void setup() { _modules.setup(); }
            void loop() { _modules.loop(); }

    };

    //template <typename... Msg_t>
    //class IotDevice::_broadCaster<Msg_t...> { };

    //template <typename... Module_t>
    //class IotDevice::_modules<Module_t...> { };
}