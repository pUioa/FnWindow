#include "ConnectFn.h"
//打开已经存在的服务
BOOL ConnectDriver::GetSvcHandle(PCHAR pServiceName)
{
	this->m_pServiceName = pServiceName;
	this->m_hSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!this->m_hSCManager)
	{
		this->m_dwLastError = GetLastError();
		return FALSE;
	}
	this->m_hService = OpenServiceA(m_hSCManager, m_pServiceName, SERVICE_ALL_ACCESS);
	if (!this->m_hService)
	{
		CloseServiceHandle(this->m_hSCManager);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
	return FALSE;
}
//安装服务服务
BOOL ConnectDriver::Install(PCHAR pSysPath, PCHAR pServiceName, PCHAR pDisplayName)
{
	this->m_pSysPath = pSysPath;
	this->m_pServiceName = pServiceName;
	this->m_pDisplayName = pDisplayName;
	this->m_hSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!this->m_hSCManager)
	{
		this->m_dwLastError = GetLastError();
		return FALSE;
	}
	this->m_hService = CreateServiceA(this->m_hSCManager, this->m_pServiceName, this->m_pDisplayName,
		SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
		this->m_pSysPath, NULL, NULL, NULL, NULL, NULL);
	if (!this->m_hService)
	{
		m_dwLastError = GetLastError();
		if (this->m_dwLastError == ERROR_SERVICE_EXISTS)
		{
			this->m_hService = OpenServiceA(m_hSCManager, m_pServiceName, SERVICE_ALL_ACCESS);
			if (!this->m_hService)
			{
				CloseServiceHandle(this->m_hSCManager);
				return FALSE;
			}
		}
		else
		{
			CloseServiceHandle(this->m_hSCManager);
			return FALSE;
		}
	}
	return TRUE;
}
//启动服务
BOOL ConnectDriver::Start()
{
	if (!StartServiceA(this->m_hService, NULL, NULL))
	{
		this->m_dwLastError = GetLastError();
		return FALSE;
	}
	return TRUE;
}
//停止服务
BOOL ConnectDriver::Stop()
{
	SERVICE_STATUS ss;
	if (!ControlService(this->m_hService, SERVICE_CONTROL_STOP, &ss))
	{
		this->m_dwLastError = GetLastError();
		return FALSE;
	}
	return TRUE;
}
//移除服务
BOOL ConnectDriver::Remove()
{
	if (!DeleteService(this->m_hService))
	{

		this->m_dwLastError = GetLastError();
		return FALSE;
	}
	return TRUE;
}

DWORD ConnectDriver::CTL_CODE_GEN(DWORD lngFunction)
{
	return (FILE_DEVICE_UNKNOWN * 65536) | (FILE_ANY_ACCESS * 16384) | (lngFunction * 4) | METHOD_BUFFERED;
}
//和驱动实现通信的核心函数
BOOL ConnectDriver::IoControl(PCHAR pLinkName,DWORD dwIoCode, PVOID InBuff, DWORD InBuffLen, PVOID OutBuff, DWORD OutBuffLen)
{
	DWORD dw = 0;
	auto m_hDriver = CreateFileA(pLinkName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0);
	auto bRet= DeviceIoControl(m_hDriver, this->CTL_CODE_GEN(dwIoCode), InBuff, InBuffLen, OutBuff, OutBuffLen, &dw, NULL);
    CloseHandle(m_hDriver);
	return bRet;

}