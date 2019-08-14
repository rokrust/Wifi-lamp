#pragma once

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
        virtual ~Message() = 0;
    };


    class NetworkModule
    {
        private:
        
        public:
            virtual void setup() = 0;
            virtual void loop() = 0;
            virtual ~NetworkModule() {}
            void send(Message* message);
            void receive(Message* message) {Serial.println("Module has not subscribed to message " + message->getId());}
    };


    class IotDevice
    {
        friend class NetworkModule;

        private:
            //Messages are sent to this queue and broadcast to all subscribed modules
            static queue<Message* > _messageQueue;
            vector<NetworkModule*> _modules;

        public:
            void setup();
            void loop();
            void addModule(NetworkModule* module);
            void removeModule(NetworkModule* module);
            void broadCastMessage(Message* message);

            ~IotDevice();
    };
}