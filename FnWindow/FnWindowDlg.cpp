
// FnWindowDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FnWindow.h"
#include "FnWindowDlg.h"
#include "afxdialogex.h"
#include <string>
#include <vector>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <WinInet.h>
#pragma comment(lib, "wininet.lib")

#define useDriver 1


std::string szSysFile = _xor_("C:\\Hikari\\MicProtect.sys");
std::string szLinkName = _xor_("MicProtect");
std::string szServiceName = _xor_("MicProtect");
bool isLoadDriver = false;

ConnectDriver* MicDriver = new ConnectDriver;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框




// CFnWindowDlg 对话框



CFnWindowDlg::CFnWindowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FNWINDOW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFnWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFnWindowDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON1, &CFnWindowDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CFnWindowDlg 消息处理程序

BOOL CFnWindowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFnWindowDlg::OnSysCommand(UINT nID, LPARAM lParam)
{

		CDialogEx::OnSysCommand(nID, lParam);
	
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFnWindowDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFnWindowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD rand_num(int min,int max)
{

	return min + rand() % (max - min + 1);

}
std::string rand_str(const int len)
{

	std::string str;
	char c;
	int idx;

	for (idx = 0; idx < len; idx++)
	{

		c = 'a' + rand() % 26;
		str.push_back(c);
	}
	return str;

}
bool URLDownLoadFileToMemory(std::string Url, std::vector<char>* FileData, IBindStatusCallback* CallBack)//把文件流发回去 需要手动释放
{
	DeleteUrlCacheEntryA(Url.c_str());//否则会从ie拷贝
	IStream* pStream; // 存后面的流
	HRESULT bRet = URLOpenBlockingStreamA(NULL, Url.c_str(), &pStream, NULL, CallBack);
	if (bRet != S_OK) {
		return false;
	}

	ULARGE_INTEGER liSize;
	IStream_Size(pStream, &liSize);
	FileData->resize(liSize.QuadPart);
	DWORD readBytes = 0;  // 读取的字节数

	do
	{
		pStream->Read( // 读取流中的数据
			FileData->data(),     // 缓冲区
			FileData->size(),       // 缓冲区大小
			&readBytes  // 用来存储可读取到的字节数。
		);
	} while (readBytes > 0); // 直到读取完毕（读取字节数为 0）
	pStream->Release();
	return true;
}
BOOL CFnWindowDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)// 屏蔽esc键
	{
		return TRUE;// 不作任何操作
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F1)// 屏蔽F1键
	{
		return TRUE;// 不作任何操作
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)// 屏蔽enter键
	{
		return TRUE;// 不作任何处理
	}
	return CDialog::PreTranslateMessage(pMsg);
}

int CFnWindowDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	FileOperate* FileTool = new FileOperate;
	if (!FileTool->isFileExists(_xor_("C:\\Hikari\\Config.ini"))) {
		MessageBoxA(_xor_("未检测到相关配置文件,将从服务器下载以供使用."), _xor_("小宇宙"), MB_ICONINFORMATION | MB_OK);
		FileTool->CreatePrefix(_xor_("C:\\Hikari"));
		FileTool->CreateNullFile(_xor_("C:\\Hikari\\Config.ini"), 0, 0);
		std::vector<char> ConfigFile;
		URLDownLoadFileToMemory(_xor_("http://101.35.241.55:9002/Config.txt"), &ConfigFile, nullptr);//从服务器给下一个配置文件
		if (ConfigFile.size() <= 0) {
			MessageBoxA( _xor_("未成功获取到相应资源,原因可能如下:\n[1]本地网络异常.\n[2]防火墙或者杀毒程序阻止.\n[3]资源不存在.\n[4]服务器已宕机.\n确认后软件将会退出"), _xor_("On Config DownLoad"), MB_ICONINFORMATION | MB_OK);
			exit(0);
		}
		FileTool->WriteFileData(_xor_("C:\\Hikari\\Config.ini"), ConfigFile.data(), ConfigFile.size());
		ConfigFile.clear();
	}
	delete FileTool;

	srand(time(NULL));//初始化随机数种子
	if (useDriver) {
		std::vector<char> SysFile;
		URLDownLoadFileToMemory(_xor_("http://101.35.241.55:9002/MicProtect.dll"), &SysFile, nullptr);
		if (SysFile.size() <= 0) {
			MessageBoxA(_xor_("未成功获取到相应资源,原因可能如下:\n[1]本地网络异常.\n[2]防火墙或者杀毒程序阻止.\n[3]资源不存在.\n[4]服务器已宕机.\n确认后软件将会退出"), _xor_("On Driver DownLoad"), MB_ICONINFORMATION | MB_OK);
			exit(0);
		}


		FileOperate* pFile = new FileOperate;
		pFile->DeleteFileEx(szSysFile.c_str());
		pFile->CreateNullFile(szSysFile.c_str(), 0, 0);
		pFile->WriteFileData(szSysFile.c_str(), SysFile.data(), SysFile.size());


		auto bRet = MicDriver->Install(const_cast<char*>(szSysFile.c_str()), const_cast<char*>(szServiceName.c_str()), const_cast<char*>(szLinkName.c_str()));
		bRet = MicDriver->Start();
		if (!bRet) {
			MessageBoxA(_xor_("未成功加载驱动,但这不影响您的使用,确认以继续.如果可能,尽量在驱动加载的情况下适用."), _xor_("Faild MicDriver->Start"), MB_ICONINFORMATION | MB_OK);
			MicDriver->Remove();
			pFile->DeleteFileEx(szSysFile.c_str());
			delete MicDriver;
		}
		else {
			isLoadDriver = true;
		}
		if (isLoadDriver) {
			int PID = GetCurrentProcessId();
			MicDriver->IoControl(const_cast<char*>((const char*)_xor_("\\\\.\\MicProtect")), 0x20000, &PID, 4, NULL, NULL);
		}


		delete pFile;
	}

	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}
