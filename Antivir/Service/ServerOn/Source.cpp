#include <windows.h>
int main()
{
    SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);

    if (NULL == schSCManager)
    {
        return 1;
    }

    SC_HANDLE schService = OpenServiceW(schSCManager, (LPCWSTR)L"AntimalwareService_Sirotkina", SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_ENUMERATE_DEPENDENTS);

    if (schService == NULL)
    {
        CloseServiceHandle(schSCManager);
        return 1;
    }
    StartService(schService, NULL, NULL);
    return 0;
}