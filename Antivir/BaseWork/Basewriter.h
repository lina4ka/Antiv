#ifndef BASEWRITER_H
#define BASEWRITER_H
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include "Recordinfo.h"



class BaseWriter
{
private:
    uint64_t recordSize;
    string spSign;
    ofstream writer;
public:
    BaseWriter(string signature);
    string createData(string path);
    bool writeIn(string path, vector<RecordInfo> buf);
private:
     bool isCorrectData(string path);
     string createData(string path,uint64_t count);
     vector<uint8_t> encryptRecord(RecordInfo rec);
};

#endif // BASEWRITER_H
