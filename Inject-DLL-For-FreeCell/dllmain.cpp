// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <stdio.h>

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory.h>
#include "resource.h"

// Req 1
void APIENTRY changeOffsetThat_move_is_not_allowedTo(char* newText)
{
	OutputDebugStringA("\n**************************************************************");
	OutputDebugStringA("Start of changeOffsetThat_move_is_not_allowedTo(char* newText)\n");

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
	unsigned long newProtectionSetting = PAGE_READWRITE;
	char tempString[100];
	SIZE_T dwSize = 100;

	if (!VirtualProtect((LPVOID)address, dwSize, newProtectionSetting, &originalProtectionSetting))
	{
		MessageBoxA(0, "VirtualProtect failed", "b[0]", 1);
	}
	else
	{
		OutputDebugStringA("\nVirtualProtect change to PAGE_READWRITE successful\n");
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
	sprintf_s(tempString, 100, "newText=%s\n", newText);
	OutputDebugStringA(tempString);

	OutputDebugStringA("Before memcpy_s");
	//memcpy_s(originalTextAddress, strlen(newText), newText, strlen(newText));
	//memcpy_s(originalTextAddress, 20, newText, 26);
	//memcpy_s(originalTextAddress, 2*originalTextLength, newText, 2*strlen(newText));

	
	for (unsigned int i = 0; i < strlen(newText) - 1; i++)
	{
		*(originalTextAddress + 2 * i) = newText[i];
		OutputDebugStringA(originalTextAddress + 2 * i);
	}
	// ********************************************************************************************

	
	// ********************************************************************************************
	// Change protection for the memory pages at that address to what it had been prior to patching
	// ********************************************************************************************
	unsigned long previousProtectionSetting;
	newProtectionSetting = originalProtectionSetting;
	if (!VirtualProtect((LPVOID)address, dwSize, PAGE_READONLY, &previousProtectionSetting))
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



	
	// ********************************************************************************************
	// Make a copy of the modified text located at originalTextAddress
	// ********************************************************************************************
	char modifiedString[originalTextLength];
	OutputDebugStringA("modifiedString=");
	sprintf_s(modifiedString, 18, "%s", originalTextAddress);
	OutputDebugStringA(modifiedString);
	// ********************************************************************************************
	
	OutputDebugStringA("\nEnd of changeOffsetThat_move_is_not_allowedTo(char* newText)");
	OutputDebugStringA("**************************************************************\n");

}


void APIENTRY setGamesWonTo(int numberOfGamesWon)
{

	OutputDebugStringA("\n**************************************************************");
	OutputDebugStringA("Start of setGamesWonTo(int numberOfGamesWon)\n");

	HKEY key;

	if (RegOpenKey(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Applets\\FreeCell"), &key) != ERROR_SUCCESS)
	{
		MessageBoxA(0, "setGamesWon", "Unable to open registry key", 1);
	}
	else
	{
		int decValue = 1000;
		if (RegSetValueEx(key,TEXT("won"), 0, REG_BINARY, (BYTE *)&numberOfGamesWon, sizeof(numberOfGamesWon)) != ERROR_SUCCESS)
		{
			MessageBoxA(0, "setGamesWon()", "Unable to set registry value value_name", 1);
		}
		else
		{
			
		}
	}
	RegCloseKey(key);

	OutputDebugStringA("\nEnd of setGamesWonTo(int numberOfGamesWon)");
	OutputDebugStringA("**************************************************************\n");
}



void APIENTRY nextValidMoveWinsTheGame(bool flag) {


	OutputDebugStringA("\n**************************************************************");
	OutputDebugStringA("Start of nextValidMoveWinsTheGame(bool flag)\n");

	char buffer[100];

	// ********************************************************************************************
	// Let's access _bCheating section of memory ... see if we can read its current value
	// ********************************************************************************************
	DWORD address = 0x01007130;		// location of variable in memory to change
	DWORD* _bCheating = (DWORD *)address;
	OutputDebugStringA("Original _bCheating=");
	sprintf_s(buffer, 26, "%x", *_bCheating);
	OutputDebugStringA(buffer);
	// ********************************************************************************************


	// ********************************************************************************************
	// Change permissions for that DWORD in memory so we can patch it later
	// ********************************************************************************************
	unsigned long originalProtectionSetting;
	unsigned long newProtectionSetting = PAGE_EXECUTE_WRITECOPY;
	//char tempString[100];
	//char* tempString = new char[100];
	SIZE_T dwSize = 2;

	if (!VirtualProtect((LPVOID)address, dwSize, newProtectionSetting, &originalProtectionSetting))
	{
		MessageBoxA(0, "VirtualProtect failed", " nextValidMoveWinsTheGame(bool flag)", 1);
	}
	else
	{
		OutputDebugStringA("\nVirtualProtect change to PAGE_EXECUTE_WRITECOPY successful\n");
		sprintf_s(buffer, 100, "originalProtectionSetting=0x%x\n", originalProtectionSetting);
		OutputDebugStringA(buffer);

		sprintf_s(buffer, 100, "newProtectionSetting=0x%x\n", newProtectionSetting);
		OutputDebugStringA(buffer);
	}
	// ********************************************************************************************


	// ********************************************************************************************
	// Change value to 2
	// ********************************************************************************************
	OutputDebugStringA("\nModifying DWORD");
	*_bCheating = 2;
	// ********************************************************************************************


	// ********************************************************************************************
	// Verify that it was properly set to 2
	// ********************************************************************************************
	//DWORD address = 0x01007130;		// location of variable in memory to change
	//DWORD* _bCheating = (DWORD *)address;
	OutputDebugStringA("Modified _bCheating=");
	sprintf_s(buffer, 26, "%x", *_bCheating);
	OutputDebugStringA(buffer);
	// ********************************************************************************************


	// ********************************************************************************************
	// Revert permissions for that DWORD in memory back to what it had been
	// ********************************************************************************************
	//unsigned long originalProtectionSetting;
	//unsigned long newProtectionSetting = PAGE_EXECUTE_WRITECOPY;
	//char tempString[100];
	//SIZE_T dwSize = 2;
	unsigned long previousProtectionSetting;
	newProtectionSetting = originalProtectionSetting;

	if (!VirtualProtect((LPVOID)address, dwSize, newProtectionSetting, &previousProtectionSetting))
	{
		MessageBoxA(0, "VirtualProtect revert to original failed", " nextValidMoveWinsTheGame(bool flag)", 1);
	}
	else
	{
		OutputDebugStringA("\nVirtualProtect revert to original successful\n");
		sprintf_s(buffer, 100, "originalProtectionSetting=0x%x\n", originalProtectionSetting);
		OutputDebugStringA(buffer);

		sprintf_s(buffer, 100, "newProtectionSetting=0x%x\n", newProtectionSetting);
		OutputDebugStringA(buffer);
	}
	// ********************************************************************************************

	OutputDebugStringA("\nEnd of nextValidMoveWinsTheGame(bool flag)");
	OutputDebugStringA("**************************************************************\n");

}


void APIENTRY patchAcceleratorTable() {

	OutputDebugStringA("\n**************************************************************");
	OutputDebugStringA("Start of patchAcceleratorTable()\n");

	char buffer[100];

	// ********************************************************************************************
	// First, let's view the current accelerator table to ensure we are in the correct place
	// ********************************************************************************************
	// Get handle to existing accelerator table FREEMENU

	HINSTANCE hInstance = GetModuleHandle(NULL);

	HACCEL hAccelFREEMENU;					// handle to FREEMENU accelerator table
	LPCTSTR lpTableNameFREEMENU = L"FREEMENU";
	hAccelFREEMENU = LoadAccelerators(hInstance, lpTableNameFREEMENU);
	sprintf_s(buffer, 100, "\nhAccelFREEMENU=0x%x\n", (int)hAccelFREEMENU);
	OutputDebugStringA(buffer);

	// how many ACCEL entries in the table?
	unsigned int countInitialTableEntriesFREEMENU = 0;
	int cAccelEntriesFREEMENU = 0;
	countInitialTableEntriesFREEMENU = CopyAcceleratorTable(hAccelFREEMENU, 0, cAccelEntriesFREEMENU);
	sprintf_s(buffer, 100, "\ncountInitialTableEntriesFREEMENU=%d\n", countInitialTableEntriesFREEMENU);		// should be 8
	OutputDebugStringA(buffer);


	// ********************************************************************************************
	// Make a new accelerator table based off of FREEMENU
	// ********************************************************************************************
	// Make a copy of FREEMENU first
	ACCEL lpAccelPTCHMENU[9];			// will hold accelerator table from copy
	CopyAcceleratorTable(hAccelFREEMENU, lpAccelPTCHMENU, countInitialTableEntriesFREEMENU);

	// Patch	VK_F10, 114, CONTROL, SHIFT, VIRTKEY
	// to
	//			VK_F6, 114, CONTROL, SHIFT, VIRTKEY
	lpAccelPTCHMENU[7].key = VK_F6;


	// add new entry to copied table
	ACCEL newACELL;
	newACELL.fVirt = FCONTROL | FSHIFT | FVIRTKEY;	newACELL.key = VK_F2; newACELL.cmd = 116;	// new!!!
	lpAccelPTCHMENU[8] = newACELL;

	OutputDebugStringA("\n****************************************************************************");
	OutputDebugStringA("PTCHMENU table entries (don't forget to create accelerator from table!)");
	OutputDebugStringA("****************************************************************************");

	// output new table for debugging purposes
	for (unsigned int a = 0; a < countInitialTableEntriesFREEMENU + 1; a++)
	{
		sprintf_s(buffer, 100, "\na[%d]", a);
		OutputDebugStringA(buffer);
		sprintf_s(buffer, 100, "  fVirt=0x%x", lpAccelPTCHMENU[a].fVirt);
		OutputDebugStringA(buffer);
		sprintf_s(buffer, 100, "  key=0x%x", lpAccelPTCHMENU[a].key);
		OutputDebugStringA(buffer);
		sprintf_s(buffer, 100, "  cmd=%x", lpAccelPTCHMENU[a].cmd);
		OutputDebugStringA(buffer);
	}
	OutputDebugStringA("****************************************************************************");

	// create new accelerator table using lpAccelPTCHMENU array as template
	HACCEL hAccelPTCHMENU;
	hAccelPTCHMENU = CreateAcceleratorTable(lpAccelPTCHMENU, countInitialTableEntriesFREEMENU + 1);
	sprintf_s(buffer, 100, "\nhAccelPTCHMENU=0x%x\n", (int)hAccelPTCHMENU);
	OutputDebugStringA(buffer);
	// ********************************************************************************************


	// ********************************************************************************************
	// Q: Can we destroy FREEMENU accelerator?
	// A: Apparently not
	// ********************************************************************************************
	/*
	bool isDestroyed;
	for (unsigned int a = 0; a < 100; a++)
	{
		isDestroyed = DestroyAcceleratorTable(hAccelFREEMENU);
		sprintf_s(buffer, 100, "[%d] isDestroyed ==%x\n", a, isDestroyed);		// should be 8
		OutputDebugStringA(buffer);
	}
	*/
	// ********************************************************************************************


	// ********************************************************************************************
	// Let's try to use TranslateAccelerator ...
	// ********************************************************************************************
	// .. but we'll need lpMsg first!
	DWORD addressHWND = 0x01008374;
	HWND* _hMainWnd = (HWND *)addressHWND;

	MSG msg;
		
	//LPMSG lpMsg = new LPMSG;
	OutputDebugStringA("Before GetMessage()");
	//BOOL didWeGetlpMsg = GetMessage(&msg, *_hMainWnd, WM_KEYFIRST, WM_KEYLAST);
	//BOOL didWeGetlpMsg = GetMessage(&msg, NULL, 0, 0);
	GetMessage(&msg, NULL, 0, 0);
	//sprintf_s(buffer, 100, "didWeGetlpMsg == %d\n", didWeGetlpMsg);
	//OutputDebugStringA(buffer);

	//sprintf_s(buffer, 100, "lpMsg == %x\n", int(lpMsg));
	//OutputDebugStringA(buffer);
	OutputDebugStringA("After GetMessage()");


	// ********************************************************************************************

	// ********************************************************************************************
	OutputDebugStringA("\nEnd of patchAcceleratorTable()");
	OutputDebugStringA("**************************************************************\n");

}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		// req 1
		changeOffsetThat_move_is_not_allowedTo("Not in this game.        ");

		// req 2
		setGamesWonTo(1000);

		// req 3
		nextValidMoveWinsTheGame(true);

		// req 4 (Make VK_F10 into VK_F6) and req 5 (Ctrl-Shift-F2 wins the game)
		patchAcceleratorTable();


		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}