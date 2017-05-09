// KeyboardHookDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "KeyboardHookDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma data_seg(".SHARE")
UINT UWN_KEYSTROKE;

HWND hWndServer = NULL;
#pragma data_seg()
#pragma comment(linker, "/section:.SHARE,rws")
HINSTANCE hInst;
HHOOK hook=NULL;

static LRESULT CALLBACK KeyBoardMsgProc( int nCode, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK LowLevelKeyBoardMsgProc( int nCode, WPARAM wParam, LPARAM lParam);


BOOL APIENTRY DllMain( HINSTANCE hModule, 
                       DWORD  dwReason, 
                       LPVOID lpReserved
					 )
{
	switch(	dwReason )
    {
		 case DLL_PROCESS_ATTACH:
			UWN_KEYSTROKE = ::RegisterWindowMessage(UWM_KEYBOARD_MSG);
			hInst = hModule;
			break;
	}
    return TRUE;
}

static LRESULT CALLBACK KeyBoardMsgProc( int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode < 0)
	{ 
		CallNextHookEx(hook, nCode, wParam, lParam);
		return 0;
	}
	if (((DWORD)lParam & 0x40000000) &&(HC_ACTION==nCode))
	{		
		if (( wParam>=VK_LBUTTON )&&( wParam<=VK_OEM_CLEAR ) )
		{
			LPMSG msg = (LPMSG)lParam;
			PostMessage( hWndServer, UWN_KEYSTROKE, wParam, lParam );
		}
		else
		{
			PostMessage( hWndServer, UWN_KEYSTROKE, wParam, lParam );
		}
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
}

static LRESULT CALLBACK LowLevelKeyBoardMsgProc( int nCode, WPARAM wParam, LPARAM lParam)
{
}

__declspec(dllexport)  BOOL InstallKeyBoardHook( HWND hWndParent )
{
	if(hWndServer != NULL)
	return FALSE; // already hooked!
	hook = SetWindowsHookEx( WH_KEYBOARD, (HOOKPROC)KeyBoardMsgProc, 
							 hInst, 0);
	if(hook != NULL)
	{ 
		hWndServer = hWndParent;
		return TRUE;
	} 
	return FALSE;
}

__declspec(dllexport) BOOL UnInstallKeyBoardHook( HWND hWndParent )
{
	 if(hWndParent != hWndServer || hWndParent == NULL)
	return FALSE;
     BOOL unhooked = UnhookWindowsHookEx(hook);
     if(unhooked)
	hWndServer = NULL;
     return unhooked;
	return TRUE;
}




