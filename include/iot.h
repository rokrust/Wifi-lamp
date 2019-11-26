#pragma once

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <Arduino.h>

namespace iot 
{
    using namespace std;

    namespace Messages { static unsigned int messageCount = 0; }
    template<typename msg> struct MessageId { static const unsigned int id; };
    template<typename msg> const unsigned int MessageId<msg>::id = Messages::messageCount++;

    struct Message { };

    class Buffer
    {
        using CallbackFunction = std::function<bool(Message *)>;

        private:
            std::map<unsigned int, std::vector<CallbackFunction>> _messages;

        public:
            template<typename Msg>
            bool send(Message* message)
            {
                std::vector<CallbackFunction> callbacks = _messages[MessageId<Msg>::id];
                for (int i = 0; i < callbacks.size(); i++)
                {
                    if(!callbacks[i](message))
                        return false;
                }

                return true;
            }

            template<typename Msg>
            void subscribe(std::function<bool(Msg*)> callback)
            {
                _messages[MessageId<Msg>::id].push_back([callback](Message *message) { return callback(static_cast<Msg *>(message)); });
            }
            
            template <typename Msg>
            void subscribe(std::function<void(Msg *)> callback, bool drop)
            {
                subscribe<Msg>([callback, drop](Msg *message){ 
                    callback(message); 
                    return !drop;
                });
            }

            template<typename ...Msg>
            void subscribe(std::function<bool(Msg*)>&&... msg)
            {
                //Hacky pre-c++17 solution for passing variadic arguments to a function
                using expand_type = int[];
                expand_type{ 0, (subscribe<Msg>(msg), 0)... };
            }

    };

    class InterceptorBuffer
    {
        private:
            Buffer filter;
            Buffer translation;
            Buffer edit;
            
            enum InterceptorType
            {
                FILTER,
                TRANSLATOR,
                EDITOR
            };
            
            InterceptorType _currentInterceptorType;
            bool _dropTranslatedMessage = false;

        public:
            template <typename msg, typename ...Args>
            bool send(Args&&... args) 
            {
                msg message = {args...};
                return send(&message);
            }

            template <typename msg>
            bool send(msg* message)
            {
                /*if(_currentInterceptorType != TRANSLATOR)
                {
                    Serial.println("Can only send message from a translation type interceptor");
                    return true;
                }*/

                // return filter.send<msg>(message) &&
                //     translation.send<msg>(message) &&
                //     edit.send<msg>(message);
                
                bool passMessageOn = filter.send<msg>(message) && 
                    translation.send<msg>(message) && !_dropTranslatedMessage &&
                    edit.send<msg>(message);

                _dropTranslatedMessage = false;
                return passMessageOn;
            }

            template<typename msg>
            void subscribeFilter(std::function<bool(msg*)> callback) 
            { 
                filter.subscribe<msg>([this, callback](msg* message)
                { 
                    _currentInterceptorType = FILTER;
                    return !callback(message);
                }); 
            }
            
            template <typename Msg>
            void subscribeTranslator(std::function<bool(Msg *)> callback) 
            { 
                translation.subscribe<Msg>([this, callback](Msg* message)
                { 
                    _currentInterceptorType = TRANSLATOR;
                    _dropTranslatedMessage = callback(message);
                }, false); 
            }
            
            template <typename msg>
            void subscribeEditor(std::function<void(msg *)> callback) 
            { 
                edit.subscribe<msg>([this, callback](msg* message)
                {
                    _currentInterceptorType = EDITOR;
                    callback(message);
                    
                    return true;
                }); 
            }
    };

    class Interceptor
    {
        private:
            Buffer *_messageBuffer;
            InterceptorBuffer *_interceptorBuffer;

            bool _sentMessage = false;

        public:
            virtual void setup() = 0;

            void setInterceptorBuffer(InterceptorBuffer* buffer) { _interceptorBuffer = buffer; }
            void setMessageBuffer(Buffer *buffer) { _messageBuffer = buffer; }

            template<typename msg, typename ... Args>
            void send(Args &&... args)
            {
                msg message = msg{args...};
                send(&message);
            }

            template <typename msg>
            void send(msg* message)
            {
                _sentMessage = true;
                _interceptorBuffer->send<msg>(message) && _messageBuffer->send<msg>(message);
            }

            //event translation
            template <typename in, typename out>
            void translate(std::function<void(out*)> callback)
            {
                _interceptorBuffer->subscribeTranslator<in>([this, callback](in* inMsg){
                    Serial.println("Event");
                    out outMsg;
                    callback(&outMsg);
                    send<out>(&outMsg);
                    
                    return false; //dump "in" message
                });
            }

