#ifndef IO__H
#define IO__H

#include <Windows.h>
#include <stdint.h>
#include "msg.h"
#include "ByteSerializer.h"
class Sender
{
private:
    HANDLE m_handle;
    ByteSerializer bs;
    bool sendInner(vector<uint8_t>& message);
public:
    Sender(HANDLE handle) : m_handle(handle) {};
    void changeHandle(HANDLE newHandle);
    bool sendTo(COMMANDLIST com);
    bool sendTo(COMMANDLIST com, vector<string> arr);//sending string messages
    bool sendTo(COMMANDLIST com, vector<uint64_t> arr);//sending numerical messages
    bool sendTo(COMMANDLIST com, vector<uint64_t> nArr, vector<string> sArr);
    bool sendTo(COMMANDLIST com,uint64_t value);
    bool sendTo(COMMANDLIST com,string str);
    bool sendTo(COMMANDLIST com,uint64_t value, string str);
    bool getFrom(msg& message);//receiving messages
};


#endif // IO__H
