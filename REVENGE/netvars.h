#pragma once
#include <tchar.h>
#include <stdio.h>

#include "libs/JSON.h"
#include "libs/stdafx.h"

#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

/* NETVARS */
int dwForceJump;
int dwlocalPlayer;
int dwClientState;

int dwSetClanTag;

int m_aimPunchAngle;
int m_bIsDefusing;

int fFlags;

int m_flC4Blow;

int m_hObserverTarget;
int m_iObserverMode;
int m_iFOV;

int m_thirdPersonViewAngles;
int m_vecViewOffset;

int m_szCustomName;
/* = *** = */

class Netvars
{
public:

	void downloadVars()
	{
		HRESULT hr = URLDownloadToFile(NULL, _T("https://raw.githubusercontent.com/frk1/hazedumper/master/csgo.json"), _T("csgo.json"), 0, NULL);
		if (hr != S_OK) {
			MessageBoxA(NULL, "Error occoured on connection to github!", "Error", 0);
			exit(0);
		}
	}

	void getNetVars()
	{
		FILE* infile = fopen("csgo.json", "r");
		fseek(infile, 0, SEEK_END);
		long filesize = ftell(infile);
		char* buf = new char[filesize + 1];
		fseek(infile, 0, SEEK_SET);
		fread(buf, 1, filesize, infile);
		fclose(infile);
		buf[filesize] = '\0';
		std::stringstream ss;
		ss.str(buf);
		delete[] buf;
		Json::Value netvars;
		ss >> netvars;

		dwForceJump = netvars["signatures"]["dwForceJump"].asInt();
		dwlocalPlayer = netvars["signatures"]["dwLocalPlayer"].asInt();
		dwClientState = netvars["signatures"]["dwClientState"].asInt();

		dwSetClanTag = netvars["signatures"]["dwSetClanTag"].asInt();

		m_aimPunchAngle = netvars["signatures"]["m_aimPunchAngle"].asInt();
		m_bIsDefusing = netvars["signatures"]["m_bIsDefusing"].asInt();

		m_szCustomName = netvars["netvars"]["m_szCustomName"].asInt();

		m_iObserverMode = netvars["netvars"]["m_iObserverMode"].asInt();
		m_hObserverTarget = netvars["netvars"]["m_hObserverTarget"].asInt();
		m_vecViewOffset = netvars["netvars"]["m_vecViewOffset"].asInt();

		m_flC4Blow = netvars["netvars"]["m_flC4Blow"].asInt();

		m_thirdPersonViewAngles = netvars["netvars"]["m_thirdPersonViewAngles"].asInt();

		m_iFOV = netvars["netvars"]["m_iFOV"].asInt();

		fFlags = netvars["netvars"]["m_fFlags"].asInt();
	}
};
