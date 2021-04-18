#include <iostream>
#include "service/AntimalwareService.h"
#include "service/ServiceInitializer.h"
#include "service/ServiceManager.h"
using namespace Antimalware::ServiceApp;
using namespace ServiceLib::ServiceManagement;

int wmain(int argc, wchar_t* argv[])
{
    /*AntimalwareService test;
    test.Start();*/
    std::wcout.imbue(std::locale(".866"));
    std::wcerr.imbue(std::locale(".866"));
    std::wcin.imbue(std::locale(".866"));

    if (argc - 1 == 0)
    {
        AntimalwareServiceFactory serviceFactory;
        InitService(serviceFactory);
    }
    else
    {
        ServiceManager sm{ u"AntimalwareService_Sirotkina", reinterpret_cast<char16_t*>(argv[0]) };
        if (wcscmp(argv[argc - 1], L"--install") == 0) {
            sm.InstallService();
        }
        else if (wcscmp(argv[argc - 1], L"--uninstall") == 0) {
            sm.UninstallService();
        }
        else if (wcscmp(argv[argc - 1], L"--start") == 0) {
            sm.RunService();
        }
    }
    return 0;
}