#pragma once
#include <Windows.h>

namespace mem {
  void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
}