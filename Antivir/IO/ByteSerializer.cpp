#include "ByteSerializer.h"

vector<uint8_t> ByteSerializer::toByte(msg& m, uint64_t& msgSize)
{

    /*calculate the size of the message*/
    ByteClass byteClass = ByteClass::MESSAGE;
    vector<uint8_t> byteMSG; uint64_t offset = 0;
    uint64_t arraySize = m.numericArray.size();
    msgSize = arraySize * sizeof(uint64_t) + sizeof(m.command) + sizeof(ByteClass) + sizeof(uint64_t) + sizeof(uint64_t) + sizeof(uint64_t) + m.stringArray.size() * sizeof(uint64_t);
    for (auto& el : m.stringArray)
        msgSize += el.size();
    byteMSG.resize(msgSize);
        /*
        *sequentially write
        * message type, command, message size, numeric array size, array itself, string array sizeand size of its elements with the elements themselves
        */
    memcpy(&byteMSG[offset], &byteClass, sizeof(m.command));
    offset += sizeof(ByteClass::MESSAGE);
    memcpy(&byteMSG[offset], &m.command, sizeof(m.command));
    offset += sizeof(m.command);
    memcpy(&byteMSG[offset], &msgSize, sizeof(msgSize));
    offset += sizeof(msgSize);
    memcpy(&byteMSG[offset], &arraySize, sizeof(arraySize));
    offset += sizeof(arraySize);
    memcpy(&byteMSG[offset], m.numericArray.data(), arraySize * sizeof(uint64_t));
    offset += arraySize * sizeof(uint64_t);
    arraySize = m.stringArray.size();
    memcpy(&byteMSG[offset], &arraySize, sizeof(arraySize));
    offset += sizeof(arraySize);
    for (auto& el : m.stringArray)
    {
        arraySize = el.size();
        std::memcpy(&byteMSG[offset], &arraySize, sizeof(arraySize));
        offset += sizeof(arraySize);
        std::memcpy(&byteMSG[offset], el.data(), arraySize);
        offset += arraySize;
    }
    return byteMSG;
}

msg ByteSerializer::fromByteMSG(vector<uint8_t>& byte)
{
    // read in order from toByte
    uint64_t msgSize, arraySize, offset;
    offset = sizeof(ByteClass);
    msg m;
    memcpy(&m.command, &byte[offset], sizeof(m.command));
    offset += sizeof(m.command);
    memcpy(&msgSize, &byte[offset], sizeof(msgSize));
    offset += sizeof(msgSize);
    memcpy(&arraySize, &byte[offset], sizeof(arraySize));
    offset += sizeof(arraySize);
    m.numericArray.resize(arraySize);
    memcpy(m.numericArray.data(), &byte[offset], m.numericArray.size() * sizeof(uint64_t));
    offset += m.numericArray.size() * sizeof(uint64_t);
    memcpy(&arraySize, &byte[offset], sizeof(arraySize));
    offset += sizeof(arraySize);
    m.stringArray.resize(arraySize);
    for (auto& el : m.stringArray)
    {
        memcpy(&arraySize, &byte[offset], sizeof(arraySize));
        offset += sizeof(arraySize);
        el.resize(arraySize);
        memcpy(&el[0], &byte[offset], arraySize);
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
