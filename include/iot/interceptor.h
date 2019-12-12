#pragma once
#include "iot/messagebuffers.h"
#include <Arduino.h>

namespace iot
{
    class Interceptor
    {
        private:
            ModuleBuffer *_moduleBuffer;
            InterceptorBuffer *_interceptorBuffer;

            bool _sentMessage = false;

        public:
            virtual void setup() = 0;

            void setInterceptorBuffer(InterceptorBuffer *buffer) { _interceptorBuffer = buffer; }
            void setModuleBuffer(ModuleBuffer *buffer) { _moduleBuffer = buffer; }

            template <typename msg, typename... Args>
            void send(Args &&... args)
            {
                msg message = msg{args...};
                send(&message);
            }

            template <typename msg>
            void send(msg *message)
            {
                _sentMessage = true;
                _interceptorBuffer->send<msg>(message) && _moduleBuffer->send<msg>(message);
            }

            //event translation
            template <typename in, typename out>
            void translate(std::function<void(out *)> callback)
            {
                _interceptorBuffer->subscribeTranslator<in>([this, callback](in *inMsg) {
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
                _interceptorBuffer->subscribeTranslator<in>([this, callback](in *inMsg) {
                    Serial.println("Direct");
                    out outMsg;
                    callback(inMsg, &outMsg);
                    send<out>(outMsg);

                    return false; //dump "in" message
                });
            }

            //direct translation
            template <typename in, typename out, typename InterceptorType>
            void translate(void (InterceptorType::*callback)(in *, out *))
            {
                translate<in, out>([this, callback](in *inMsg, out *outMsg) {
                    Serial.println("Direct");
                    (((InterceptorType *)this)->*callback)(inMsg, outMsg);
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
            template <typename in, typename classType>
            void translate(void (classType::*callback)(in *), bool drop = false)
            {
                _interceptorBuffer->subscribeTranslator<in>([this, callback, drop](in *msg) {
                    Serial.println("One to many");
                    (((classType *)this)->*callback)(msg);
                    return drop;
                });
            }

            //Captures an incoming message and edits its data fields
            template <typename msg>
            void edit(std::function<void(msg *)> callback) { _interceptorBuffer->subscribeEditor<msg>(callback, false); }

            template <typename Msg, typename classType>
            void edit(void (classType::*callback)(Msg *))
            {
                _interceptorBuffer->subscribeEditor<Msg>([this, callback](Msg *msg) {
                    (((classType *)this)->*callback)(msg);
                    return true;
                });
            }

            template <typename msg>
            void filter(std::function<bool(msg *)> filterFunction)
            {
                _interceptorBuffer->subscribeFilter(filterFunction);
            }

            template <typename Msg, typename classType>
            void filter(bool (classType::*callback)(Msg *))
            {
                _interceptorBuffer->subscribeFilter<Msg>([this, callback](Msg *msg) { return (((classType *)this)->*callback)(msg); });
            }

            template <typename msg>
            void filter(bool drop)
            {
                _interceptorBuffer->subscribeFilter<msg>([drop](msg *message) { return drop; });
            }

            template <typename msg>
            void filter(bool *active)
            {
                _interceptorBuffer->subscribeFilter<msg>([active](msg *message) { return *active; });
            }

            template <typename msg, typename InterceptorType>
            void filter(bool InterceptorType::*active)
            {
                _interceptorBuffer->subscribeFilter<msg>([this, active](msg *message) { return *active; });
            }
    };
}