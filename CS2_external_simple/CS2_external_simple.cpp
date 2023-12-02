#include <iostream>
#include "proc.h"
#include "mem.h"

struct playerOffsets {
  UINT playerOffset = 0x16CF6c0;

  unsigned int  health = 0x32c;
  unsigned int  team = 0x3bf;
  unsigned int  bSpotted = 0x163c;
  unsigned int  isPlayer = 0x164; // value will be ff800 (idk why, just works)
}offsets;
  
  
  BOOL isValidPlayer(HANDLE hProcess, BYTE* baseAddress, DWORD arrayOffset) {
  BYTE* pNextPlayer = FindDMAAddy(hProcess, (baseAddress + (offsets.playerOffset + arrayOffset)), { offsets.isPlayer }); // check if offset 164 == ff800 (idk why)
  DWORD playerEntData = 0;
  ReadProcessMemory(hProcess, pNextPlayer, &playerEntData, sizeof(playerEntData), NULL);
  return playerEntData == 0xff800;
}

int main()
{
  std::cout << "Waiting for CS2..." << std::endl;

  DWORD processId;
  while (!(processId = GetProcId((wchar_t*)TEXT("cs2.exe"))));

  std::cout << "Process found (PID: " << processId << ")" << std::endl;

  BYTE* moduleBaseAddress;
  while (!(moduleBaseAddress = GetModBaseAddress(processId, TEXT("client.dll"))));
  std::cout << "Module base address found. (0x" << std::hex << (uintptr_t)moduleBaseAddress << ")" << std::endl;

  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, processId);

  BOOL bMinimap = FALSE;
  DWORD dwExit;

  while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE) {
    // HACKCONTROLS
    if (GetAsyncKeyState(VK_DELETE) & 1) {
      CloseHandle(hProcess);
      return 0;
    }

    if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
      bMinimap = !bMinimap;
      std::cout << "Radar hack " << (bMinimap ? "ON" : "OFF") << std::endl;
    }
    


    // HACKLOOP
    if (bMinimap) {
      BYTE* player = 0;
      const int trueConst = 1;
      BOOL odd = false;
      for (unsigned int playerIndex = 0; playerIndex < 30; playerIndex++) {
        if (isValidPlayer(hProcess, moduleBaseAddress, playerIndex * 0x8)) {
          std::cout << std::hex << (uintptr_t)(moduleBaseAddress + (offsets.playerOffset + playerIndex * 0x8)) << std::endl;
          BYTE* pPlayerMinimapVisible = FindDMAAddy(hProcess, (moduleBaseAddress + (offsets.playerOffset + playerIndex * 0x8)), { offsets.bSpotted });
          WriteProcessMemory(hProcess, pPlayerMinimapVisible, (BYTE*)&trueConst, sizeof(trueConst), nullptr);
        }
      }
    }

    Sleep(10);
  }

  CloseHandle(hProcess);
}

