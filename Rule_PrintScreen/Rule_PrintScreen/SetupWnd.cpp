// SetupWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Rule_PrintScreen.h"
#include "SetupWnd.h"


// CSetupWnd

IMPLEMENT_DYNAMIC(CSetupWnd, CWnd)

CSetupWnd::CSetupWnd()
{

}

CSetupWnd::~CSetupWnd()
{
}

BEGIN_MESSAGE_MAP(CSetupWnd, CWnd)
END_MESSAGE_MAP()



// CSetupWnd message handlers


BOOL CSetupWnd::InitWnd(CWnd* pParent)
{
	CRect rect;
	pParent->GetClientRect(&rect);

	m_btnGroup = new CButton();
	if (!m_btnGroup)
		return FALSE;
	else
	{
		CString strTemp;
		strTemp = "555";
		m_btnGroup->Create(L"Select", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, rect.right, 30), this, IDC_STATIC);
	}

	m_btnEnable = new CButton();
	if (!m_btnEnable)
		return FALSE;
	else
	{
		CString strTemp;
		strTemp = "555";
		m_btnEnable->Create(L"",	WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_LEFTTEXT, CRect(30, 22, 37, 10), this, IDC_STATIC);
	}		

	return TRUE;
}