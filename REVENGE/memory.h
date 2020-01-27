#pragma once

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <iterator>

class Memory
{
public:
	template <typename datatype>
	datatype ReadMemory(HANDLE hProcess, DWORD address)
	{
		datatype buffer;
		ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(datatype), NULL);
		return buffer;
	}

	template <typename datatype>
	void WriteMemory(HANDLE hProcess, DWORD address, datatype value)
	{
		WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(value), NULL);
	}

	DWORD GetProcessIdByProcessName(const wchar_t* processName)
	{
		DWORD pID = NULL;
		HANDLE ss = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (ss != INVALID_HANDLE_VALUE)
		{
			PROCESSENTRY32 pe;
			pe.dwSize = sizeof(PROCESSENTRY32);
			do
			{
				if (!wcscmp(pe.szExeFile, processName))
				{
					pID = pe.th32ProcessID;
				}
			} while (Process32Next(ss, &pe));
			CloseHandle(ss);
		}
		return pID;
	}

	DWORD GetModuleBaseAddress(DWORD pID, const wchar_t* moduleName)
	{
		DWORD ModuleBaseAddress = NULL;

		HANDLE ss = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pID);
		if (ss != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 me;
			me.dwSize = sizeof(MODULEENTRY32);

			if (Module32First(ss, &me))
			{
				do
				{
					if (!wcscmp(me.szModule, moduleName))
					{
						ModuleBaseAddress = (DWORD)me.modBaseAddr;
						break;
					}
				} while (Module32Next(ss, &me));
			}
			CloseHandle(ss);
		}
		return ModuleBaseAddress;
	}

	MODULEENTRY32 GetModule(DWORD pID, const wchar_t* moduleName)
	{
		DWORD ModuleBaseAddress = NULL;
		MODULEENTRY32 me;
		HANDLE ss = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pID);
		if (ss != INVALID_HANDLE_VALUE)
		{
			me.dwSize = sizeof(MODULEENTRY32);
			if (Module32First(ss, &me))
			{
				do
				{
					if (!wcscmp(me.szModule, moduleName))
					{
						ModuleBaseAddress = (DWORD)me.modBaseAddr;
						break;
					}
				} while (Module32Next(ss, &me));
			}
			CloseHandle(ss);
		}
		return me;
	}

	DWORD GetModuleSize(DWORD pID, const wchar_t* moduleName)
	{
		HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pID);
		MODULEENTRY32 mEntry;
		mEntry.dwSize = sizeof(mEntry);

		do
			if (!wcscmp(mEntry.szModule, moduleName))
			{
				CloseHandle(hModule);
				return (DWORD)mEntry.modBaseSize;
			}
		while (Module32Next(hModule, &mEntry));
		return 0;
	}

	bool DataCompare(BYTE* data, BYTE* sign, char* mask)
	{
		for (; *mask; mask++, sign++, data++)
		{
			if (*mask == 'x' && *data != *sign)
			{
				return false;
			}
		}
		return true;
	}

	DWORD grabSig(HANDLE hPROC, DWORD base, DWORD size, BYTE* sign, char* mask)
	{
		MEMORY_BASIC_INFORMATION mbi = { 0 };
		DWORD offset = 0;
		while (offset < size)
		{
			VirtualQueryEx(hPROC, (LPCVOID)(base + offset), &mbi, sizeof(MEMORY_BASIC_INFORMATION));
			if (mbi.State != MEM_FREE)
			{
				BYTE* buffer = new BYTE[mbi.RegionSize];
				ReadProcessMemory(hPROC, mbi.BaseAddress, buffer, mbi.RegionSize, NULL);
				for (int i = 0; i < mbi.RegionSize; i++)
				{
					if (DataCompare(buffer + i, sign, mask))
					{
						delete[] buffer;
						return (DWORD)mbi.BaseAddress + i;
					}
				}

				delete[] buffer;
			}
			offset += mbi.RegionSize;
		}
		return 0;
	}
};