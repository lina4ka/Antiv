#pragma once
#include <memory>
#include <string>
#include <Windows.h>

namespace ServiceLib::ServiceManagement
{
	class Service
	{
	public:
		Service(const std::u16string& serviceName);

		const std::u16string& GetName() const;

		int Start();
		void Stop();
		void Shutdown();
		void NotifyState();

	protected:
		virtual int Init() = 0;
		virtual void DoWork() = 0;
		virtual int DoStop() = 0;

	private:
		std::u16string m_serviceName;

		SERVICE_STATUS m_serviceStatus;
		SERVICE_STATUS_HANDLE m_serviceStatusHandle;
	};

	struct IServiceFactory
	{
		virtual std::unique_ptr<Service> Create() = 0;
	};
}
