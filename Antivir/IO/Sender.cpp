#include "Sender.hpp"

bool Sender::sendTo(COMMANDLIST com, std::vector<uint64_t> arr)
{
    msg message;
    message.numericArray = arr;
    message.command = com;
    uint64_t msgSize = 0;
    auto msg = bs.toByte(message, msgSize);
    return sendInner(msg);    
}

bool Sender::getFrom(msg& message)
{
    if (m_handle == INVALID_HANDLE_VALUE)
        return false;
    std::vector<uint8_t> msg;
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


bool Sender::sendInner(std::vector<uint8_t>& message)
{
    if (!WriteFile(m_handle, message.data(), 4096, NULL, 0))
        return false;
    return true;
}

bool Sender::sendTo(COMMANDLIST com, std::vector<std::string> arr)
{
    msg message;
    message.stringArray = arr;
    message.command = com;
    uint64_t msgSize = 0;
    auto msg = bs.toByte(message, msgSize);
    return sendInner(msg);
}
