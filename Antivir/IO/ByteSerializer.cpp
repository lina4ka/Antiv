#include "ByteSerializer.hpp"

std::vector<uint8_t> ByteSerializer::toByte(msg& m, uint64_t& msgSize)
{

	/*calculate the size of the message*/
	ByteClass byteClass = ByteClass::MESSAGE;
	std::vector<uint8_t> byteMSG; uint64_t offset = 0;
	uint64_t arraySize = m.numericArray.size();
	msgSize = arraySize * sizeof(uint64_t) + sizeof(m.command) + sizeof(ByteClass) + sizeof(uint64_t) + sizeof(uint64_t) + sizeof(uint64_t) + m.stringArray.size() * sizeof(uint64_t);
	for (auto& el : m.stringArray)
		msgSize += el.size() + 1;
	byteMSG.resize(msgSize);
		/*
		*sequentially write
		* message type, command, message size, numeric array size, array itself, string array sizeand size of its elements with the elements themselves
		*/
	std::memcpy(&byteMSG[offset], &byteClass, sizeof(m.command));
	offset += sizeof(ByteClass::MESSAGE);
	std::memcpy(&byteMSG[offset], &m.command, sizeof(m.command));
	offset += sizeof(m.command);
	std::memcpy(&byteMSG[offset], &msgSize, sizeof(msgSize));
	offset += sizeof(msgSize);
	std::memcpy(&byteMSG[offset], &arraySize, sizeof(arraySize));
	offset += sizeof(arraySize);
	std::memcpy(&byteMSG[offset], m.numericArray.data(), arraySize * sizeof(uint64_t));
	offset += arraySize * sizeof(uint64_t);
	arraySize = m.stringArray.size();
	std::memcpy(&byteMSG[offset], &arraySize, sizeof(arraySize));
	offset += sizeof(arraySize);
	for (auto& el : m.stringArray)
	{
		arraySize = el.size() + 1;
		std::memcpy(&byteMSG[offset], &arraySize, sizeof(arraySize));
		offset += sizeof(arraySize);
		std::memcpy(&byteMSG[offset], el.data(), arraySize);
		offset += arraySize;
	}
	return byteMSG;
}

msg ByteSerializer::fromByteMSG(std::vector<uint8_t>& byte)
{
	// read in order from toByte
	uint64_t msgSize, arraySize, offset;
	offset = sizeof(ByteClass);
	msg m;
	std::memcpy(&m.command, &byte[offset], sizeof(m.command));
	offset += sizeof(m.command);
	std::memcpy(&msgSize, &byte[offset], sizeof(msgSize));
	offset += sizeof(msgSize);
	std::memcpy(&arraySize, &byte[offset], sizeof(arraySize));
	offset += sizeof(arraySize);
	m.numericArray.resize(arraySize);
	std::memcpy(m.numericArray.data(), &byte[offset], m.numericArray.size() * sizeof(uint64_t));
	offset += m.numericArray.size() * sizeof(uint64_t);
	std::memcpy(&arraySize, &byte[offset], sizeof(arraySize));
	offset += sizeof(arraySize);
	m.stringArray.resize(arraySize);
	for (auto& el : m.stringArray)
	{
		std::memcpy(&arraySize, &byte[offset], sizeof(arraySize));
		offset += sizeof(arraySize);
		el.resize(arraySize);
		std::memcpy(&el[0], &byte[offset], arraySize);
		offset += arraySize;
	}
	return m;
}

ByteSerializer::ByteClass ByteSerializer::getType(std::vector<uint8_t>& byte)
{
	//package type check
	if (byte.size() < sizeof(ByteClass))
		return ByteClass::UNKNOWN;
	ByteClass bc;
	std::memcpy(&bc, byte.data(), sizeof(bc));
	return bc;
}
