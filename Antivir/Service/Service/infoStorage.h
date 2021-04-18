#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <fstream>
using namespace std;
class infoStorage
{
private:
	vector <std::filesystem::path> detectedPath,quarantinPath;
	std::filesystem::path quatPath;
	std::filesystem::path memPath;
public:
	bool transfer(uint64_t index, bool fromDetected = true);
	vector<string> getInnerInfo();
	bool addDetected(std::filesystem::path path);
	infoStorage();
	~infoStorage();
	void save();
	bool removeFile(uint64_t index);
private:	
	void load();
	void transferFile(std::filesystem::path fileOldPath,bool isRestore = false);
	void breakFile(std::filesystem::path path);
};

