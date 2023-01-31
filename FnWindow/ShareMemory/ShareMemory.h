#pragma once
#ifndef _ShareMemory_
#define _ShareMemory_
#include <Windows.h>
#include <string>
class ShareMemory {
private:
	HANDLE hMemory=NULL;
	std::string Flag = "";
	std::string Key = "";
	DWORD rand_num();
	std::string rand_str(const int len);
public:
	bool CreatMemory();
	bool WriteMemory();
	bool FreeMemory();
	std::string GetFlag();
	std::string GetKey();
	int GetLength();
};
#endif // !ShareMemory
