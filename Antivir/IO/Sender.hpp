#pragma once
#include <Windows.h>
#include <stdint.h>
#include "msg.hpp"
#include "ByteSerializer.hpp"
class Sender
{
private:
	HANDLE m_handle;
	ByteSerializer bs;
	bool sendInner(std::vector<uint8_t>& message);
public:
	
	Sender(HANDLE handle) : m_handle(handle) {};
	bool sendTo(COMMANDLIST com, std::vector<std::string> arr);//sending string messages
	bool sendTo(COMMANDLIST com, std::vector<uint64_t> arr);//sending numerical messages
	bool getFrom(msg& message);//receiving messages
};

