#ifndef RECORDINFO_H
#define RECORDINFO_H
#include <stdint.h>
#include <string>
#include "sha256.h"
using namespace std;
struct RecordInfo
{
    uint64_t prefix,
    signatureSize,
    offsetBegin,
    offsetEnd;
    string SHA,malvareName;
    RecordInfo(string name,string signature,uint64_t sOffset,uint64_t eOffset,string sha256);
    RecordInfo();
};
class RecordFabric
{
private:
    SHA256 SHA;
public:
    RecordInfo create(string name,string signature, uint64_t sOffset, uint64_t eOffset);
};
#endif // RECORDINFO_H
