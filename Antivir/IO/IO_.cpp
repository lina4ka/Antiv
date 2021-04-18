#include "IO_.h"

bool Sender::sendTo(COMMANDLIST com, vector<uint64_t> arr)
{
    msg message;
    message.numericArray = arr;
    message.command = com;
    uint64_t msgSize = 0;
    auto msg = bs.toByte(message, msgSize);
    return sendInner(msg);
}

bool Sender::sendTo(COMMANDLIST com, vector<uint64_t> nArr, vector<string> sArr)
{
    msg message;
    message.stringArray = sArr;
    message.numericArray = nArr;
    message.command = com;
    uint64_t msgSize = 0;
    auto msg = bs.toByte(message, msgSize);
    return sendInner(msg);
}

bool Sender::sendTo(COMMANDLIST com, uint64_t value)
{
    msg message;
    message.numericArray.emplace_back(value);
    message.command = com;
    uint64_t msgSize = 0;
    auto msg = bs.toByte(message, msgSize);
    return sendInner(msg);
}

bool Sender::sendTo(COMMANDLIST com, string str)
{
    msg message;
    message.stringArray.emplace_back(str);
    message.command = com;
    uint64_t msgSize = 0;
    auto msg = bs.toByte(message, msgSize);
    return sendInner(msg);
}

bool Sender::sendTo(COMMANDLIST com, uint64_t value, string str)
{
    msg message;
    message.numericArray.emplace_back(value);
    message.stringArray.emplace_back(str);
    message.command = com;
    uint64_t msgSize = 0;
    auto msg = bs.toByte(message, msgSize);
    return sendInner(msg);
}

bool Sender::getFrom(msg& message)
{
    if (m_handle == INVALID_HANDLE_VALUE)
        return false;
    vector<uint8_t> msg;
    msg.resize(4096);
    if (!ReadFile(m_handle, msg.data(), 4096, NULL, 0))
        return false;
    if (bs.getType(msg) == ByteSerializer::ByteClass::MESSAGE)
    {
        message = bs.fromByteMSG(msg);
        return true;
    }
    return false;
}


bool Sender::sendInner(vector<uint8_t>& message)
{
    if (m_handle == NULL || m_handle == INVALID_HANDLE_VALUE)
        return false;
    if (!WriteFile(m_handle, message.data(), 4096, NULL, 0))
        return false;
    return true;
}

void Sender::changeHandle(HANDLE newHandle)
{
    this->m_handle = newHandle;
}

bool Sender::sendTo(COMMANDLIST com)
{
    msg message;
    message.command = com;
    uint64_t msgSize = 0;
    auto msg = bs.toByte(message, msgSize);
    return sendInner(msg);
}

bool Sender::sendTo(COMMANDLIST com, vector<string> arr)
{
    msg message;
    message.stringArray = arr;
    message.command = com;
    uint64_t msgSize = 0;
    auto msg = bs.toByte(message, msgSize);
    return sendInner(msg);
}
