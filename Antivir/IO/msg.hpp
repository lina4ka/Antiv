#pragma once
#include <vector>
#include <string>
enum class COMMANDLIST : uint8_t { UNKNOWN, START_SCAN, STOP_SCAN, MOVE_TO_QUARANTINE, DELETE_FILE, GET_REPORT };//command list
struct msg//a structure containing the arguments to be passed
{
	COMMANDLIST command;//command
	std::vector<uint64_t> numericArray;//numeric array
	std::vector<std::string> stringArray;//string array
};