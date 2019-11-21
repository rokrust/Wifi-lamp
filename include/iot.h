#pragma once

#include <vector>
#include <string>
#include <map>
#include <functional>

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
            void subscribe(std::function<void(Msg *)> callback)
            {
                _messages[MessageId<Msg>::id].push_back([callback](Message *message) { 
                    callback(static_cast<Msg *>(message)); 
                    return true;
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

    struct InterceptorBuffer
    {
        Buffer filter;
        Buffer translation;
        Buffer edit;

        template <typename msg, typename ...Args>
        bool send(Args&&... args) 
        {
            msg message = {args...};
            return send(&message);
        }

        template <typename msg>
        bool send(msg* message)
        {
            return filter.send<msg>(message) &&
                   translation.send<msg>(message) &&
                   edit.send<msg>(message);
        }

        template<typename msg>
        void subscribeFilter(std::function<bool(msg*)> callback) { filter.subscribe(callback); }
        
        template <typename msg>
        void subscribeTranslator(std::function<bool(msg *)> callback) { translation.subscribe(callback); }
        
        template <typename msg>
        void subscribeEditor(std::function<void(msg *)> callback) { edit.subscribe(callback); }
    };

    class Interceptor
    {
        private:
            Buffer *_messageBuffer;
            InterceptorBuffer *_interceptorBuffer;

            enum InterceptorType { FILTER, TRANSLATOR, EDITOR };
            InterceptorType _currentInterceptorType;
            bool _sentMessage = false;

        public:
            virtual void setup() = 0;

            void setInterceptorBuffer(InterceptorBuffer* buffer) { _interceptorBuffer = buffer; }
            void setMessageBuffer(Buffer *buffer) { _messageBuffer = buffer; }

            template<typename msg, typename ... Args>
            void send(Args &&... args)
            {
                msg message = msg{...args};
                send(&message);
            }

            template <typename msg>
            void send(msg* message)
            {
                if(_currentInterceptorType != TRANSLATOR)
                    return; //none of the other interceptors should send a message. Throw an exception here maybe

                _sentMessage = true;
                _interceptorBuffer->send<msg>(message);
            }


            //simple event translation
            template <typename in, typename out>
            void translate(std::function<void(out*)> callback)
            {
                _interceptorBuffer->subscribeTranslator<in>([this, callback](in* inMsg){
                    _currentInterceptorType = TRANSLATOR;

                    out outMsg;
                    callback(&outMsg);
                    send<out>(outMsg);
                    
                    return false; //dump "in" message
                });
            }

            //direct translation
            template <typename in, typename out>
            void translate(std::function<void(in*, out*)> callback)
            {
                _interceptorBuffer->subscribeTranslator<in>([this, callback](in* inMsg)
                {
                    _currentInterceptorType = TRANSLATOR;

                    out outMsg;
                    callback(inMsg, &outMsg);
                    send<out>(outMsg);

                    return false; //dump "in" message
                });
            }

            //conditional translation
            template <typename in, typename out>
            void translate(std::function<bool(in*, out*)> callback)
            {
                _interceptorBuffer->subscribeTranslator<in>([this, callback](in* inMsg)
                {
                    _currentInterceptorType = TRANSLATOR;

                    out outMsg;
                    if(callback(inMsg, &outMsg))
                    {
                        send<out>(outMsg);
                        return false; //dump "in message" if a new message was sent
                    }
                    
                    return true; //pass "in message" onward if out wasn't sent
                });
            }

            //one to many translation
            template<typename in, typename classType>
            void translate(bool(classType::*callback)(in*))
            {
                _interceptorBuffer->subscribeTranslator<in>([this, callback](in* msg)
                { 
                    _currentInterceptorType = TRANSLATOR;
                    return callback(msg); 
                });
            }

            template <typename in, typename classType>
            void translate(void (classType::*callback)(in *))
            {
                _interceptorBuffer->subscribeTranslator<in>([this, callback](in *msg) {
                    _currentInterceptorType = TRANSLATOR;
                    callback(msg);
                    
                    return false;
                });
            }

            //Captures an incoming message and edits its data fields
            template<typename msg>
            void edit(std::function<void(msg*)> callback) { _interceptorBuffer->subscribeEditor<msg>(callback); }

            template<typename Msg, typename classType>
            void edit(void(classType::*callback)(Msg*))
            {
                _interceptorBuffer->subscribeEditor<Msg>([this, callback](Msg* msg)
                { 
                    _currentInterceptorType = EDITOR;
                    ((classType)this)->callback(msg);
                    
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
                _interceptorBuffer->subscribeFilter<Msg>([this, callback](Msg* msg)
                { 
                    _currentInterceptorType = FILTER;
                    return ((classType)this)->callback(msg);
                });
            }

            template<typename msg>
            void filter(bool drop)
            { 
                _interceptorBuffer->subscribeFilter<msg>([this](msg* message)
                {
                    _currentInterceptorType = FILTER;
                    return false;
                }); 
            }

            template <typename msg>
            void filter(bool* active)
            {
                _interceptorBuffer->subscribeFilter<msg>([this, active](msg *message) {
                    _currentInterceptorType = FILTER;
                    return !*active;
                });
            }

            template <typename msg, typename InterceptorType>
            void filter(bool InterceptorType::* active)
            {
                _interceptorBuffer->subscribeFilter<msg>([this, active](msg *message) {
                    _currentInterceptorType = FILTER;
                    return !*active;
                });
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
            
            /*template <typename msg>
            void add(std::function<void(msg *message)> callback) { _interceptorBuffer.subscribe<msg>(callback); }
            
            template<typename msg> 
            void add(std::function<bool(msg* message)> callback) { _interceptorBuffer.subscribe<msg>(callback); }
            */

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