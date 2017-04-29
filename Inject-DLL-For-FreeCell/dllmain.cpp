// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <stdio.h>

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory.h>

// Req 1
void APIENTRY changeOffsetThat_move_is_not_allowedTo(char* newText)
{
	DWORD address = 0x01010C04;		// location of string in memory to change

	const unsigned int originalTextLength = 26;
	const char originalTextExpected[] = "That move is not allowed.";

	char* originalTextAddress;
	char* modifiedTextAddress;
	originalTextAddress = (char *)address;
	modifiedTextAddress = (char *)address;

	// ********************************************************************************************
	// First let's make a copy of the original text located at originalTextAddress
	// ********************************************************************************************
	char oldString[originalTextLength];
	char* b[originalTextLength];

	for (unsigned int i = 0; i < originalTextLength-1; i++)
	{
		b[i] = originalTextAddress + 2 * i;
	}
	b[25] = "\0";
	OutputDebugStringA("\noldString=");
	sprintf_s(oldString, 26, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8], b[9], b[10], b[11], b[12], b[13], b[14], b[15], b[16], b[17], b[18], b[19], b[20], b[21], b[22], b[23], b[24], b[25]);
	OutputDebugStringA(oldString);
	// ********************************************************************************************


	// ********************************************************************************************
	// Change protection for the memory pages at that address so we can modify the text
	// ********************************************************************************************
	unsigned long originalProtectionSetting;
	unsigned long newProtectionSetting = PAGE_EXECUTE_WRITECOPY;
	char tempString[100];
	SIZE_T dwSize = 80;

	if (!VirtualProtect((LPVOID)address, dwSize, newProtectionSetting, &originalProtectionSetting))
	{
		MessageBoxA(0, "VirtualProtect failed", "b[0]", 1);
	}
	else
	{
		OutputDebugStringA("\nVirtualProtect change to PAGE_EXECUTE_WRITECOPY successful\n");
		sprintf_s(tempString, 100, "originalProtectionSetting=0x%x\n", originalProtectionSetting);
		OutputDebugStringA(tempString);

		sprintf_s(tempString, 100, "newProtectionSetting=0x%x\n", newProtectionSetting);
		OutputDebugStringA(tempString);
	}
	// ********************************************************************************************


	// ********************************************************************************************
	// Change contents of the memory pages to use the modified text
	// ********************************************************************************************
	OutputDebugStringA("\nModifying string");
	memcpy_s(originalTextAddress, strlen(newText), newText, strlen(newText));
	// ********************************************************************************************


	// ********************************************************************************************
	// Make a copy of the modifiedl text located at originalTextAddress
	// ********************************************************************************************
	char modifiedString[originalTextLength];
	OutputDebugStringA("modifiedString=");
	sprintf_s(modifiedString, 26, "%s", originalTextAddress);
	OutputDebugStringA(modifiedString);
	// ********************************************************************************************


	// ********************************************************************************************
	// Change protection for the memory pages at that address to what it had been prior to patching
	// ********************************************************************************************
	unsigned long previousProtectionSetting;
	newProtectionSetting = originalProtectionSetting;
	if (!VirtualProtect((LPVOID)address, dwSize, newProtectionSetting, &previousProtectionSetting))
	{
		MessageBoxA(0, "VirtualProtect revert to original failed", "b[0]", 1);
	}
	else
	{
		OutputDebugStringA("\nVirtualProtect revert to original successful\n");
		sprintf_s(tempString, 100, "Setting prior to reverting to original=0x%x\n", previousProtectionSetting);
		OutputDebugStringA(tempString);

		sprintf_s(tempString, 100, "The new ProtectionSetting is now=0x%x\n", newProtectionSetting);
		OutputDebugStringA(tempString);
		OutputDebugStringA("\n");

	}
	// ********************************************************************************************

	
	//OutputDebugStringA("Lab06: End of changeOffsetThat_move_is_not_allowedTo(char* newValue)\n");

}


void APIENTRY setGamesWonTo(int numberOfGamesWon)
{

	//log("setGamesWonTo: Start\n");
	HKEY key;

	if (RegOpenKey(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Applets\\FreeCell"), &key) != ERROR_SUCCESS)
	{
		//log("setGamesWon: Unable to open registry key");
		MessageBoxA(0, "setGamesWon", "Unable to open registry key", 1);
	}
	else
	{
		//log("setGamesWon: Able to open registry key\n");
		int decValue = 1000;
		//gamesWon
		if (RegSetValueEx(key,TEXT("won"), 0, REG_BINARY, (BYTE *)&numberOfGamesWon, sizeof(numberOfGamesWon)) != ERROR_SUCCESS)
		{
			MessageBoxA(0, "setGamesWon()", "Unable to set registry value value_name", 1);
			//log("setGamesWon: Unable to set registry value value_name\n");
		}
		else
		{
			//log("setGamesWon: Able to set registry value value_name\n");
		}
	}
	RegCloseKey(key);

	//log("setGamesWon: End\n");
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//MessageBoxA(0, "Pause", "Pause", 1);

		
		// req 1
		//changeOffsetThat_move_is_not_allowedTo("Not in this game.");
		changeOffsetThat_move_is_not_allowedTo("Not in this game.        ");

		// req 2
		setGamesWonTo(1000);

		// req 3

		// req 4
		// Make VK_F10 into VK_F6
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

