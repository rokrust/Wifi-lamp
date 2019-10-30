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
            template<typename Msg>
            void send(Message* message)
            {
                std::vector<CallbackFunction> callbacks = _messages[Msg::id];
                for (int i = 0; i < callbacks.size(); i++)
                {
                    callbacks[i](message);
                }
            }

            template<typename Msg>
            void subscribe(std::function<void(Msg*)> callback)
            {
                _messages[Msg::id].push_back([callback](Message *message) { callback(static_cast<Msg *>(message)); });
            }

            template<typename ...Msg>
            void subscribe(std::function<void(Msg*)>&&... msg)
            {
                //Hacky pre-c++17 solution for passing variadic arguments to a function
                using expand_type = int[];
                expand_type{ 0, (subscribe<Msg>(msg), 0)... };
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

            //Requires the message to have a constructor
            template<typename msg, typename... Args>
            void send(Args &&... args) 
            {     
                msg message = msg{args...};
                _buffer->send<msg>(&(message)); 
            }

            template <typename msg>
            void send(msg* message)
            {
                _buffer->send<msg>(message);
            }

            template<typename msg>
            void subscribe(std::function<void(msg*)> callback)
            {
                _buffer->subscribe<msg>(callback);
            }

            template<typename ...Msg>
            void subscribe(std::function<void(Msg*)>&&... msg)
            {
                //Hacky pre-c++17 solution for passing variadic arguments to a function
                using expand_type = int[];
                expand_type{0, (subscribe<Msg>(msg), 0)...};
            }

            //This is unsafe as it expects the member function to belong to the calling instance
            template<typename Msg, typename ModuleInstance>
            void subscribe(void(ModuleInstance::*callback)(Msg*))
            {
                using namespace std::placeholders;
                subscribe<Msg>([this, callback](Msg *msg) { (((ModuleInstance*)this)->*callback)(msg); });
            }

            //Explicit, safe version. Can set member function callback from a different class
            template <typename Msg, typename ModuleInstance>
            void subscribe(ModuleInstance* self, void (ModuleInstance::*callback)(Msg *))
            {
                using namespace std::placeholders;
                subscribe<Msg>([self, callback](Msg *msg) { (self->*callback)(msg); });
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

            //~ModulePack() { for (unsigned int i = 0; i < _modules.size(); i++) delete _modules[i]; }
    };

    class IotDevice
    {
        private:
            ModulePack* _modules;

        public:
            IotDevice() {}
            IotDevice(ModulePack* modulePack)
            {
                _modules = modulePack;
            }

            void setup() { _modules->setup(); }
            void loop() { _modules->loop(); }

    };

    //template <typename... Msg_t>
    //class IotDevice::_broadCaster<Msg_t...> { };

    //template <typename... Module_t>
    //class IotDevice::_modules<Module_t...> { };
}