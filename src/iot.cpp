#include <vector>
#include <string>
#include <map>
#include <queue>
#include "iot.h"

namespace iot
{
    using namespace std;
    SubscriptionMap_t _subscriptionMap;
    queue<Message*> _messageQueue;

    void NetworkModule::send(Message* message) { _messageQueue.push(message); }
    void NetworkModule::subscribe(const unsigned int id, function<void(Message*)> callback)
    {
        if (IotDevice::_subscriptionMap.find(id) == IotDevice::_subscriptionMap.end())
        {
            IotDevice::_subscriptionMap[id] = std::vector<CallbackPair_t>();
        }

        IotDevice::_subscriptionMap[id].push_back(make_pair(this, callback)); 
    }

    void NetworkModule::subscribe(Message *message, function<void(Message*)>& callback)
    { 
        subscribe(message->getId(), callback); 
    }

    void NetworkModule::unsubscribe(unsigned int id) 
    {
        for (int i = 0; i < IotDevice::_subscriptionMap[id].size(); i++)
        {
            if (IotDevice::_subscriptionMap[id][i].first == this)
            {
                IotDevice::_subscriptionMap[id].erase(IotDevice::_subscriptionMap[id].begin() + i);
            }
        }

        if(IotDevice::_subscriptionMap[id].empty())
        {
            IotDevice::_subscriptionMap.erase(id);
        }
    }

    void IotDevice::setup()
    {
        for (int i = 0; i < _modules.size(); i++)
        {
            _modules[i]->setup();
        }
    }

    void IotDevice::loop()
    {
        while (!_messageQueue.empty())
        {
            broadCastMessage(_messageQueue.front());
            delete _messageQueue.front();
            _messageQueue.pop();
        }

        for (int i = 0; i < _modules.size(); i++)
        {
            _modules[i]->loop();
        }
    }

    void IotDevice::addModule(NetworkModule *module)
    {
        _modules.push_back(module);
    }

    void IotDevice::removeModule(NetworkModule *module)
    {

    }

    void IotDevice::broadCastMessage(Message* message)
    {
        for (int i = 0; i < _subscriptionMap[message->id].size(); i++)
        {
            _subscriptionMap[message->id][i].second(message);
        }
    }

    IotDevice::~IotDevice()
    {
        for(int i = 0; i < _modules.size(); i++)
        {
            delete _modules[i];
        }
    }
}