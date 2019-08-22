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
            virtual NetworkModule* clone() = 0;
            virtual ~NetworkModule() {}

            template <class Callback> void subscribe(Callback callback) { IotDevice::_broadCaster.subscribe(callback); }
            template <class Msg> void send(Msg *message) { IotDevice::_broadCaster.send(message); };
            void receive(Message* message) {Serial.println("Module has not subscribed to message " + message->getId());}
    };

    template<class ...Msg_t>
    class IotDevice
    {
        friend class NetworkModule;

        private:
            //Messages are sent to this queue and broadcast to all subscribed modules
            static queue<Message*> _messageQueue;
            static BroadCaster<Msg_t...> _broadCaster;
            vector<NetworkModule*> _modules;

        public:
            void setup();
            void loop();
            void addModule(NetworkModule* module);
            void removeModule(NetworkModule* module);
            void broadCastMessage(Message* message);

            ~IotDevice();
    };

    template<typename ...Msg_t> 
    BroadCaster<Msg_t...> IotDevice<Msg_t...>::_broadCaster;

    template<typename ...Msg_t>
    queue<Message *> IotDevice<Msg_t...>::_messageQueue;
}