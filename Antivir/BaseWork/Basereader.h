#ifndef BASEREADER_H
#define BASEREADER_H
#include <fstream>
#include <string>
#include <vector>
#include "Recordinfo.h"

using namespace std;
class BaseReader
{
private:
    uint64_t recordSize;
    ifstream reader;
    string spSign,file;
public:
    BaseReader(string signature, string pathToFile);
    bool isFile();

    vector<RecordInfo> get();
private:
    bool isCorrectData(string path);
    bool changeInnerInfo(string signature, string pathToFile);
    RecordInfo recordDecrypt(vector<uint8_t> buffer);
};

#endif // BASEREADER_H
