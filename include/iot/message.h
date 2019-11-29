#pragma once
#include<vector>
typedef unsigned char Byte;

#define READTYPE(type)           \
    Byte p[sizeof(type)];        \
    readStream(p, sizeof(type)); \
    return *((type *)p);

struct iot::Message;

class Stream
{

    private:
        vector<Byte> _stream;
        int _byte;
        bool _block;

        void writeStream(Byte *var, int size)
        {
            while(_block);
            _block = true;

            //Resize if necessary
            if(_stream.size() < size) _stream.resize(size);

            for (int i = 0; i < size; i++)
                _stream[_byte++] = var[i];

            _block = false;
        }

        void readStream(Byte *p, int size)
        {
            while(_block);
            _block = true;
            
            for (int i = 0; i < size; i++)
                p[i] = _stream[_byte++];
            
            _block = false;
        }

    public:
        Stream()
        {
            _byte = 0;
            _block = false;
        }


        template <typename T>
        void write(T var, int size = sizeof(T)) { writeStream((Byte *)&var, size); }
        void writeMessage(iot::Message *msg)
        {
            msg->serialize(this);
        }

        int32_t readInt32() { READTYPE(int32_t); }
        int16_t readInt16() { READTYPE(int16_t); }
        int8_t readInt8() { READTYPE(int8_t); }
        char readChar() { READTYPE(char); }
        void readString(char *str)
        {
            for (int i = 0; _stream[_byte] != '\0'; i++)
                str[i] = _stream[i + _byte++];
        }
        void readMessage(iot::Message* message)
        {
            message->deserialize(this);
        }

        void copy(Byte* stream, int size)
        {
            //Resize if necessary
            if(_stream.size() < size) _stream.resize(size);

            for(int i = 0; i < size; i++)
                _stream[i] = stream[i];
        }
};

namespace iot
{
    namespace Messages { static unsigned int messageCount = 0; }
    
    template <typename msg>
    struct MessageId { static const unsigned int id; };
    
    template <typename msg>
    const unsigned int MessageId<msg>::id = Messages::messageCount++;

    struct Message
    {
        virtual void serialize(Stream* stream) {}
        virtual void deserialize(Stream* stream) {}
    };

}


