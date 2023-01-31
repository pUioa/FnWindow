#pragma once
#ifndef  File_H
#define File_H
#include <Windows.h>
#include <thread>
#include <fstream>
#include <iostream>
#include <direct.h>
#include <io.h>

#include <vector>
#include <string>

using namespace std;
class FileOperate {
private:


public:
	bool isPrefixExists(std::string Prefix);
	bool isFileExists(std::string FileName);
	bool CreateNullFile(const char* pszName, DWORD dwHigh, DWORD dwLow);
	bool DeleteFileEx(const char* pszName);
	bool WriteFileData(const char* pszName, char* Data, int Size);
	bool CreatePrefix(std::string Path);

};
#endif // ! FILE+H
