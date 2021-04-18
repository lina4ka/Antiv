#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <memory>
#include "Scanner.h"
#include <thread>
#include <algorithm>
#include <chrono>
struct scheduleIndo
{
	uint64_t time;
	int status;
	std::filesystem::path path;
	vector<std::filesystem::path> resPath;
	scheduleIndo(uint64_t _time, int _status, std::filesystem::path _path, vector<std::filesystem::path> _resPath) : time(_time), status(_status), path(_path),resPath(_resPath) {};
};
class Schedule
{
private:
	class ScheduleCell
	{
	public:
		std::filesystem::path path;
		uint64_t time;
		int status;
		shared_ptr<Scanner> scanner;
		vector<std::filesystem::path> resPath;
		ScheduleCell(uint64_t _time, std::filesystem::path _path, shared_ptr<Scanner> _scanner)
			:status(2), time(_time), path(_path), scanner(_scanner) {};
		void start();
	};
	vector<ScheduleCell> scheduleList;
	shared_ptr<Scanner> scanner;
	std::filesystem::path memPath;
	std::mutex mutex;
public:
	Schedule(shared_ptr<Scanner> _scan);
	void addSchedule(string path, uint64_t time);
	void removeSchedule(string path, uint64_t time);
	vector<scheduleIndo> getActive();
	vector<scheduleIndo > getReult();
	void save();
private:
	void load();
};

