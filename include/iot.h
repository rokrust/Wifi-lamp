#pragma once

#include <vector>
#include <string>
#include <map>
#include <functional>

namespace iot 
{
    using namespace std;

    namespace Messages { static unsigned int messageCount = 0; }
    template<typename msg> struct MessageId { static unsigned int id; };
    template<typename msg> unsigned int MessageId<msg>::id = Messages::messageCount++;

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
                std::vector<CallbackFunction> callbacks = _messages[MessageId<Msg>::id];
                for (int i = 0; i < callbacks.size(); i++)
                {
                    callbacks[i](message);
                }
            }

            template<typename Msg>
            void subscribe(std::function<void(Msg*)> callback)
            {
                _messages[MessageId<Msg>::id].push_back([callback](Message *message) { callback(static_cast<Msg *>(message)); });
            }

            template<typename ...Msg>
            void subscribe(std::function<void(Msg*)>&&... msg)
            {
                //Hacky pre-c++17 solution for passing variadic arguments to a function
                using expand_type = int[];
                expand_type{ 0, (subscribe<Msg>(msg), 0)... };
            }

    };

    class Interceptor
    {
        private:
            Buffer *_buffer;

        public:
            template <typename msg>
            void subscribe(std::function<void(msg *)> callback)
            {
                _buffer->subscribe<msg>(callback);
            }

            template <typename... Msg>
            void subscribe(std::function<void(Msg *)> &&... callback)
            {
                //Hacky pre-c++17 solution for passing variadic arguments to a function
                using expand_type = int[];
                expand_type{0, (subscribe<Msg>(callback), 0)...};
            }

            //This is unsafe as it expects the member function to belong to the calling instance
            template <typename Msg, typename ModuleInstance>
            void subscribe(void (ModuleInstance::*callback)(Msg *))
            {
                subscribe<Msg>([this, callback](Msg *msg) { (((ModuleInstance *)this)->*callback)(msg); });
            }

            //Explicit, safe version. Can set member function callback from a different class
            template <typename Msg, typename ModuleInstance>
            void subscribe(ModuleInstance *self, void (ModuleInstance::*callback)(Msg *))
            {
                subscribe<Msg>([self, callback](Msg *msg) { (self->*callback)(msg); });
            }

            void setBuffer(Buffer* buffer) { _buffer = buffer; }
    };

    class Module
    {
        private:
            Buffer *_messageBuffer;
            Buffer *_requestBuffer;
            Buffer *_interceptorBuffer;

        public:
            //Requires the message to have a constructor
            template <typename msg, typename... Args>
            void send(Args &&... args)
            {
                msg message = msg{args...};
                send(&(message));
            }

            template <typename msg>
            void send(msg *message)
            {
                _interceptorBuffer->send<msg>(message);
                _messageBuffer->send<msg>(message);
            }

            template <typename msg>
            void request()
            {
                msg message;
                _requestBuffer->send<msg>(&message);
            }

            template <typename msg>
            void respond(std::function<void(msg *)> callback)
            {
                _requestBuffer->subscribe<msg>(callback);
            }

            //This is unsafe as it expects the member function to belong to the calling instance
            template <typename Msg, typename ModuleInstance>
            void respond(void (ModuleInstance::*callback)(Msg *))
            {
                respond<Msg>([this, callback](Msg *msg) { (((ModuleInstance *)this)->*callback)(msg); });
            }

            template <typename msg>
            void subscribe(std::function<void(msg *)> callback)
            {
                _messageBuffer->subscribe<msg>(callback);
            }

            template <typename... Msg>
            void subscribe(std::function<void(Msg *)> &&... msg)
            {
                //Hacky pre-c++17 solution for passing variadic arguments to a function
                using expand_type = int[];
                expand_type{0, (subscribe<Msg>(msg), 0)...};
            }

            //This is unsafe as it expects the member function to belong to the calling instance
            template <typename Msg, typename ModuleInstance>
            void subscribe(void (ModuleInstance::*callback)(Msg *))
            {
                subscribe<Msg>([this, callback](Msg *msg) { (((ModuleInstance *)this)->*callback)(msg); });
            }

            //Explicit, safe version. Can set member function callback from a different class
            template <typename Msg, typename ModuleInstance>
            void subscribe(ModuleInstance *self, void (ModuleInstance::*callback)(Msg *))
            {
                subscribe<Msg>([self, callback](Msg *msg) { (self->*callback)(msg); });
            }

            void setInterceptorBuffer(Buffer *buffer) { _interceptorBuffer = buffer; }
            void setMessageBuffer(Buffer *buffer) { _messageBuffer = buffer; }
            void setRequestBuffer(Buffer *buffer) { _requestBuffer = buffer; }


            //Called once at the beginning of the program
            virtual void setup() = 0;

            //Called for each update of the system
            virtual void loop() = 0;

            //On returning true, module is removed from Modulepack
            virtual bool remove() { return false; }
    };

    class ModulePack
    {
        private:
            vector<Module*> _modules;
            Buffer _messageBuffer;
            Buffer _requestBuffer;
            Buffer _interceptorBuffer;

        public:
            ModulePack() { }

            void setup();
            void loop();

            void add(Module* module);
            void add(Interceptor* interceptor);
            template<typename msg> void add(std::function<void(msg* message)> callback) { _interceptorBuffer.subscribe<msg>(callback); }

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