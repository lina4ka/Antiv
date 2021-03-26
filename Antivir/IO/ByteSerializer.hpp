#pragma once
#include "msg.hpp"
#include <vector>
#include <stdint.h>
class ByteSerializer
{
public:
	enum class ByteClass : uint8_t { UNKNOWN, MESSAGE, RECORD	} ;//types for decoding
	std::vector<uint8_t> toByte(msg& m,uint64_t& msgSize);//convert message to byte array
	msg fromByteMSG(std::vector<uint8_t>& byte);//convert byte array to message
	ByteClass getType(std::vector<uint8_t>& byte);//check the package type
};

