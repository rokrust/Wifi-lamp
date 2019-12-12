#include <cstdint>
#include <vector>

typedef unsigned char Byte;
#define READTYPE(type) Byte p[sizeof(type)]; readStream(p, sizeof(type)); return *((type*)p);

namespace iot { struct Message; }

class Serializer
{
    private:
        std::vector<Byte> _stream;
        unsigned int _byte;
        unsigned int _size;
        bool _block;
        bool _dirty;

        void writeStream(Byte *var, unsigned int size);
        void readStream(Byte *p, unsigned int size);

    public:
        Serializer();
        bool dirty() { return _dirty; }
        void clearDirty() { _dirty = false; }
        Byte* getArray() { return _stream.data(); }
        unsigned int getSize() { return _size; }
        void clear() { _size = 0; _byte = 0; }

        template <typename T>
        void write(T var, int size = sizeof(T)) { writeStream((Byte *)&var, size); }
        void writeMessage(iot::Message *msg);

        int32_t readInt32() { READTYPE(int32_t); }
        int16_t readInt16() { READTYPE(int16_t); }
        int8_t readInt8() { READTYPE(int8_t); }
        char readChar() { READTYPE(char); }
        void readString(char *str);
        void readMessage(iot::Message *message);
        void resetPointer() { _byte = 0; }

        void copy(Byte *stream, unsigned int size);
};