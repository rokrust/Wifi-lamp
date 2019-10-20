#include <vector>
#include <string>
#include <map>
#include <queue>
#include <Arduino.h>
#include <type_traits>

#include "iot.h"

namespace iot
{
    using namespace std;
    const unsigned int Message::id;

    Message::~Message() {}

    /*
    void Module::send(Message* message) 
    {
        Serial.print("Sending message: ");
        Serial.println(message->getId());
        IotDevice::_messageQueue.push(message); 
    }
    */
    void ModulePack::setup()
    {
        for (int i = 0; i < _modules.size(); i++)
        {
            Serial.println("Setting up module");
            _modules[i]->setup();
        }
    }

    void ModulePack::loop()
    {
        for (int i = 0; i < _modules.size(); i++)
        {
            _modules[i]->loop();
        }
    }

    void ModulePack::addModule(Module *module)
    {
        _modules.push_back(module);
        Serial.println("Module added" + _modules.size());
    }

    void ModulePack::removeModule(Module *module)
    {
        for(int i = 0; i < _modules.size(); i++)
        {
            if(_modules[i] == module)
                _modules.erase(_modules.begin() + i);
        }
    }

    template <class... Messages_t>
    void IotDevice<Messages_t...>::loop()
    {
        /*
        while (!_messageQueue.empty())
        {
            Message* message = _messageQueue.front();
            for(int i = 0; i < _modules.size(); i++)
            {
                _broadCaster.send(message);
            }

            delete message;
            _messageQueue.pop();
        }
        */
        _modules.loop();
    }

    template <class... Messages_t>
    IotDevice<Messages_t...>::~IotDevice()
    {
        /*
        for(int i = 0; i < _messageQueue.size(); i++)
        {
            delete _messageQueue.front();
            _messageQueue.pop();
        }
        */
    }

    /*
    template <class... Messages_t>
    IotDevice<Messages_t...>::IotDevice(ModulePack<Messages_t...> modules, BroadCaster<Messages_t...> broadcaster)
    {
        _broadCaster = broadcaster;
        _modules = modules;
    }
    */
}