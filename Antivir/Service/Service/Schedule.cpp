#include "Schedule.h"

Schedule::Schedule(shared_ptr<Scanner> _scan)
{
	scanner = _scan;
	memPath = std::filesystem::current_path().string() + "\\ANTIV_memoryFolder\\Schedule.bin";
	load();
}

void Schedule::addSchedule(string path, uint64_t time)
{
	mutex.lock();
	scheduleList.emplace_back(ScheduleCell(time,path,scanner));
	thread th(&Schedule::ScheduleCell::start,&scheduleList.back());
	th.detach();
	mutex.unlock();
}

void Schedule::removeSchedule(string path, uint64_t time)
{
	mutex.lock();
	auto res = find_if(scheduleList.begin(), scheduleList.end(), [&](ScheduleCell& el)
		{
			return el.path == path;
		});
	if (res != scheduleList.end())
	{
		auto index = distance(scheduleList.end(), res);
		scheduleList.at(scheduleList.size() + index).status = -1;
	}
	mutex.unlock();
}

vector<scheduleIndo> Schedule::getActive()
{
	mutex.lock();
	vector<scheduleIndo> vec;
	for (auto& el : scheduleList)
		if (el.status ==2)
			vec.emplace_back(scheduleIndo(el.time, el.status, el.path, el.resPath));
	mutex.unlock();
	return vec;
}

vector<scheduleIndo> Schedule::getReult()
{
	mutex.lock();
	vector<scheduleIndo> vec;
	for (auto& el : scheduleList)
	{
		if (el.status == 1 || el.status == 0)
		{
			vec.emplace_back(scheduleIndo(el.time, el.status, el.path,el.resPath));
			el.status = -1;
		}
	}
	mutex.unlock();
	return vec;
}

void Schedule::save()
{
	vector<ScheduleCell> vec;
	for (auto& el : scheduleList)
		if (el.status == 2)
			vec.emplace_back(el);
	uint64_t size = vec.size();
	ofstream writer(memPath, ios::binary);
	if (!writer.is_open())
		return;
	writer.write((char*)&size, sizeof(size));
	for (auto& el : vec)
	{		
		size = el.path.string().size();
		writer.write((char*)&size, sizeof(size));
		writer.write(el.path.string().data(), size);
		writer.write((char*)&el.time, sizeof(el.time));
	}
	writer.close();
}

void Schedule::load()
{
	vector<pair<string, uint64_t>> recoverList;
	ifstream reader(memPath, ios::binary);
	if (!reader.is_open())
		return;
	uint64_t size;
	reader.read((char*)&size, sizeof(size));
	recoverList.resize(size);
	for (size_t i = 0; i < recoverList.size(); ++i)
	{
		reader.read((char*)&size, sizeof(size));
		recoverList.at(i).first.resize(size);
		reader.read(recoverList.at(i).first.data(), size);
		reader.read((char*)&recoverList.at(i).second, sizeof(recoverList.at(i).second));
	}
	for (auto& el : recoverList)
		this->addSchedule(el.first, el.second);
}

void Schedule::ScheduleCell::start()
{
	std::time_t t = std::time(0);
	std::tm* now = std::localtime(&t);
	uint64_t curTime = now->tm_hour * 60 + now->tm_min;
	if (time > curTime)
		this_thread::sleep_for(chrono::minutes(time - curTime));
	auto vecObj = scanner->createObject(vector<string>{ path.string() });
	for (auto& el : vecObj)
	{
		int res = scanner->scan(el);
		if (res == 1)
			resPath.emplace_back(el.path);
		
	}
	if (resPath.empty())
		this->status = 0;
	else
		this->status = 1;
}
