#include <vector>
#include <string>
#include <map>
#include <queue>


namespace iot 
{
    using namespace std;
    class Iot;
    class NetworkModule;

    class Message
    {
        public:
            static const unsigned int id = 0;
            virtual unsigned int getId() { return Message::id; }
    };

    class NetworkModule
    {
        private:
            friend void IotDevice::addModule(NetworkModule* module);
        
        public:
            void setDevice(IotDevice* device);
            virtual void setup() = 0;
            virtual void loop() = 0;

            void unsubscribe(unsigned int id);
            void subscribe(unsigned int id) { IotDevice::_subscriptionMap[id] = this; }
            void subscribe(const Message& message) { subscribe(message.getId()); }
        //    void send(Message message) { _device->broadCastMessage(message); }
            void receive(const Message& message);
    };

    class IotDevice
    {
        friend void NetworkModule::receive(const Message& message);
        friend void NetworkModule::unsubscribe(unsigned int id);
        friend void NetworkModule::subscribe(unsigned int id);

        private:
            static map<unsigned int, vector<NetworkModule*>> _subscriptionMap;
            static queue<Message> _messageQueue;
            vector<NetworkModule*> _modules;

        public:
            void setup()
            {
                for (int i = 0; i < _modules.size(); i++)
                {
                    _modules[i]->setup();
                }
            }

            void loop()
            {
                while(!_messageQueue.empty())
                {
                    broadCastMessage(_messageQueue.front());
                    _messageQueue.pop();
                }

                for (int i = 0; i < _modules.size(); i++)
                {
                    _modules[i]->loop();
                }
            }

            void addModule(NetworkModule* module)
            {
                _modules.push_back(module);
            }


            void broadCastMessage(const Message& message)
            {
                for(int i = 0; i < _subscriptionMap[message.id].size(); i++)
                {
                    _subscriptionMap[message.id][i]->receive(message);
                }
            }

    };


    class TestMessage : public Message
    {
        public:
            static const unsigned int id = 1;
            TestMessage() { }
    };
    //NewMessage() : Message(id) {}

    class TestModule : NetworkModule
    {
        private:
        public:
            void setup() {}
            void loop() { subscribe(TestMessage::id); }
    }test;
    
    map<unsigned int, vector<NetworkModule*>> IotDevice::_subscriptionMap;
    queue<Message> IotDevice::_messageQueue;
}