#include "File.h"
bool  FileOperate::isPrefixExists(string Prefix) {
	if (_access(Prefix.append("\\").c_str(), 0) == -1)	//如果文件夹不存在
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

	HANDLE hFile = ::CreateFileA(pszName,     // 创建文件的名称。
		GENERIC_WRITE | GENERIC_READ | GENERIC_ALL,          // 写和读文件。
		0,                      // 不共享读写。
		NULL,                   // 缺省安全属性。
		CREATE_ALWAYS,          // 如果文件存在，也创建。
		FILE_ATTRIBUTE_NORMAL, // 一般的文件。       
		NULL);                 // 模板文件为空。

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