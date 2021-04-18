#ifndef MSG_H
#define MSG_H
#include <vector>
#include <string>
using namespace std;
enum class COMMANDLIST : uint8_t { UNKNOWN, START_SCAN, STOP_SCAN, MOVE_TO_QUARANTINE,RETURN_FROM_QUARANTINE, DELETE_FILE, GET_REPORT,
                                 START_MONITOR, END_MONITOR,START_SCHEDULE,STOP_SCHEDULE,OFF,
                                 UPDATE_INFO };//command list
struct msg//a structure containing the arguments to be passed
{
    COMMANDLIST command;//command
    vector<uint64_t> numericArray;//numeric array
    vector<string> stringArray;//string array
};
#endif // MSG_H
