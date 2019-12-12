#pragma once
#include "iot/messagebuffers.h"

namespace iot
{
    class Module
    {
        private:
            Buffer *_requestBuffer;
            InterceptorBuffer *_interceptorBuffer;
            ModuleBuffer *_moduleBuffer;

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
                _interceptorBuffer->send<msg>(message) && _moduleBuffer->send<msg>(message);
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
                _requestBuffer->subscribe<msg>(callback, false);
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
                _moduleBuffer->subscribe<msg>(callback);
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
                subscribe<Msg, ModuleInstance>((ModuleInstance *)this, callback);
            }

            //Explicit, safe version. Can set member function callback from a different class
            template <typename Msg, typename ModuleInstance>
            void subscribe(ModuleInstance *self, void (ModuleInstance::*callback)(Msg *))
            {
                subscribe<Msg>([self, callback](Msg *msg) { (self->*callback)(msg); });
            }

            void setInterceptorBuffer(InterceptorBuffer *buffer) { _interceptorBuffer = buffer; }
            void setModuleBuffer(ModuleBuffer *buffer) { _moduleBuffer = buffer; }
            void setRequestBuffer(Buffer *buffer) { _requestBuffer = buffer; }

            //Called once at the beginning of the program
            virtual void setup() = 0;

            //Called for each update of the system
            virtual void loop() = 0;

            //On returning true, module is removed from Modulepack
            virtual bool remove() { return false; }
    };    
}
