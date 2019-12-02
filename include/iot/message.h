#pragma once
#include "iot/serializing.h"

#include <vector>
#include <cstdint>

namespace iot
{
    namespace Messages { static unsigned int messageCount = 0; }
    
    template <typename msg>
    struct MessageId { static const unsigned int id; };
    
    template <typename msg>
    const unsigned int MessageId<msg>::id = Messages::messageCount++;

    struct Message
    {
        virtual void serialize(Serializer* stream) {}
        virtual void deserialize(Serializer* stream) {}
    };

}