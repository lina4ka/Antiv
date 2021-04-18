#include "Recordinfo.h"


RecordInfo::RecordInfo(string name,string signature, uint64_t sOffset, uint64_t eOffset,string sha256)
{
    this->prefix = 0;
    memcpy(&prefix,signature.data(),sizeof (prefix));
    this->offsetBegin=sOffset;
    this->offsetEnd=eOffset;
    this->SHA = sha256;
    this->signatureSize=signature.length();
    this->malvareName = name;
    SHA.resize(64);
    malvareName.resize(10);
}

RecordInfo::RecordInfo()
{
    SHA.resize(64,'\0');
    malvareName.resize(10, '\0');
    this->offsetBegin = 0;
    this->offsetEnd = 0;
    this->signatureSize = 0;
    this->prefix = 0;
}

RecordInfo RecordFabric::create(string name, string signature, uint64_t sOffset, uint64_t eOffset)
{
    RecordInfo ri(name, signature, sOffset, eOffset, SHA.getSHA(signature));
    return move(ri);
}
