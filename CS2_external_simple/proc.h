#pragma once
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

DWORD GetProcId(TCHAR* procName);

BYTE* GetModBaseAddress(DWORD processId, const wchar_t* moduleName);

BYTE* FindDMAAddy(HANDLE hProcess, BYTE* base_ptr, std::vector<unsigned int> offsets);