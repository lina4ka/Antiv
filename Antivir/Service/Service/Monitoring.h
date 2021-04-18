#pragma once
#include "Scanner.h"
#include <thread>
#include <mutex>
class MonitorCell
{
private:
	shared_ptr<Scanner> scanner;
	std::filesystem::path path;
	unordered_map<string, std::filesystem::file_time_type> fileMonitoring;
	vector<std::filesystem::path> resPath;
	std::mutex mutex;
	bool isWork = true;
	void scanIt(string file);
public:
	MonitorCell(shared_ptr<Scanner> _scanner, std::filesystem::path _path);
	MonitorCell(MonitorCell const& el);
	void start();
	vector<string> getResult();
	void stop();
};
class Monitoring
{
private:
	shared_ptr<Scanner> scanner;
	vector<std::filesystem::path> monitorList;
	vector<MonitorCell*> cell;
	void load();
	std::mutex mutex;
	std::filesystem::path memPath;
public:
	Monitoring(shared_ptr<Scanner> _scanner);
	void save();
	void addMonitoring(std::filesystem::path path);
	void delMonitoring(uint64_t index);
	vector<string> result();
	vector<string> getList();
	~Monitoring();
};

