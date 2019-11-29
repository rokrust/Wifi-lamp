#include "iot/message.h"
#include <cstdint>

typedef unsigned char Byte;
#define READTYPE(type) Byte p[sizeof(type)]; readStream(p, sizeof(type)); return *((type*)p);

class Stream
{
    
    private:
        Byte _stream[];
        int _byte;
        
        void writeStream(Byte* var, int size) 
        { 
            for(int i = 0; i < size; i++)
                _stream[_byte++] = var[i];
        }

        void readStream(Byte* p, int size)
        {
            for(int i = 0; i < size; i++)
                p[i] = _stream[_byte++];
        }


    
    public:
        template<typename T>
        void write(T var, int size=sizeof(T)) { writeStream((Byte*) &var, size); }
        void writeMessage(iot::Message* msg)
        {
            msg->deserialize(this);
        }

        int32_t readInt32() { READTYPE(int32_t); }
        int16_t readInt16() { READTYPE(int16_t); }
        int8_t readInt8() { READTYPE(int8_t); }
        char readChar() { READTYPE(char); }
        void readString(char* str) 
        {
            for(int i = 0; _stream[_byte] != '\0'; i++)
                str[i] = _stream[i + _byte++];
        } 
        
};
