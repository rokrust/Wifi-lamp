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
    template <class... modules_t>
    void ModulePack<modules_t...>::setup()
    {
        for (int i = 0; i < _modules.size(); i++)
        {
            Serial.println("Setting up module");
            _modules[i]->setup();
        }
    }

    template <typename... modules_t>
    void ModulePack<modules_t...>::loop()
    {
        for (int i = 0; i < _modules.size(); i++)
        {
            _modules[i]->loop();
        }
    }

    template <class... modules_t>
    void ModulePack<modules_t...>::addModule(Module *module)
    {
        _modules.push_back(module);
        Serial.println("Module added" + _modules.size());
    }

    template <class... modules_t>
    void ModulePack<modules_t...>::removeModule(Module *module)
    {
        for(int i = 0; i < _modules.size(); i++)
        {
            if(_modules[i] == module)
                _modules.erase(_modules.begin() + i);
        }
    }

    template <typename modulePack_t, typename broadCaster_t>
    void IotDevice<modulePack_t, broadCaster_t>::loop()
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

        _modules.loop();
        */
    }

    template <typename modulePack_t, typename broadCaster_t>
    IotDevice<modulePack_t, broadCaster_t>::~IotDevice()
    {
        /*
        for(int i = 0; i < _messageQueue.size(); i++)
        {
            delete _messageQueue.front();
            _messageQueue.pop();
        }
        */
    }

    template <typename modulePack_t, typename broadCaster_t>
    IotDevice<modulePack_t, broadCaster_t>::IotDevice(modulePack_t modules, broadCaster_t broadcaster)
    {
        _broadCaster = broadcaster;
        _modules = modules;
    }
}