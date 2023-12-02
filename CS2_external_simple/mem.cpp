#include "mem.h"

void mem::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
{
  DWORD oldProtect;
  VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
  WriteProcessMemory(hProcess, dst, src, size, nullptr);
  VirtualProtectEx(hProcess, dst, size, oldProtect, &oldProtect);
}
