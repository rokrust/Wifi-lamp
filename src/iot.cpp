#include <vector>
#include <string>
#include <map>
#include <queue>
#include <Arduino.h>
#include "iot.h"

namespace iot
{
    using namespace std;
    queue<Message *> IotDevice::_messageQueue;
    const unsigned int Message::id;

    Message::~Message() {}

    void NetworkModule::send(Message* message) { Serial.println("Sending message: " + message->getId()); IotDevice::_messageQueue.push(message); }

    void IotDevice::setup()
    {
        for (int i = 0; i < _modules.size(); i++)
        {
            Serial.println("Setting up module");
            _modules[i]->setup();
        }
    }

    void IotDevice::loop()
    {
        while (!_messageQueue.empty())
        {
            Message* message = _messageQueue.front();
            for(int i = 0; i < _modules.size(); i++)
            {
                _modules[i]->receive(message);
            }

            delete message;
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
        Serial.println("Module added" + _modules.size());
    }

    void IotDevice::removeModule(NetworkModule *module)
    {
        for(int i = 0; i < _modules.size(); i++)
        {
            if(_modules[i] == module)
                _modules.remove(i);
        }
    }

    IotDevice::~IotDevice()
    {
        for(int i = 0; i < _modules.size(); i++)
        {
            delete _modules[i];
        }

        for(int i = 0; i < _messageQueue.size(); i++)
        {
            delete _messageQueue.front();
            _messageQueue.pop();
        }
    }
}