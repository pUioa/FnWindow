#include <Windows.h>
#include <winsvc.h>
class ConnectDriver
{
public:
	ConnectDriver()//��ʼ����������
	{
		this->m_pSysPath = NULL;
		this->m_pServiceName = NULL;
		this->m_pDisplayName = NULL;
		this->m_hSCManager = NULL;
		this->m_hService = NULL;
		this->m_dwLastError = NULL;
	}
	~ConnectDriver()//�������
	{
		
	}
public:

	BOOL Install(PCHAR pSysPath, PCHAR pServiceName, PCHAR pDisplayName); //��װ��������
	BOOL Start();//������������
	BOOL Stop();//ֹͣ��������
	BOOL Remove();//�Ƴ���������
	
	BOOL IoControl(PCHAR pLinkName, DWORD dwIoCode, PVOID InBuff, DWORD InBuffLen, PVOID OutBuff, DWORD OutBuffLen);
private:
	DWORD m_dwLastError; //���Ĵ���
	PCHAR m_pSysPath; //����·��
	PCHAR m_pServiceName; //������
	PCHAR m_pDisplayName; //��ʾ��
	SC_HANDLE m_hSCManager; //SCM���
	SC_HANDLE m_hService; //������
private:
	DWORD CTL_CODE_GEN(DWORD lngFunction);
	BOOL GetSvcHandle(PCHAR pServiceName);
};
