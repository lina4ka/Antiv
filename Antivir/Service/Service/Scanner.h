#pragma once
#include <memory>
#include "malwaredata.h"
#include <fstream>
#include <filesystem>
#include <zip.h>
#include <stack>
#include <mutex>
enum class TypeObject
{
	SIMPLE,EXE,ZIP
};

struct ScanObject
{
	ifstream stream;
	std::filesystem::path path;
	TypeObject type;
	uint64_t size;

};
class Scanner
{
private:
	SHA256 sha;
	int pe = 0x5a4d;
	int zip = 0x04034b50;
	size_t bufSize = 8128;
	shared_ptr<MalwareData> storage;
	bool isWork;
	std::mutex mutex;
public:
	Scanner(shared_ptr<MalwareData> _storage);
	int scan(ScanObject& obj);
	ScanObject createObject(std::filesystem::path path);
	vector<ScanObject> createObject(vector<string> path);
	void stop();
	void start();
	bool isStop();
private:
	ScanObject createObject(ifstream& _stream, std::filesystem::path path);
	bool scanPE(ScanObject& obj);
	bool scanZip(ScanObject& obj);
};

