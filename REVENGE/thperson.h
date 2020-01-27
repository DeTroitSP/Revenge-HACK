#pragma once

#include "main.h"

/* Cheat vars */
bool third_person = false;
/*  */

class THPerson
{
public:
	void process(Memory mem, HANDLE csgo, DWORD client)
	{
		if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
		{
			if (!third_person)
			{
				third_person = true;
				mem.WriteMemory<int>(csgo, localplayer + m_hObserverTarget, 0);
				mem.WriteMemory<int>(csgo, localplayer + m_iObserverMode, 1);
			}

			if (third_person) 
			{ 
				third_person = false; 
			}
		}
		else
		{
			if (third_person)
			{
				mem.WriteMemory<int>(csgo, localplayer + m_hObserverTarget, 0);
				mem.WriteMemory<int>(csgo, localplayer + m_iObserverMode, 1);
			}

			if (!third_person)
			{
				mem.WriteMemory<int>(csgo, localplayer + m_hObserverTarget, -1);
				mem.WriteMemory<int>(csgo, localplayer + m_iObserverMode, 0);
			}
		}
		Sleep(1);	
	}
};