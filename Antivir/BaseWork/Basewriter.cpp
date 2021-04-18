#include "Basewriter.h"

BaseWriter::BaseWriter(string signature)
{
    spSign=signature;
    this->recordSize = sizeof(RecordInfo::signatureSize)+sizeof(RecordInfo::prefix)+sizeof(RecordInfo::offsetEnd)+sizeof(RecordInfo::offsetBegin)
            +10/*MalvareName Size*/
            +64;/*SHA Size*/
}


string BaseWriter::createData(string path)
{
    return createData(path,0);
}


bool BaseWriter::writeIn(string path, vector<RecordInfo> buf)
{
    if (isCorrectData(path))
    {
        createData(path,buf.size());
        vector<uint8_t> encrData;
        writer.open(path,ios::binary|ios::app);
        for (RecordInfo const& rec: buf)
        {
            encrData=encryptRecord(rec);
            writer.write(static_cast<char*>(static_cast<void*>(encrData.data())),encrData.size());
        }
        writer.close();
    }
    return false;
}

bool BaseWriter::isCorrectData(string path)
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

string BaseWriter::createData(string path, uint64_t count)
{
    if (path.empty())
        path= filesystem::current_path().string()+"\\base.bin";
    writer.open(path,ios::binary | ios::trunc);
    writer.write(spSign.data(),spSign.size());
    writer.write(static_cast<char*>(static_cast<void*>(&count)),sizeof (count));
    writer.close();
    return path;
}

vector<uint8_t> BaseWriter::encryptRecord(RecordInfo rec)
{
    vector<uint8_t> temp(recordSize);
    uint32_t offset=0;
    memcpy(temp.data(),rec.malvareName.data(),10);
    offset+=10;
    memcpy(temp.data()+offset,&rec.prefix,sizeof (rec.prefix));
    offset+=sizeof (rec.prefix);
    memcpy(temp.data()+offset,&rec.signatureSize,sizeof (rec.signatureSize));
    offset+=sizeof (rec.signatureSize);
    memcpy(temp.data()+offset,&rec.offsetBegin,sizeof (rec.offsetBegin));
    offset+=sizeof (rec.offsetBegin);
    memcpy(temp.data()+offset,&rec.offsetEnd,sizeof (rec.offsetEnd));
    offset+=sizeof (rec.offsetEnd);
    memcpy(temp.data()+offset,rec.SHA.data(),64);
    offset+=64;
    return temp;
}
