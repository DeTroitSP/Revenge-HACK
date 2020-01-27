#include "memory.h"
#include "Windows.h"
#include <TlHelp32.h>

Memory::Memory()
{
	handle = NULL;
}

Memory::~Memory()
{
	CloseHandle(handle);
}

DWORD Memory::getProcess(const wchar_t* proc)
{
	HANDLE hProcessId = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	DWORD process;
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);

	do
	{
		if (!wcscmp(pEntry.szExeFile, proc))
		{
			process = pEntry.th32ProcessID;
			CloseHandle(hProcessId);
			handle = OpenProcess(PROCESS_ALL_ACCESS, false, process);
		}

	} while (Process32Next(hProcessId, &pEntry));
	return process;
}

uintptr_t Memory::getModule(DWORD procId, const wchar_t* modName)
{
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procId);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);

	do
	{
		if (!wcscmp(mEntry.szModule, modName))
		{
			CloseHandle(hModule);
			return (DWORD)mEntry.hModule;
		}
	} while (Module32Next(hModule, &mEntry));
	return 0;
}

DWORD Memory::getAddress(DWORD addr, std::vector<DWORD> vect)
{
	for (unsigned int i = 0; i < vect.size(); i++)
	{
		ReadProcessMemory(handle, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += vect[i];
	}
	return addr;
}