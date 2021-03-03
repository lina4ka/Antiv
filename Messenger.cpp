#include "Messenger.h"

void msg::addValue(uint64_t value)
{
	numericArray.push_back(value);
}

void msg::addString(std::string str)
{
	stringArray.push_back(str);
}

void msg::clear(int index)
{
	if (index == 0)
		numericArray.clear();
	else if (index == 1)
		stringArray.clear();
}

std::vector<uint64_t> & msg::getNumeric()
{
	return numericArray;
}

std::vector<std::string> & msg::getString()
{
	return stringArray;
}

void Messenger::setChanelSize(uint64_t size)
{
	chanelSize = size;
}

void Messenger::sendMessage(HANDLE chanel, msg& message, COMMANDLIST command)
{
	std::string msg = toByte(message, command);
	uint64_t msgSize = getMSGsize(msg);
	uint64_t sendTimes = (msgSize + msgSize % chanelSize) / chanelSize;
	uint64_t offset = 0;
	for (uint64_t i = 0; i <= sendTimes; ++i)
	{
		WriteFile(chanel, &msg[offset], chanelSize, NULL, 0);
		offset += chanelSize;
	}
	
}

Messenger::COMMANDLIST Messenger::readMessage(HANDLE chanel, msg& message)
{
	std::string msg;
	COMMANDLIST command;
	uint64_t msgSize = sizeof(COMMANDLIST) + sizeof(uint64_t), offset = 0;
	uint64_t sendTimes = (msgSize + msgSize % chanelSize) / chanelSize;
	msg.resize(msgSize + msgSize % chanelSize);
	for (uint64_t i = 0; i < sendTimes; ++i)
	{
		ReadFile(chanel, &msg[offset], chanelSize, NULL, 0);
		offset += chanelSize;
	}
	msgSize = getMSGsize(msg);
	sendTimes = (msgSize + msgSize % chanelSize) / chanelSize - sendTimes;
	msg.resize(msgSize + msgSize % chanelSize);
	for (uint64_t i = 0; i <= sendTimes; ++i)
	{
		ReadFile(chanel, &msg[offset], chanelSize, NULL, 0);
		offset += chanelSize;
	}
	message = fromByte(msg, command);
	return command;
}

std::string Messenger::toByte(msg& message, COMMANDLIST command)
{
	std::string byteMSG; uint64_t offset = 0;
	uint64_t arraySize = message.getNumeric().size();
	uint64_t msgSize = message.getNumeric().size() * sizeof(uint64_t) + sizeof(COMMANDLIST)+sizeof(uint64_t) + sizeof(uint64_t) + sizeof(uint64_t) + message.getString().size() * sizeof(uint64_t);
	for (auto& el : message.getString())
		msgSize += el.size() + 1;
	byteMSG.resize(msgSize);
	std::memcpy(&byteMSG[offset], &command, sizeof(command));
	offset += sizeof(command);
	std::memcpy(&byteMSG[offset], &msgSize, sizeof(msgSize));
	offset += sizeof(msgSize);
	std::memcpy(&byteMSG[offset], &arraySize, sizeof(arraySize));
	offset += sizeof(arraySize);
	std::memcpy(&byteMSG[offset], message.getNumeric().data(), arraySize*sizeof(uint64_t));
	offset += arraySize * sizeof(uint64_t);
	arraySize = message.getString().size();
	std::memcpy(&byteMSG[offset], &arraySize, sizeof(arraySize));
	offset += sizeof(arraySize);
	for (auto& el : message.getString())
	{
		arraySize = el.size()+1;
		std::memcpy(&byteMSG[offset], &arraySize, sizeof(arraySize));
		offset += sizeof(arraySize);
		std::memcpy(&byteMSG[offset], el.data(), arraySize);
		offset += arraySize;
	}
	return byteMSG;
}

msg Messenger::fromByte(std::string str, COMMANDLIST& command)
{
	uint64_t msgSize, arraySize,offset = 0;
	msg newMessage;
	std::memcpy(&command, &str[offset], sizeof(command));
	offset += sizeof(command);
	std::memcpy(&msgSize, &str[offset], sizeof(msgSize));
	offset += sizeof(msgSize);
	std::memcpy(&arraySize, &str[offset], sizeof(arraySize));
	offset += sizeof(arraySize);
	newMessage.getNumeric().resize(arraySize);
	std::memcpy(newMessage.getNumeric().data(), &str[offset], newMessage.getNumeric().size()*sizeof(uint64_t));
	offset += newMessage.getNumeric().size()*sizeof(uint64_t);
	std::memcpy(&arraySize, &str[offset], sizeof(arraySize));
	offset += sizeof(arraySize);
	newMessage.getString().resize(arraySize);
	for (auto& el : newMessage.getString())
	{
		std::memcpy(&arraySize, &str[offset], sizeof(arraySize));
		offset += sizeof(arraySize);
		el.resize(arraySize);
		std::memcpy(&el[0], &str[offset], arraySize);
		offset += arraySize;
	}
	return newMessage;
}

uint64_t Messenger::getMSGsize(std::string str)
{
	uint64_t tempSize = 0, 	offset = sizeof(COMMANDLIST);
	std::memcpy(&tempSize, &str[offset], sizeof(tempSize));
	return tempSize;
}
