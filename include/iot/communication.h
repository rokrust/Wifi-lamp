#pragma once

#include "iot/serializing.h"
#include "iot/message.h"

#include <vector>

class Communicator
{
    protected:
        Serializer stream;
    
    public:
        virtual void setup() { }
};

class Receiver : public Communicator
{

};

class Transmitter : public Communicator
{
    private:
        std::vector<int> _targets;
    
    public:
        void sendMessage(iot::Message* msg)
        {
            msg->serialize(&stream);
            write(stream.getArray(), stream.getSize());
        }
        
        //Only used to send serialized messages. Should not write to any specific registers
        virtual void write(Byte* stream, unsigned int size) = 0;
};