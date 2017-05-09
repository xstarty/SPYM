// HookDLL.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "MouseHookDll.h"


#pragma data_seg(".Segment")
HWND hWndServer = NULL;
UINT UWM_MOUSEMOVE;
UINT UWM_MOUSELBUTTONUP;
UINT UWM_MOUSELBUTTONDOWN;
UINT UWM_MOUSERBUTTONUP;
UINT UWM_MOUSERBUTTONDOWN;
UINT UWM_MOUSELDBCLICK;
#pragma data_seg()
#pragma comment(linker, "/section:.Segment,rws")

HINSTANCE hInst;
//HWND hWndServer = NULL;
HHOOK hook;

static LRESULT CALLBACK MouseMsgProc(UINT nCode, WPARAM wParam, LPARAM lParam);

BOOL APIENTRY DllMain( HINSTANCE hModule, 
                       DWORD  dwReason, 
                       LPVOID lpReserved )
{
	switch(	dwReason )
    {
		 case DLL_PROCESS_ATTACH:
			hInst = hModule;
			UWM_MOUSEMOVE = ::RegisterWindowMessage(UWM_MOUSEMOVE_MSG);
			UWM_MOUSELBUTTONUP = ::RegisterWindowMessage(UWM_MOUSELBUTTONUP_MSG);
			UWM_MOUSELBUTTONDOWN = ::RegisterWindowMessage(UWM_MOUSELBUTTONDOWN_MSG);
			UWM_MOUSERBUTTONUP = ::RegisterWindowMessage(UWM_MOUSERBUTTONUP_MSG);
			UWM_MOUSERBUTTONDOWN = ::RegisterWindowMessage(UWM_MOUSERBUTTONDOWN_MSG);
			UWM_MOUSELDBCLICK   = ::RegisterWindowMessage(UWM_MOUSELDBCLICK_MSG);
			break;
		 default:
			 break;
	}
    return TRUE;
}

__declspec(dllexport)  BOOL InstallHook( HWND hWndParent )
{
	if(hWndServer != NULL)
	return FALSE; // already hooked!
	hook = SetWindowsHookEx( WH_GETMESSAGE, (HOOKPROC)MouseMsgProc, 
							 hInst, 0);
	if(hook != NULL)
	{ 
		hWndServer = hWndParent;
		return TRUE;
	} 
	return FALSE;
}

__declspec(dllexport) BOOL UnInstallHook(	HWND hWndParent )
{
	 if(hWndParent != hWndServer || hWndParent == NULL)
	return FALSE;
     BOOL unhooked = UnhookWindowsHookEx(hook);
     if(unhooked)
	hWndServer = NULL;
     return unhooked;
	return TRUE;
}


static LRESULT CALLBACK MouseMsgProc(UINT nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode < 0)
	{ 
		CallNextHookEx(hook, nCode, wParam, lParam);
		return 0;
	}
	LPMSG msg = (LPMSG)lParam;
	switch( msg->message  )
	{
		case WM_LBUTTONDBLCLK:
		SendMessage( hWndServer, UWM_MOUSELDBCLICK, 0 , 0);
		break;
		case WM_MOUSEMOVE:
			SendMessage( hWndServer, UWM_MOUSEMOVE, 0, 0);
		break;
	
		case WM_NCMOUSEMOVE:
			SendMessage( hWndServer, UWM_MOUSEMOVE, 0, 0);
		break;
		case WM_LBUTTONDOWN:
			SendMessage( hWndServer, UWM_MOUSELBUTTONDOWN, 0 , 0 );
		break;
		case WM_NCLBUTTONDOWN:
			SendMessage( hWndServer, UWM_MOUSELBUTTONDOWN, 0 , 0);
		break;
		case WM_LBUTTONUP:
			SendMessage( hWndServer, UWM_MOUSELBUTTONUP, 0 , 0 );
		break;
		case WM_NCLBUTTONUP:
			SendMessage( hWndServer, UWM_MOUSELBUTTONUP, 0 , 0);
		break;
		case WM_RBUTTONDOWN:
			SendMessage( hWndServer, UWM_MOUSERBUTTONDOWN, 0 , 0 );
		break;
		case WM_NCRBUTTONDOWN:
			SendMessage( hWndServer, UWM_MOUSERBUTTONDOWN, 0 , 0);
		break;
		case WM_RBUTTONUP:
			SendMessage( hWndServer, UWM_MOUSERBUTTONUP, 0 , 0 );
		break;
		case WM_NCRBUTTONUP:
			SendMessage( hWndServer, UWM_MOUSERBUTTONUP, 0 , 0);
		break;
		
	default:
		break;
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
} 