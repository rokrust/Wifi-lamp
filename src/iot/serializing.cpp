#include "iot/message.h"

#include <cstdint>
#include <vector>

void Serializer::writeStream(Byte *var, unsigned int size)
{
    while (_block) ;
    _block = true;

    //Resize if necessary
    if (_stream.size() < size)
        _stream.resize(size);

    for (int i = 0; i < size; i++)
        _stream[_byte++] = var[i];

    _block = false;
    _dirty = true;
    _size += size;
}

void Serializer::readStream(Byte *p, unsigned int size)
{
    while (_block) ;
    _block = true;

    for (int i = 0; i < size; i++)
        p[i] = _stream[_byte++];

    _block = false;
}

Serializer::Serializer()
{
    _byte = 0;
    _size = 0;
    _block = false;
    _dirty = false;
}

void Serializer::writeMessage(iot::Message *msg)
{
    msg->serialize(this);
}

void Serializer::readString(char *str)
{
    for (int i = 0; _stream[_byte] != '\0'; i++)
        str[i] = _stream[i + _byte++];
}

void Serializer::readMessage(iot::Message *message)
{
    message->deserialize(this);
}

void Serializer::copy(Byte *stream, unsigned int size)
{
    //Resize if necessary
    if (_stream.size() < size)
        _stream.resize(size);

    for (int i = 0; i < size; i++)
        _stream[i] = stream[i];
}