            //event translation
            template <typename in, typename out, typename InterceptorType>
            void translate(void (InterceptorType::*callback)(out *))
            {
                translate<in, out>([this, callback](out *msg) { Serial.println("Event"); (((InterceptorType*)this)->*callback)(msg); });
            }

            //direct translation
            template <typename in, typename out>
            void translate(std::function<void(in *, out *)> callback)
            {
                _interceptorBuffer->subscribeTranslator<in>([this, callback](in* inMsg)
                {
                    Serial.println("Direct");
                    out outMsg;
                    callback(inMsg, &outMsg);
                    send<out>(outMsg);

                    return false; //dump "in" message
                });
            }

            //direct translation
            template <typename in, typename out, typename InterceptorType>
            void translate(void(InterceptorType::*callback)(in *, out *))
            {
                translate<in, out>([this, callback](in *inMsg, out *outMsg) {
                    Serial.println("Direct");
                    (((InterceptorType*)this)->*callback)(inMsg, outMsg);
                });
            }
/*
            // //conditional translation
            // template <typename in, typename out>
            // void translate(std::function<bool(in*, out*)> callback)
            // {
            //     _interceptorBuffer->subscribeTranslator<in>([this, callback](in* inMsg)
            //     {
            //         out outMsg;
            //         if(callback(inMsg, &outMsg))
            //         {
            //             send<out>(outMsg);
            //             return false; //dump "in message" if a new message was sent
            //         }
                    
            //         return true; //pass "in message" onward if out wasn't sent
            //     });
            // }

            // //conditional translation
            // template<typename in, typename out, typename InterceptorType>
            // void translate(bool(InterceptorType::*callback)(in*, out*))
            // {
            //     translate<in, out>([this, callback](in* inMsg, out* outMsg)
            //     {
            //         return ((InterceptorType*)this)->*callback(inMsg, outMsg);
            //     });
            // }
*/
            //one to many translation - pass message on when true is returned
            template<typename in, typename classType>
            void translate(void(classType::*callback)(in*), bool drop=false)
            {
                _interceptorBuffer->subscribeTranslator<in>([this, callback, drop](in *msg) 
                { 
                    Serial.println("One to many"); 
                    (((classType *)this)->*callback)(msg); 
                    return drop;
                });
            }

            //Captures an incoming message and edits its data fields
            template<typename msg>
            void edit(std::function<void(msg*)> callback) { _interceptorBuffer->subscribeEditor<msg>(callback, false); }

            template<typename Msg, typename classType>
            void edit(void(classType::*callback)(Msg*))
            {
                _interceptorBuffer->subscribeEditor<Msg>([this, callback](Msg* msg)
                { 
                    (((classType*)this)->*callback)(msg);
                    return true;
                });
            }

            template<typename msg>
            void filter(std::function<bool(msg*)> filterFunction)
            {
                _interceptorBuffer->subscribeFilter(filterFunction);
            }

            template<typename Msg, typename classType>
            void filter(bool(classType::*callback)(Msg*))
            {
                _interceptorBuffer->subscribeFilter<Msg>([this, callback](Msg* msg){ return (((classType*)this)->*callback)(msg); });
            }

            template<typename msg>
            void filter(bool drop)
            { 
                _interceptorBuffer->subscribeFilter<msg>([drop](msg* message){ return drop; }); 
            }

            template <typename msg>
            void filter(bool* active)
            {
                _interceptorBuffer->subscribeFilter<msg>([active](msg *message) { return *active; });
            }

            template <typename msg, typename InterceptorType>
            void filter(bool InterceptorType::* active)
            {
                _interceptorBuffer->subscribeFilter<msg>([this, active](msg *message) { return *active; });
            }
    };

    class Module
    {
        private:
            Buffer *_messageBuffer;
            Buffer *_requestBuffer;
            InterceptorBuffer *_interceptorBuffer;

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
                _interceptorBuffer->send<msg>(message) && _messageBuffer->send<msg>(message);
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
                _messageBuffer->subscribe<msg>(callback, false);
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
                subscribe<Msg, ModuleInstance>((ModuleInstance*)this, callback);
            }

            //Explicit, safe version. Can set member function callback from a different class
            template <typename Msg, typename ModuleInstance>
            void subscribe(ModuleInstance *self, void (ModuleInstance::*callback)(Msg *))
            {
                subscribe<Msg>([self, callback](Msg *msg){ (self->*callback)(msg); });
            }

            void setInterceptorBuffer(InterceptorBuffer *buffer) { _interceptorBuffer = buffer; }
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