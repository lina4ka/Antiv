#include "Scanner.h"

Scanner::Scanner(shared_ptr<MalwareData> _storage)
{
	storage = _storage;
	isWork = true;
}
bool Scanner::scanPE(ScanObject& obj)
{
	uint64_t offsetGlob = 0;
	string buf(bufSize, 0);
	do
	{
		obj.stream.read(buf.data(), bufSize);
		if (storage->getSize() < (bufSize / 8))
		{
			string prefix(sizeof(uint64_t),0);
			auto _vec = storage->getList();
			for (auto& el : _vec)
			{
				std::memcpy(prefix.data(), &el.prefix, sizeof(uint64_t));
				int64_t index = buf.find(prefix);
				if (index == std::string::npos)
					continue;
				if ((buf.size() - index) >= el.signatureSize)
				{
					std::string _str = buf.substr(index, el.signatureSize);
					if ((offsetGlob + index >= el.offsetBegin) && (offsetGlob + index <= el.offsetEnd))
						if (el.SHA == sha.getSHA(_str))
							return true;
				}
			}
		}
		else
		{
			uint64_t prefix, offsetInner = 0;
			while (offsetInner <bufSize)
			{
				memcpy(&prefix, buf.data() + offsetInner, sizeof(prefix));
				++offsetInner;
				auto rec = storage->isExist(prefix);
				if (rec.malvareName.empty())
					continue;
				if (offsetGlob + offsetInner > rec.offsetBegin && offsetGlob + offsetInner < rec.offsetEnd)
					if ((buf.size() - offsetInner) >= rec.signatureSize)
					{
						std::string _str = buf.substr(offsetInner, rec.signatureSize);
						if (rec.SHA == sha.getSHA(_str))
							return true;
					}
			}
		}
		offsetGlob += bufSize;
	}while (offsetGlob < obj.size);
	return false;
}

bool Scanner::scanZip(ScanObject& obj)
{
	int err;
	obj.stream.close();
	stack<ScanObject> innerZip;
	struct zip* hZip = zip_open(obj.path.string().c_str(), 0, &err);
	if (hZip)
	{
		size_t totalIndex = zip_get_num_entries(hZip, 0);
		for (size_t i = 0; i < totalIndex; i++)
		{
			struct zip_stat st;
			zip_stat_init(&st);
			zip_stat_index(hZip, i, 0, &st);

			struct zip_file* zf = zip_fopen_index(hZip, i, 0);
			std::vector<char> buffer;
			if (!zf)
			{
				zip_close(hZip);
				return {};
			}
			if (st.size == 0)
			{
				continue;
			}
			std::filesystem::path path_ = std::filesystem::current_path().string() + "\\UNZIP" + st.name;
			std::ofstream fs(path_, std::ios::binary | std::ios::trunc | std::ios::out);
			uint64_t offset{ 0 };
			uint64_t readSize{ 1024 * 1024 };
			while (true)
			{
				if (offset >= st.size)
					break;
				if (offset + readSize >= st.size)
					readSize = st.size - offset;
				buffer.resize(readSize);
				zip_fread(zf, buffer.data(), readSize);
				fs.write(buffer.data(), buffer.size());
				offset += readSize;
				readSize = 1024 * 1024;
			}
			innerZip.emplace( this->createObject(path_));
			if (innerZip.top().type != TypeObject::ZIP)
			{
				int res = this->scan(innerZip.top());
				fs.close();
				innerZip.top().stream.close();
				std::filesystem::remove(path_);
				innerZip.pop();
				if (res == true)
				{
					zip_fclose(zf);
					zip_close(hZip);
					return true;
				}
				
			}
			zip_fclose(zf);
			
		}
		zip_close(hZip);
	}
	while (innerZip.size())
	{
		if (scanZip(innerZip.top()) == 1)
			return true;
		innerZip.pop();
	}
	return false;
}

int Scanner::scan(ScanObject& obj)
{
	switch (obj.type)
	{
	case TypeObject::SIMPLE:
		return 2;
	case TypeObject::EXE:
		return scanPE(obj);
	case TypeObject::ZIP:
		return scanZip(obj);
	}
	return 2;
}

ScanObject Scanner::createObject(std::filesystem::path path)
{
	ifstream stream(path, ios::binary);
	ScanObject obj = createObject(stream,path);
	obj.size = std::filesystem::file_size(path);
	return obj;

}

vector<ScanObject> Scanner::createObject(vector<string> path)
{
	vector<ScanObject> vec;
	for (uint64_t i = 0; i < path.size(); ++i)
	{
		auto& el = path.at(i);
		if (std::filesystem::is_directory(el))
		{
			for (auto& p : std::filesystem::directory_iterator(el))
				path.emplace_back(p.path().string());
			continue;
		}
		vec.emplace_back(createObject(el));		
	}
	return vec;
}

void Scanner::stop()
{
	mutex.lock();
	isWork = false;
	mutex.unlock();
}
void Scanner::start()
{
	mutex.lock();
	isWork = true;
	mutex.unlock();
}
bool Scanner::isStop()
{
	mutex.lock();
	bool isStop = !isWork;
	mutex.unlock();
	return isStop;
}
ScanObject Scanner::createObject(ifstream& _stream, std::filesystem::path path)
{
	ScanObject obj;
	uint16_t mzHeader = 0;
	uint32_t zipHeader = 0;
	_stream.read((char*)&mzHeader, sizeof(uint16_t));
	_stream.seekg(0);
	_stream.read((char*)&zipHeader, sizeof(uint32_t));
	_stream.seekg(0);
	if(mzHeader == pe)
		obj.type = TypeObject::EXE;
	else if (zip == zipHeader)
		obj.type = TypeObject::ZIP;
	else
		obj.type = TypeObject::SIMPLE;
	_stream.close();
	obj.stream.open(path,ios::binary);
	obj.path = path;
	return obj;
}

