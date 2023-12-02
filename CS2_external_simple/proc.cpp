#include "proc.h"

DWORD GetProcId(TCHAR* procName)
{
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  DWORD processId = 0; 

  if (hSnapshot != INVALID_HANDLE_VALUE) {
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(processEntry);

    if (Process32First(hSnapshot, &processEntry)) {
      do {
        if (!_wcsicmp(procName, processEntry.szExeFile)) {

          processId = processEntry.th32ProcessID;
          break;
        }
      } while (Process32Next(hSnapshot, &processEntry));
    }

  }

  CloseHandle(hSnapshot);
  return processId;
}

BYTE* GetModBaseAddress(DWORD processId, const wchar_t* moduleName)
{
  HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);
  BYTE* baseAddress = 0;


  if (hSnap != INVALID_HANDLE_VALUE) {
    MODULEENTRY32 modEntry;
    modEntry.dwSize = sizeof(modEntry);

    if (Module32First(hSnap, &modEntry)) {
      do {
        if (!_wcsicmp(moduleName, modEntry.szModule)) {
          baseAddress = modEntry.modBaseAddr;
        }
      } while (Module32Next(hSnap, &modEntry));
    }

  }

  return baseAddress;
}

BYTE* FindDMAAddy(HANDLE hProcess, BYTE* base_ptr, std::vector<unsigned int> offsets)
{
  BYTE* ptr = base_ptr;

  for (unsigned int i = 0; i < offsets.size(); i++) {
    ReadProcessMemory(hProcess, ptr, &ptr, sizeof(ptr), 0);
    ptr += offsets[i];
  }

  return ptr;
}
