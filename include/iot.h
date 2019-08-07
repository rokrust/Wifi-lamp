#pragma once

#include <vector>
#include <string>
#include <map>
#include <queue>
#include <functional>
#include <utility>


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
            void unsubscribe(unsigned int id);

            template<typename CallbackFunction>
            void subscribe(const unsigned int id, CallbackFunction callback);

            template<typename CallbackFunction>
            void subscribe(Message *message, CallbackFunction callback);
    };




    typedef pair<NetworkModule *, function<void(Message*)>> CallbackPair_t;
    typedef std::map<unsigned int, vector<CallbackPair_t>> SubscriptionMap_t; //messy but effective

    class IotDevice
    {
        friend class NetworkModule;

        private:
            //Message id's are used as keys, while the values contain callback functions and the accompanying NetworkModule instance
            static SubscriptionMap_t _subscriptionMap;

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


    /*class TestMessage : public Message
    {
        public:
            static const unsigned int id = 1;
            TestMessage() { }
    };

    class TestModule : NetworkModule
    {
        private:
        public:
            void setup() {}
            void loop() { subscribe(TestMessage::id, [](){}); }//empty callback
    }test;*/
}