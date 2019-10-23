#pragma once

#include "broadcaster.h"

#include <vector>
#include <string>
#include <map>
#include <typeindex>
#include <functional>
#include <Arduino.h>


namespace iot 
{
    using namespace std;

    class IotDevice;

    struct Message
    {
    };

    class Buffer
    {
        private:
            std::map<std::type_index, Message*> _messages;

        public:

            template<typename msg, typename... Args>
            void send(Args &&... args)
            {
                //Delete old message first
                ;
                _messages[std::type_index(typeid(msg))] = new msg{args...};
            }

            template<typename msg>
            msg* getMessage()
            {
                auto message = _messages.find(std::type_index(typeid(msg)));
                if (message != end(_messages))
                    return static_cast<msg *>(message->second);

                return nullptr;
            }

            template<typename msg>
            msg* receive()
            {
                msg* message = getMessage<msg>();
                if(message && message->isNew)
                    return message;
                
                return nullptr;
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
            void send(Args &&... args) { _buffer->send<msg>(args...); /*_buffer->send<msg>(std::forward<Args>(args)...);*/ }
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

            ~ModulePack() { for (int i = 0; i < _modules.size(); i++) delete _modules[i]; }
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