// Rule_PringScreen.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Rule_PrintScreen.h"
#include "SetupWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// The one and only CRule_PringScreenApp object
CRule_PringScreenApp theApp;

extern "C" __declspec(dllexport)
CWnd* InitWnd(CWnd* pParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return theApp.InitWnd(pParent);
}

extern "C" __declspec(dllexport)
BOOL FinishWnd()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return theApp.FinishWnd();
}

extern "C" __declspec(dllexport)
TCHAR* GetGUID(TCHAR* tc)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return theApp.GetGUID(tc);
}

// CRule_PringScreenApp

BEGIN_MESSAGE_MAP(CRule_PringScreenApp, CWinApp)
END_MESSAGE_MAP()


// CRule_PringScreenApp construction

CRule_PringScreenApp::CRule_PringScreenApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// CRule_PringScreenApp initialization

BOOL CRule_PringScreenApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

CWnd* CRule_PringScreenApp::InitWnd(CWnd* pParent)
{
	if (!pParent)
		return NULL;

	m_pSetupWnd = new CSetupWnd();
	if (!m_pSetupWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), pParent, IDC_SETUPWND))
		return NULL;

	m_pSetupWnd->InitWnd(pParent);

	return m_pSetupWnd;
}

BOOL CRule_PringScreenApp::FinishWnd()
{
	if (m_pSetupWnd)
	{
		m_pSetupWnd->DestroyWindow();
		delete m_pSetupWnd;
		m_pSetupWnd = NULL;
	}

	return TRUE;
}

TCHAR* CRule_PringScreenApp::GetGUID(TCHAR* tc)
{
	CString strText;
	strText.LoadString(IDD_GUID);
	lstrcpy(tc, strText);

	return tc;
}
