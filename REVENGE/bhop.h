#pragma once

#include "main.h"

/* Cheat vars */
bool bhop_enabled = true;

DWORD localplayer;

#define PLAYER_ON_FLOOR (1 << 0)
/*  */

class Bhop
{
public:
	void refreshENT(Memory mem, HANDLE csgo, DWORD client)
	{
		localplayer = mem.ReadMemory<DWORD>(csgo, client + dwlocalPlayer);
	}

	void process(Memory mem, HANDLE csgo, DWORD client)
	{
		if (bhop_enabled)
		{
			if (localplayer)
			{
				int flags = mem.ReadMemory<int>(csgo, localplayer + fFlags);
				if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && flags & PLAYER_ON_FLOOR) 
				{
					mem.WriteMemory<DWORD>(csgo, client + dwForceJump, 6);
				}
			}
		}
	}
};