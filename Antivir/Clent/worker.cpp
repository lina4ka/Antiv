#include "worker.h"



void Worker::_reconnect()
{
    CloseHandle(iHandle);
    iHandle = NULL;
    iHandle = CreateFile(
                L"\\\\.\\pipe\\AntimalwarePipeBack", GENERIC_READ | GENERIC_WRITE,
                0, NULL, OPEN_EXISTING, 0, NULL);
    if (iHandle != NULL && iHandle != INVALID_HANDLE_VALUE)
        innerSender->changeHandle(iHandle);
}



void Worker::doWork()
{
    iHandle = NULL;
    innerSender.reset(new Sender(iHandle));
    msg Message;
    int count = 0;
    while(isWork)
    {
        if (count ==100)
            break;
        if (isWork == false)
            break;
        if (!innerSender->getFrom(Message))
        {
            this_thread::sleep_for(chrono::seconds(1));
            ++count;
            _reconnect();
            if (iHandle != NULL && iHandle != INVALID_HANDLE_VALUE)
                emit reconnect();
        }
        else
        {
            count = 0;
            emit toDo(Message);
        }
    }
    innerSender.reset();
    ExitProcess(0);
}
