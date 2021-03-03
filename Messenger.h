#pragma once
#include <vector>
#include <string>
#include <Windows.h>
class msg
{
public:
	void addValue(uint64_t value);
	void addString(std::string str);
	void clear(int index);
	std::vector<uint64_t>  &getNumeric();
	std::vector<std::string> & getString();
private:
	std::vector<uint64_t> numericArray;
	std::vector<std::string> stringArray;
};


class Messenger
{
public:
	void setChanelSize(uint64_t size);
	enum class COMMANDLIST : uint8_t {UNKNOWN,START_SCAN,STOP_SCAN,MOVE_TO_QUARANTINE, DELETE_FILE} ;
	void sendMessage(HANDLE chanel,msg& message, COMMANDLIST command);
	Messenger::COMMANDLIST readMessage(HANDLE chanel, msg& message);
private:
	uint64_t chanelSize = 0;
	std::string toByte(msg& message, COMMANDLIST command);
	msg fromByte(std::string str, COMMANDLIST & command);
	uint64_t getMSGsize(std::string str);
};

