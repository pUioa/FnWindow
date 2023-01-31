#include "../pch.h"
#include "ShareMemory.h"
DWORD ShareMemory::rand_num()
{
	//5~20
	return 5 + rand() % 16;
}
std::string  ShareMemory::rand_str(const int len)
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

bool  ShareMemory::CreatMemory() {
	this->Flag = this->rand_str(this->rand_num());
	this->hMemory = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, NULL, 1024, this->Flag.c_str());
	return this->hMemory != NULL;
}
bool  ShareMemory::WriteMemory(){
	this->Key= this->rand_str(this->rand_num());
	LPVOID lpBase = MapViewOfFile(this->hMemory, FILE_MAP_WRITE, NULL, NULL, this->Key.length());
	memcpy(lpBase, this->Key.data(), this->Key.length());

	UnmapViewOfFile(lpBase);
	return true;
}
bool  ShareMemory::FreeMemory() {
	if (this->hMemory == NULL)
		return true;
	CloseHandle(this->hMemory);
	this->hMemory = NULL;
	return true;
}
std::string ShareMemory::GetFlag() {
	return this->Flag;
}
std::string ShareMemory::GetKey() {
	return this->Key;
}
int ShareMemory::GetLength() {
	return this->Key.length();
}