#include "File.h"
bool  FileOperate::isPrefixExists(string Prefix) {
	if (_access(Prefix.append("\\").c_str(), 0) == -1)	//����ļ��в�����
		return false;
	else
		return true;
}
bool FileOperate::isFileExists(string FileName) {
	return (_access(FileName.c_str(), 0) != -1);
}
bool FileOperate::CreateNullFile(const char* pszName, DWORD dwHigh, DWORD dwLow)//FileSize=dwHigh * 4G + dwLow
{
	if (this->isFileExists(pszName))
		return false;
	DWORD dwResult = 0;
	HANDLE hFile = CreateFileA(pszName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return GetLastError();
	}

	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, dwHigh, dwLow, NULL);
	if (NULL == hFileMap)
	{
		CloseHandle(hFileMap);
		CloseHandle(hFile);
		return false;
	}

	CloseHandle(hFileMap);
	CloseHandle(hFile);

	return true;
}
bool FileOperate::DeleteFileEx(const char* pszName)//FileSize=dwHigh * 4G + dwLow
{

	DeleteFileA(pszName);
	return !this->isFileExists(pszName);
}
bool FileOperate::WriteFileData(const char* pszName, char* Data, int Size) {
	if (!this->isFileExists(pszName))
		return false;

	HANDLE hFile = ::CreateFileA(pszName,     // �����ļ������ơ�
		GENERIC_WRITE | GENERIC_READ | GENERIC_ALL,          // д�Ͷ��ļ���
		0,                      // �������д��
		NULL,                   // ȱʡ��ȫ���ԡ�
		CREATE_ALWAYS,          // ����ļ����ڣ�Ҳ������
		FILE_ATTRIBUTE_NORMAL, // һ����ļ���       
		NULL);                 // ģ���ļ�Ϊ�ա�

	DWORD dwWritenSize = 0;
	BOOL bRet = ::WriteFile(hFile, Data, Size, &dwWritenSize, NULL);
	CloseHandle(hFile);
	return bRet;
}
bool FileOperate::CreatePrefix(std::string Path)
{
	Path.append("\\");
	int len = Path.length();
	char tmpDirPath[256] = { 0 };
	for (int i = 0; i < len; i++)
	{
		tmpDirPath[i] = Path[i];
		if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/')
		{
			if (_access(tmpDirPath, 0) == -1)
			{
				int ret = _mkdir(tmpDirPath);
				if (ret == -1)
					return true;
			}
		}
	}
	return true;
}