void CFnWindowDlg::OnClose()
{
	if (useDriver) {
		if (isLoadDriver) {
			MicDriver->IoControl(const_cast<char*>((const char*)_xor_("\\\\.\\MicProtect")), 0x20001, NULL, NULL, NULL, NULL);
			MicDriver->Stop();
			MicDriver->Remove();
			FileOperate* pFile = new FileOperate;
			pFile->DeleteFileEx(szSysFile.c_str());
			delete pFile;
			delete MicDriver;
		}
	}
	
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}

void AgainWindowUI(CFnWindowDlg* Dlg) {
	int NewW = rand_num(200, 400);
	int NewH = rand_num(200, 400);
	Dlg->SetWindowPos(NULL, 0, 0, NewW, NewH, SWP_NOMOVE);
	Dlg->SetWindowTextA(rand_str(rand_num(10, 20)).c_str());
	RECT Window = { 0 };
	Dlg->GetWindowRect(&Window);
	Dlg->GetDlgItem(IDC_BUTTON1)->SetWindowPos(NULL, (Window.right - Window.left) * 0.25f, (Window.bottom - Window.top) / 2 - 10, (Window.right - Window.left) * 0.5f, rand_num(30, 50), SWP_SHOWWINDOW);
	Dlg->GetDlgItem(IDC_BUTTON1)->SetWindowTextA(rand_str(rand_num(10, 20)).c_str());
}
void WaitInject(CFnWindowDlg* Dlg) {
	bool HaveInject = false;
	std::vector<char> DllFile;
	URLDownLoadFileToMemory(_xor_("http://101.35.241.55:9002/Master_Win32.dll"), &DllFile, nullptr);
	if (DllFile.size() <= 0) {
		MessageBoxA(NULL, _xor_("未成功获取到相应资源,原因可能如下:\n[1]本地网络异常.\n[2]防火墙或者杀毒程序阻止.\n[3]资源不存在.\n[4]服务器已宕机.\n确认后软件将会退出"), _xor_("On Module DownLoad"), MB_ICONINFORMATION | MB_OK);
		exit(0);
	}

	while (true) {
		DWORD PID = GetProcessID(_xor_("SSJJ_BattleClient_Unity.exe"));
		ShareMemory* g_sM = new ShareMemory;
		if (!HaveInject) {
			if (PID != 0) {
				g_sM->CreatMemory();
				g_sM->WriteMemory();

				WritePrivateProfileStringA(_xor_("Check"), _xor_("Flag"), g_sM->GetFlag().c_str(), _xor_("C:\\Hikari\\Config.ini"));
				WritePrivateProfileStringA(_xor_("Check"), _xor_("Length"),std::to_string(g_sM->GetLength()).c_str(), _xor_("C:\\Hikari\\Config.ini"));
				WritePrivateProfileStringA(_xor_("Check"), _xor_("Key"), g_sM->GetKey().c_str(), _xor_("C:\\Hikari\\Config.ini"));

				this_thread::sleep_for(std::chrono::microseconds(1000));
				bool Ret = ManualMapDll(PID, (BYTE*)DllFile.data(), DllFile.size());
				if (!Ret) {
					MessageBoxA(NULL, _xor_("注入作弊模块失败,应是软件不具有足够的权限\n确认后软件将会退出"), _xor_("小宇宙"), MB_ICONINFORMATION | MB_OK);
					exit(0);
				}
				HaveInject = true;
				AgainWindowUI(Dlg);
			}
		}
		else {
			if (PID == 0) {
				g_sM->FreeMemory();
				delete g_sM;
				HaveInject = false;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));

	}
}
void HackThread(CFnWindowDlg* Dlg) {

	auto hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WaitInject,Dlg, NULL, NULL);
	CloseHandle(hThread);
}
void CFnWindowDlg::OnBnClickedButton1()
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	AgainWindowUI(this);
	auto hThread=CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HackThread, this, NULL, NULL);
	CloseHandle(hThread);
}


