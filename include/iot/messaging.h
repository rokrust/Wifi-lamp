#pragma once 

#include "iot/message.h"

#include <map>
#include <vector>
#include <functional>
#include <iterator>

namespace iot 
{

    class Buffer
    {
        using CallbackFunction = std::function<bool(Message *)>;

        private:
            std::map<unsigned int, std::vector<CallbackFunction>> _messages;

        public:
            template <typename Msg>
            bool send(Message *message)
            {
                std::vector<CallbackFunction> callbacks = _messages[MessageId<Msg>::id];
                for (int i = 0; i < callbacks.size(); i++)
                {
                    if (!callbacks[i](message))
                        return false;
                }

                return true;
            }

            template <typename Msg>
            void subscribe(std::function<bool(Msg *)> callback)
            {
                _messages[MessageId<Msg>::id].push_back([callback](Message *message) { return callback(static_cast<Msg *>(message)); });
            }

            template <typename Msg>
            void subscribe(std::function<void(Msg *)> callback, bool drop)
            {
                subscribe<Msg>([callback, drop](Msg *message) {
                    callback(message);
                    return !drop;
                });
            }

            template <typename... Msg>
            void subscribe(std::function<bool(Msg *)> &&... msg)
            {
                //Hacky pre-c++17 solution for passing variadic arguments to a function
                using expand_type = int[];
                expand_type{0, (subscribe<Msg>(msg), 0)...};
            }
    };

    class ModuleBuffer
    {
        
        private:
            using SerializeFunction = std::function<void(Serializer*)>;
            
            std::vector<int> _sources;
            std::map<int, SerializeFunction> _messageSerializeMap;

            Buffer _messageBuffer;
            Serializer _serializer;

            template<typename msg>
            bool broadcast(msg* message)
            {
                return true;
            }

            template<typename msg>
            void setDeserializeIfNecessary()
            {
                std::map<int, SerializeFunction>::iterator it = _messageSerializeMap.find(MessageId<msg>::id);
                if (it == _messageSerializeMap.end())
                {
                    //subscribe serializer
                }

                //call subscribed serializer
                _messageSerializeMap[MessageId<msg>::id] = [this](Serializer *serializer) {
                    msg *message;
                    message->deserialize(serializer);
                    
                    send<msg>(message); //must be sent to interceptor first
                };
            }

        public:
            template<typename msg>
            bool send(msg* message) 
            {
                if(!_messageBuffer.send<msg>(message)) return false;
                
                message->serialize(&_serializer);
                if(_serializer.dirty())
                {
                    broadcast(message);
                    _serializer.clearDirty();
                }

                return true;
            }

            template<typename msg>
            void subscribe(std::function<void(msg *)> callback)
            {
                
                setDeserializeIfNecessary<msg>();
                _messageBuffer.subscribe<msg>(callback, false);
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
            template <typename msg, typename... Args>
            bool send(Args &&... args)
            {
                msg message = {args...};
                return send(&message);
            }

            template <typename msg>
            bool send(msg *message)
            {
                bool passMessageOn = filter.send<msg>(message) &&
                                    translation.send<msg>(message) && !_dropTranslatedMessage &&
                                    edit.send<msg>(message);

                _dropTranslatedMessage = false;
                return passMessageOn;
            }

            template <typename msg>
            void subscribeFilter(std::function<bool(msg *)> callback)
            {
                filter.subscribe<msg>([this, callback](msg *message) {
                    _currentInterceptorType = FILTER;
                    return !callback(message);
                });
            }

            template <typename Msg>
            void subscribeTranslator(std::function<bool(Msg *)> callback)
            {
                translation.subscribe<Msg>([this, callback](Msg *message) {
                    _currentInterceptorType = TRANSLATOR;
                    _dropTranslatedMessage = callback(message);
                }, false);
            }

            template <typename msg>
            void subscribeEditor(std::function<void(msg *)> callback)
            {
                edit.subscribe<msg>([this, callback](msg *message) {
                    _currentInterceptorType = EDITOR;
                    callback(message);

                    return true;
                });
            }
    };
}