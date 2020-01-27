#include "main.h"
#include <iostream>
#include <stdio.h>

HANDLE hProcess;

DWORD dwPID = 0;
DWORD dwClient = 0;
DWORD dwEngine = 0;

Netvars* offsets = new Netvars();
Memory mem;

Bhop* bhop = new Bhop();
THPerson* thperson = new THPerson();
CustomTag* customtag = new CustomTag();

GUI* pGUI = new GUI();

int main()
{
	initCSGO();
	doCheats();

	return 0;
}

void initCSGO()
{
	offsets->downloadVars();
	offsets->getNetVars();

	std::wcout << L"Waiting csgo.exe..." << std::endl;
	while (!dwPID)
	{
		Sleep(100);
		dwPID = mem.GetProcessIdByProcessName(L"csgo.exe");
	}

	std::wcout << L"Waiting client_panorama.dll..." << std::endl;
	while (!dwClient)
	{
		Sleep(100);
		dwClient = mem.GetModuleBaseAddress(dwPID, L"client_panorama.dll");
	}

	std::cout << "Waiting engine.dll..." << std::endl;
	do {
		Sleep(100);
		dwEngine = mem.GetModuleBaseAddress(dwPID, L"engine.dll");
	} while (!dwEngine);

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	createGUI();

	system("cls");

	std::wcout << L"Successful injected (/^_^)/" << std::endl;

	while (dwPID && dwClient) { doCheats(); }
}

void createGUI()
{

}

void doCheats()
{
	thperson->process(mem, hProcess, dwClient);

	customtag->process(mem, dwPID, hProcess, dwClient, dwEngine);

	bhop->refreshENT(mem, hProcess, dwClient);
	bhop->process(mem, hProcess, dwClient);
}