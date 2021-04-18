#include "serviceoner.h"
#include <Windows.h>
ServiceOner::ServiceOner(const char* name)
{

}

void ServiceOner::startService()
{
    SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);

        if (NULL == schSCManager)
        {
            return;
        }

        SC_HANDLE schService = OpenService(schSCManager,
            L"NadyezhniyAntimalwareService",
            SERVICE_START |	SERVICE_QUERY_STATUS |	SERVICE_ENUMERATE_DEPENDENTS);

        if (schService == NULL)
        {
            CloseServiceHandle(schSCManager);
            return;
        }
        StartService(schService, NULL, NULL);
}
