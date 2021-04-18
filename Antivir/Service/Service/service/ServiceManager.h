#pragma once
#include <string>
#include <Windows.h>
namespace ServiceLib::ServiceManagement
{
	class ServiceManager
	{
	public:
		ServiceManager( const std::u16string& serviceName, const std::u16string& servicePath)
			: m_serviceName(serviceName)
			, m_servicePath(servicePath)
		{ }

		int InstallService();
		int UninstallService();
		int RunService();

	private:
		std::u16string m_serviceName;
		std::u16string m_servicePath;
	};
}
