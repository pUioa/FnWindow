#include <Windows.h>
#include <winsvc.h>
class ConnectDriver
{
public:
	ConnectDriver()//初始化各个变量
	{
		this->m_pSysPath = NULL;
		this->m_pServiceName = NULL;
		this->m_pDisplayName = NULL;
		this->m_hSCManager = NULL;
		this->m_hService = NULL;
		this->m_dwLastError = NULL;
	}
	~ConnectDriver()//清除垃圾
	{
		
	}
public:

	BOOL Install(PCHAR pSysPath, PCHAR pServiceName, PCHAR pDisplayName); //安装驱动服务
	BOOL Start();//启动驱动服务
	BOOL Stop();//停止驱动服务
	BOOL Remove();//移除驱动服务
	
	BOOL IoControl(PCHAR pLinkName, DWORD dwIoCode, PVOID InBuff, DWORD InBuffLen, PVOID OutBuff, DWORD OutBuffLen);
private:
	DWORD m_dwLastError; //最后的错误
	PCHAR m_pSysPath; //驱动路径
	PCHAR m_pServiceName; //服务名
	PCHAR m_pDisplayName; //显示名
	SC_HANDLE m_hSCManager; //SCM句柄
	SC_HANDLE m_hService; //服务句柄
private:
	DWORD CTL_CODE_GEN(DWORD lngFunction);
	BOOL GetSvcHandle(PCHAR pServiceName);
};
