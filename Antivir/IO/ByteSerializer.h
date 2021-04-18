#ifndef BYTESERIALIZER_H
#define BYTESERIALIZER_H
#include "msg.h"
#include <vector>
#include <stdint.h>
class ByteSerializer
{
public:
    enum class ByteClass : uint8_t { UNKNOWN, MESSAGE, RECORD	} ;//types for decoding
    vector<uint8_t> toByte(msg& m,uint64_t& msgSize);//convert message to byte array
    msg fromByteMSG(vector<uint8_t>& byte);//convert byte array to message
    ByteClass getType(vector<uint8_t>& byte);//check the package type
};

#endif // BYTESERIALIZER_H
