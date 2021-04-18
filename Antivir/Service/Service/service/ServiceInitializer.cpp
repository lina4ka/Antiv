#include "ServiceInitializer.h"

namespace ServiceLib::ServiceManagement
{
	std::unique_ptr<Service> pService = nullptr;

	void WINAPI ServiceMain(DWORD argc, wchar_t** argv)
	{
		pService->Start();
	}

	void InitService( IServiceFactory& serviceFactory)
	{
		pService = serviceFactory.Create();
		std::u16string serviceName = pService->GetName();
		SERVICE_TABLE_ENTRYW ServiceTable[2];
		ServiceTable[0].lpServiceName = reinterpret_cast<LPWSTR>(const_cast<char16_t*>(serviceName.c_str()));
		ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTIONW)ServiceMain;
		ServiceTable[1].lpServiceName = NULL;
		ServiceTable[1].lpServiceProc = NULL;

		if (!StartServiceCtrlDispatcherW(ServiceTable)) {
			int i = GetLastError();
			int r = 0;
			//tracer->Trace(TraceLevel::Error) << ("Error: StartServiceCtrlDispatcher: ") << GetLastError();
		}
	}

	void WINAPI ControlHandler(DWORD request)
	{
		if (!pService)
		{
			return;
		}
		switch (request)
		{
		case SERVICE_CONTROL_STOP:
			pService->Stop();
			return;
		case SERVICE_CONTROL_SHUTDOWN:
			pService->Shutdown();
			return;
		}
		pService->NotifyState();
	}
}