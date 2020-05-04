#include "proc.h"

using namespace std;

DWORD GetProcId(const wchar_t* procName) {
	DWORD ProcId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 ProcEntry;
		ProcEntry.dwSize = sizeof(ProcEntry);

		if (Process32First(hSnap, &ProcEntry)) {
			 do {
				 if (!_wcsicmp(ProcEntry.szExeFile, procName)) {
					 ProcId = ProcEntry.th32ProcessID;
					 break;
				 }

			 } while (Process32Next(hSnap, &ProcEntry));
		}
	}

	CloseHandle(hSnap);
	return ProcId;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {
	uintptr_t ModBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ModEntry;
		ModEntry.dwSize = sizeof(ModEntry);

		if (Module32First(hSnap, &ModEntry)) {
			do {
				if (!_wcsicmp(ModEntry.szModule, modName)) {
					ModBaseAddr = (uintptr_t)ModEntry.modBaseAddr;
					break;
				}

			} while (Module32Next(hSnap, &ModEntry));
		}

	}

	CloseHandle(hSnap);
	return ModBaseAddr;
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, vector<unsigned int> offsets) {
	uintptr_t addr = ptr;

	for (unsigned int i = 0; i < offsets.size(); ++i) {
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}

	return addr;
}