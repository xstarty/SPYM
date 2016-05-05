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
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CSetupWnd message handlers

void CSetupWnd::OnDestroy()
{
	if (m_btnGroup)
	{ 
		delete m_btnGroup;
		m_btnGroup = NULL;
	}

	if (m_btnEnable)
	{
		delete m_btnEnable;
		m_btnEnable = NULL;
	}

	CWnd::OnDestroy();
}

BOOL CSetupWnd::InitWnd(CWnd* pParent)
{
	CRect rect;
	pParent->GetClientRect(&rect);

	CString strText;

	m_btnGroup = new CButton();
	if (!m_btnGroup)
		return FALSE;
	else
	{
		strText.LoadString(IDS_WND_GROUP);
		m_btnGroup->Create(strText, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, rect.right, 60), this, IDC_BTN01);
	}

	m_btnEnable = new CButton();
	if (!m_btnEnable)
		return FALSE;
	else
	{
		strText.LoadString(IDS_WND_ENABLE);
		m_btnEnable->Create(strText, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, CRect(15, 22, 80, 50), this, IDC_BTN02);
	}		

	return TRUE;
}

BOOL CSetupWnd::SaveWndParameter(CString& strParameter)
{
	strParameter.Empty();

	strParameter += m_btnEnable->GetCheck() == 1 ? "1" : "0";

	return TRUE;
}

BOOL CSetupWnd::LoadWndParameter(CString& strParameter)
{
	CStringArray arData;

	CString sToken = _T("");
	int i = 0;
	while (AfxExtractSubString(sToken, strParameter, i, ','))
	{
		arData.Add(sToken);
		i++;
	}

	if (arData.GetCount() != $VALUE_MAX)
		return FALSE;
		
	m_btnEnable->SetCheck(_tstoi(arData.GetAt($VALUE_ENABLE)));

	return TRUE;
}
