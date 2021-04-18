#include "Service.h"
namespace ServiceLib::ServiceManagement
{
	void WINAPI ControlHandler(DWORD request);

	Service::Service(const std::u16string& serviceName)
		:  m_serviceName{ serviceName }
		, m_serviceStatus{ 0 }
		, m_serviceStatusHandle{ 0 }
	{
		m_serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
		m_serviceStatus.dwCurrentState = SERVICE_START_PENDING;
		m_serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	}

	const std::u16string& Service::GetName() const
	{
		return m_serviceName;
	}

	int Service::Start()
	{
		m_serviceStatus.dwCurrentState = SERVICE_START_PENDING;

		m_serviceStatusHandle = RegisterServiceCtrlHandlerW(reinterpret_cast<LPCWSTR>(m_serviceName.c_str()), (LPHANDLER_FUNCTION)ControlHandler);
		if (m_serviceStatusHandle == (SERVICE_STATUS_HANDLE)0) {
			//m_tracer.Trace(TraceLevel::Error) << GetLastErrorAsString();
			return -1;
		}
		
		int error = Init();
		if (error) {
			m_serviceStatus.dwCurrentState = SERVICE_STOPPED;
			m_serviceStatus.dwWin32ExitCode = error;
			NotifyState();
			//m_tracer.Trace(TraceLevel::Error) << ("Init failed: error = ") << error;
			return error;
		}

		m_serviceStatus.dwCurrentState = SERVICE_RUNNING;
		NotifyState();


		while (m_serviceStatus.dwCurrentState == SERVICE_RUNNING)
		{
			DoWork();
		}

		return 0;
	}

	void Service::Stop()
	{
		m_serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		//m_tracer.Trace(TraceLevel::Info) << ("Stopping...");

		NotifyState();
		if (DoStop() == 0)
		{
			//m_tracer.Trace(TraceLevel::Info) << ("Stopped.");
			m_serviceStatus.dwWin32ExitCode = 0;
			m_serviceStatus.dwCurrentState = SERVICE_STOPPED;
			NotifyState();
		}
	}

	void Service::Shutdown()
	{
		//m_tracer.Trace(TraceLevel::Info) << ("Shutting down...");
		if (DoStop() == 0)
		{
			//m_tracer.Trace(TraceLevel::Info) << ("Shutdown.");
			m_serviceStatus.dwWin32ExitCode = 0;
			m_serviceStatus.dwCurrentState = SERVICE_STOPPED;
			NotifyState();
		}
	}

	void Service::NotifyState()
	{
		SetServiceStatus(m_serviceStatusHandle, &m_serviceStatus);
	}
}

