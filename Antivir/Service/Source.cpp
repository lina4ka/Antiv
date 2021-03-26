#include "../IO/Sender.hpp"
#include <iostream>
int main(int argc, char* argv[])
{
	HANDLE hNamedPipe = CreateNamedPipe(
		L"\\\\.\\pipe\\NAME",
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		4096, 4096, 5000, NULL);

	ConnectNamedPipe(hNamedPipe, NULL);	
	Sender ms(hNamedPipe);
	std::cout << "connected\n";
	while (true)
	{
		msg m;
		if (ms.getFrom(m))
		{
			std::cout <<"Command - " <<(int)m.command << std::endl << "Uint element - ";
			for (auto &el : m.numericArray)
				std::cout << el << " ";
			std::cout << std::endl << "String element - ";
			for (auto& el : m.stringArray)
				std::cout << el << " ";
			std::cout << std::endl;
		}
	}
	return 0;
}