// Rule_PringScreen.h : main header file for the Rule_PringScreen DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// CRule_PringScreenApp
// See Rule_PringScreen.cpp for the implementation of this class
//

class CSetupWnd;
class CRule_PringScreenApp : public CWinApp
{
public:
	CRule_PringScreenApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

public:
	CWnd* InitWnd(CWnd* pParent);
	BOOL FinishWnd();
	
	TCHAR* GetGUID(TCHAR* tc);
	
	TCHAR* SaveWndParameter(TCHAR* tc);
	BOOL LoadWndParameter(TCHAR* tc);

private:
	CSetupWnd* m_pSetupWnd;	
};