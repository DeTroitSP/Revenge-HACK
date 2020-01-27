#pragma once

#include "main.h"

const char* clantaganimation[13] = {
	"REVENGE",
	"REVENG",
	"REVEN",
	"REVE",
	"REV",
	"RE",
	"R",
	"RE",
	"REV",
	"REVE",
	"REVEN",
	"REVENG",
	"REVENGE",
};


int kek = 0;
int count = 0;

static bool enabled_tag = false;

class CustomTag
{
public:
	void process(Memory mem, DWORD prid, HANDLE process, DWORD client, DWORD engine)
	{
		if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
		{
			enabled_tag = true;
		}

		if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000)
		{
			enabled_tag = false;
			ChangeTag(mem, prid, process, client, engine, "", "hehehe");
		}

		if (enabled_tag)
		{
			AnimatedTag(mem, prid, process, client, engine);
		}
	}

	void AnimatedTag(Memory mem, DWORD prid, HANDLE process, DWORD client, DWORD engine)
	{
		static size_t lastTime = 0;

		if (GetTickCount() > lastTime)
		{
			kek++;
			if (kek > 14) {
				count = count + 1;

				if (count >= 13)
					count = 0;

				char random[255];
				ChangeTag(mem, prid, process, client, engine, clantaganimation[count], "hehe");

				lastTime = GetTickCount() + 500;
			}

			if (kek > 15)
				kek = 0;
		}
	}

	void ChangeTag(Memory mem, DWORD prid, HANDLE process, DWORD client, DWORD engine, const char* tag, const char* name)
	{
		unsigned char Shellcode[] =
			"\x51"                    //push ecx 
			"\x52"                    //push edx 
			"\xB9\x00\x00\x00\x00"    //mov ecx,00000000 { 0 } 
			"\xBA\x00\x00\x00\x00"    //mov edx,00000000 { 0 } 
			"\xE8\x00\x00\x00\x00"    //call 0 
			"\x83\x04\x24\x0A"        //add dword ptr [esp],0A { 10 } 
			"\x68\x00\x00\x00\x00"    //push engine.dll+9AC90 
			"\xC3"                    //ret 
			"\x5A"                    //pop edx 
			"\x59"                    //pop ecx 
			"\xC3"                    //ret 
			"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" //reserve memory[0x10] 
			"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" //reserve memory[0x10] 
			;

		static unsigned int SHELLCODE_SIZE = sizeof(Shellcode) - 0x21;
		unsigned int TAG_SIZE = (strlen(tag) > 15) ? 15 : strlen(tag);
		unsigned int NAME_SIZE = (strlen(name) > 15) ? 15 : strlen(name);
		unsigned int DATA_SIZE = TAG_SIZE + NAME_SIZE + 2;

		LPVOID pShellCodeAdress = VirtualAllocEx(
			process,
			0,
			SHELLCODE_SIZE + DATA_SIZE,
			MEM_COMMIT | MEM_RESERVE,
			PAGE_EXECUTE_READWRITE
		);

		DWORD tagAdress = (DWORD)pShellCodeAdress + SHELLCODE_SIZE;
		DWORD nameAdress = (DWORD)pShellCodeAdress + SHELLCODE_SIZE + TAG_SIZE + 1;
		DWORD fnSetClanAdress = mem.grabSig(process, engine, mem.GetModuleSize(prid, L"engine.dll"), (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15", (char*)"xxxxxxxxx");

		memcpy(Shellcode + 0x3, &tagAdress, sizeof(DWORD));
		memcpy(Shellcode + 0x8, &nameAdress, sizeof(DWORD));
		memcpy(Shellcode + 0x16, &fnSetClanAdress, sizeof(DWORD));
		memcpy(Shellcode + SHELLCODE_SIZE, tag, TAG_SIZE);
		memcpy(Shellcode + SHELLCODE_SIZE + TAG_SIZE + 1, name, NAME_SIZE);

		WriteProcessMemory(process, pShellCodeAdress, Shellcode, SHELLCODE_SIZE + DATA_SIZE, 0);

		HANDLE hThread = CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)pShellCodeAdress, NULL, NULL, NULL);
		WaitForSingleObject(hThread, INFINITE);
		VirtualFreeEx(process, pShellCodeAdress, 0, MEM_RELEASE);
	}
};