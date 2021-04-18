#ifndef RECORDINFO_H
#define RECORDINFO_H
#include <stdint.h>
#include <string>
using namespace std;
class RecordInfo
{
private:
    uint64_t prefix,
    signatureSize,
    offsetBegin,
    offsetEnd;
    string SHA;
public:
    RecordInfo(string signature,uint64_t sOffset,uint64_t eOffset);
};

#endif // RECORDINFO_H
