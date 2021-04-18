#include "infoStorage.h"

bool infoStorage::transfer(uint64_t index, bool fromDetected)
{
    if (fromDetected)
    {
        if (detectedPath.size() <= index)
            return false;
        quarantinPath.emplace_back(detectedPath.at(index));
        transferFile(detectedPath.at(index));
        detectedPath.erase(detectedPath.begin() + index);
        quarantinPath.erase(unique(quarantinPath.begin(), quarantinPath.end()), quarantinPath.end());
        
    }
    else
    {
        if (quarantinPath.size() <= index)
            return false;
        detectedPath.emplace_back(quarantinPath.at(index));
        transferFile(quarantinPath.at(index),true);
        quarantinPath.erase(quarantinPath.begin() + index);
        detectedPath.erase(unique(detectedPath.begin(), detectedPath.end()), detectedPath.end());
    }
    return true;
}

vector<string> infoStorage::getInnerInfo()
{
    vector<string> res;
    for (auto const& el : detectedPath)
        res.emplace_back(el.string());
    res.emplace_back("$Quar");
    for (auto const& el : quarantinPath)
        res.emplace_back(el.string());
    return res;
}

bool infoStorage::addDetected(std::filesystem::path path)
{
    detectedPath.emplace_back(path);
    detectedPath.erase(unique(detectedPath.begin(), detectedPath.end()), detectedPath.end());
    return true;
}

infoStorage::infoStorage()
{
    memPath = std::filesystem::current_path().string() +"\\ANTIV_memoryFolder\\Storage.bin";
    quatPath = std::filesystem::current_path().string() + "\\ANTIV_quarFolder";
    load();
}

infoStorage::~infoStorage()
{
    save();
}

void infoStorage::save()
{
    ofstream writer(memPath, ios::binary);
    if (!writer.is_open())
        return;
    uint64_t size = detectedPath.size() + quarantinPath.size()+1;
    uint64_t iSize;
    writer.write((char*)&size, sizeof(size));
    vector<string> allRec = this->getInnerInfo();
    for (auto const& el : allRec)
    {
        iSize = el.size();
        writer.write((char*)&iSize, sizeof(iSize));
        writer.write(el.data(), iSize);
    }
    writer.close();
}

bool infoStorage::removeFile(uint64_t index)
{
    if (detectedPath.size() <= index)
        return false;
    std::filesystem::remove(detectedPath.at(index));
    detectedPath.erase(detectedPath.begin() + index);
}

void infoStorage::load()
{
    ifstream reader(memPath, ios::binary);
    if (!reader.is_open())
        return;
    uint64_t size;
    uint64_t iSize;
    string buf;
    bool isFirst = true;
    reader.read((char*)&size, sizeof(size));
    for (uint64_t i = 0; i < size; ++i)
    {
        reader.read((char*)&iSize, sizeof(iSize));
        buf.resize(iSize);
        reader.read(buf.data(), iSize);
        if (buf == "$Quar")
            isFirst = false;
        else if (isFirst)
            detectedPath.emplace_back(buf);
        else
            quarantinPath.emplace_back(buf);
    }
    reader.close();
}

void infoStorage:: transferFile(std::filesystem::path fileOldPath, bool isRestore)
{
    std::filesystem::path fileName = fileOldPath.filename();
    if (!isRestore)
    {
        std::filesystem::copy_file(fileOldPath.string(), quatPath.string() + "\\" + fileName.string(),std::filesystem::copy_options::overwrite_existing);
        std::filesystem::remove(fileOldPath.string());
        breakFile(quatPath.string() + "\\" + fileName.string());
    }
    else
    {
        breakFile(quatPath.string() + "\\" + fileName.string());
        std::filesystem::copy_file(quatPath.string() + "\\" + fileName.string(), fileOldPath.string());
        std::filesystem::remove(quatPath.string() + "\\" + fileName.string());
    }
}

void infoStorage::breakFile(std::filesystem::path path)
{
    std::ifstream fs(path, std::ios::binary);
    uint64_t header = 0;
    fs.read((char*)&header, sizeof(uint64_t));
    fs.close();
    header = ~header;
    std::fstream fi(path, std::ios::in | std::ios::out | std::ios::binary);
    fi.seekp(0);
    fi.write((char*)&header, sizeof(uint64_t));
    fi.close();
}
