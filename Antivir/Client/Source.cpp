#include "../IO/Sender.hpp"
#include <iostream>
int main(int argc, char* argv[])
{
	HANDLE hNamedPipe = CreateFile(
		L"\\\\.\\pipe\\NAME", GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);
	Sender ms(hNamedPipe);
	while (true)
	{
		uint64_t a,com;
		std::cout << "Command 1-5- ";
		std::cin >> com;
		if (com > 5)
			continue;
		std::cout << "array? 0/1 -";
		std::cin >> a;
		if (a)
		{
			std::cin >> a;
			ms.sendTo((COMMANDLIST)com,std::vector<uint64_t> { a });
		}
		else
		{
			std::string s;
			std::cin >> s;
			ms.sendTo((COMMANDLIST)com, std::vector<std::string> { s });
			
		}
	}
	return 0;
}