#include "Monitoring.h"

void Monitoring::load()
{
	vector<string> recoverList;
	ifstream reader(memPath, ios::binary);
	if (!reader.is_open())
		return;
	uint64_t size;
	reader.read((char*)&size, sizeof(size));
	recoverList.resize(size);
	for (size_t i = 0; i < recoverList.size(); ++i)
	{
		reader.read((char*)&size, sizeof(size));
		recoverList.at(i).resize(size);
		reader.read(recoverList.at(i).data(), size);
	}
	for (auto& el : recoverList)
		this->addMonitoring(el);
}

Monitoring::Monitoring(shared_ptr<Scanner> _scanner)
{
	scanner = _scanner;
	memPath = std::filesystem::current_path().string() + "\\ANTIV_memoryFolder\\Monitor.bin";
	load();
}

void Monitoring::save()
{
	vector<string> vec;
	for (auto& el : monitorList)
		vec.emplace_back(el.string());
	uint64_t size = vec.size();
	ofstream writer(memPath, ios::binary);
	if (!writer.is_open())
		return;
	writer.write((char*)&size, sizeof(size));
	for (auto& el : vec)
	{
		size = el.size();
		writer.write((char*)&size, sizeof(size));
		writer.write(el.data(), size);
	}
	writer.close();
}

void Monitoring::addMonitoring(std::filesystem::path path)
{
	mutex.lock();
	monitorList.emplace_back(path);
	cell.emplace_back(new MonitorCell(scanner,path));
	thread t(&MonitorCell::start, cell.back());
	t.detach();
	mutex.unlock();
}

void Monitoring::delMonitoring(uint64_t index)
{
	mutex.lock();
	cell.at(index)->stop();
	cell.erase(cell.begin() + index);
	monitorList.erase(monitorList.begin() + index);
	mutex.unlock();
}

vector<string> Monitoring::result()
{
	vector<string> vec;
	mutex.lock();
	for (auto& el : cell)
		for (auto& _el : el->getResult())
			vec.emplace_back(_el);
	mutex.unlock();
	return vec;
}

vector<string> Monitoring::getList()
{
	vector<string> vec;
	mutex.lock();
	for (auto& el : monitorList)
		vec.emplace_back(el.string());
	mutex.unlock();
	return vec;
}

Monitoring::~Monitoring()
{
	for (size_t i = 0; i < cell.size(); ++i)
		delete cell.at(i);
}

void MonitorCell::scanIt(string file)
{
	fileMonitoring[file] = std::filesystem::last_write_time(file);
	auto vecObj = scanner->createObject(vector<string>{ file});
	for (auto& el : vecObj)
	{
		int res = scanner->scan(el);
		if (res == 1)
			resPath.emplace_back(el.path);
	}
}


MonitorCell::MonitorCell(shared_ptr<Scanner> _scanner, std::filesystem::path _path)
{
	path = _path;
	scanner = _scanner;
	fileMonitoring[path.string()] = std::filesystem::last_write_time(path);
}

MonitorCell::MonitorCell(MonitorCell const& el)
{
	path = el.path;
	scanner = el.scanner;
	fileMonitoring[path.string()] = std::filesystem::last_write_time(path);
}

void MonitorCell::start()
{
	while (isWork)
	{
		std::this_thread::sleep_for(std::chrono::seconds(5));
		mutex.lock();
		auto it = fileMonitoring.begin();
		while (it != fileMonitoring.end())
		{
			if (!std::filesystem::exists(it->first))
				it = fileMonitoring.erase(it);
			else
				++it;
		}
        for (auto& file : std::filesystem::recursive_directory_iterator(path))
        {
            auto current_file_last_write_time = std::filesystem::last_write_time(file);
            if (!fileMonitoring.contains(file.path().string()))
				scanIt(file.path().string());
            else if (fileMonitoring[file.path().string()] != current_file_last_write_time)
				scanIt(file.path().string());            
        }
		mutex.unlock();
	}
}

vector<string> MonitorCell::getResult()
{
	vector<string> vec;
	mutex.lock();
	for (auto& el : resPath)
		vec.emplace_back(el.string());
	resPath.clear();
	mutex.unlock();
	return vec;
}

void MonitorCell::stop()
{
	mutex.lock();
	isWork = false;
	mutex.unlock();
}
