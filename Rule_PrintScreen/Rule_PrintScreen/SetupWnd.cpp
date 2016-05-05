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
	ON_BN_CLICKED(IDC_BTN02, OnBtnClickSavePath)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CSetupWnd message handlers

void CSetupWnd::OnBtnClickSavePath()
{
	/*
	CString    strFolderPath;
	BROWSEINFO broInfo = {0};
	TCHAR       szDisName[MAX_PATH] = {0};

	broInfo.hwndOwner = this->m_hWnd;
	broInfo.pidlRoot  = NULL;
	broInfo.pszDisplayName = szDisName;
	broInfo.lpszTitle = _T("??«O¦s¸ô?");
	broInfo.ulFlags   = BIF_NEWDIALOGSTYLE | BIF_DONTGOBELOWDOMAIN
	| BIF_BROWSEFORCOMPUTER | BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
	broInfo.lpfn      = NULL;
	broInfo.lParam    = NULL;
	broInfo.iImage    = IDR_MAINFRAME;

	LPITEMIDLIST pIDList = SHBrowseForFolder(&broInfo);
	if (pIDList != NULL)
	{
	memset(szDisName, 0, sizeof(szDisName));
	SHGetPathFromIDList(pIDList, szDisName);
	strFolderPath = szDisName;
	}
	*/
}

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

	if (m_stSec)
	{
		delete m_stSec;
		m_stSec = NULL;
	}

	if (m_edSec)
	{
		delete m_edSec;
		m_edSec = NULL;
	}

	if (m_stPath)
	{
		delete m_stPath;
		m_stPath = NULL;
	}

	if (m_edPath)
	{
		delete m_edPath;
		m_edPath = NULL;
	}

	if (m_btnPath)
	{
		delete m_btnPath;
		m_btnPath = NULL;
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

	//////

	m_stSec = new CStatic();
	if (m_stSec)
	{

	}

	m_edSec = new CEdit();
	if (m_edSec)
	{

	}

	m_stPath = new CStatic();
	if (m_stPath)
	{

	}

	m_edPath = new CEdit();
	if (m_edPath)
	{

	}

	m_btnPath = new CButton();
	if (m_btnPath)
	{

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
