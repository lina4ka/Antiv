#include "Basereader.h"

BaseReader::BaseReader(string signature, string pathToFile)
{
    this->recordSize = sizeof(RecordInfo::signatureSize)+sizeof(RecordInfo::prefix)+sizeof(RecordInfo::offsetEnd)+sizeof(RecordInfo::offsetBegin)
            +10/*MalvareName Size*/
            +64;/*SHA Size*/
    changeInnerInfo( signature,  pathToFile);
}

bool BaseReader::isFile()
{
    return (!spSign.empty() && !file.empty());
}

bool BaseReader::changeInnerInfo(string signature, string pathToFile)
{
    this->spSign=signature;
    if(isCorrectData((pathToFile)))
    {
       this->file = pathToFile;
    }
    else
    {
        this->spSign.clear();
        this->file.clear();
    }
    return isFile();
}

RecordInfo BaseReader::recordDecrypt(vector<uint8_t> buffer)
{
    RecordInfo temp;
    uint32_t offset=0;
    memcpy(temp.malvareName.data(),buffer.data()+offset,10);
    offset+=10;
    memcpy(&temp.prefix,buffer.data()+offset,sizeof (temp.prefix));
    offset+=sizeof (temp.prefix);
    memcpy(&temp.signatureSize,buffer.data()+offset,sizeof (temp.signatureSize));
    offset+=sizeof (temp.signatureSize);
    memcpy(&temp.offsetBegin,buffer.data()+offset,sizeof (temp.offsetBegin));
    offset+=sizeof (temp.offsetBegin);
    memcpy(&temp.offsetEnd,buffer.data()+offset,sizeof (temp.offsetEnd));
    offset+=sizeof (temp.offsetEnd);
    memcpy(temp.SHA.data(),buffer.data()+offset,64);
    offset+=64;
    return temp;
}

bool BaseReader::isCorrectData(string path)
{
    ifstream readStr(path,ios::binary);
    if (readStr.is_open())
    {
        string readSign(spSign.size(),0);
        readStr.read(&readSign[0],readSign.size());
        readStr.close();
        return readSign == spSign;
    }
    return false;
}

vector<RecordInfo> BaseReader::get()
{
    if (this->file.empty())
        return {};
    reader.open(this->file,ios::binary);
    reader.seekg(spSign.size());
    uint64_t countOfRecord = 0, count = 0;
    reader.read((char*)&countOfRecord,sizeof (countOfRecord));
    vector<RecordInfo> recordVector(countOfRecord);
    vector<uint8_t> buffer;
    buffer.resize(this->recordSize);
    while (!reader.eof() && count != countOfRecord)
    {
        reader.read(static_cast<char*>(static_cast<void*>(buffer.data())),this->recordSize);
        recordVector.at(count) = recordDecrypt(buffer);
        ++count;
    }
    reader.close();
    return recordVector;
}
