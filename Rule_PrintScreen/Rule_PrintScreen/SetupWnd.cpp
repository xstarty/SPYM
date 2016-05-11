// SetupWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Rule_PrintScreen.h"
#include "SetupWnd.h"
#include "Control.h"

// CSetupWnd

IMPLEMENT_DYNAMIC(CSetupWnd, CWnd)

CSetupWnd::CSetupWnd()
{

}

CSetupWnd::~CSetupWnd()
{
}

BEGIN_MESSAGE_MAP(CSetupWnd, CWnd)
	ON_BN_CLICKED(IDC_BTN03, OnBtnClickSavePath)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CSetupWnd message handlers

void CSetupWnd::OnBtnClickSavePath()
{
	CString    strFolderPath;
	BROWSEINFO broInfo = {0};
	TCHAR      szDisName[MAX_PATH] = {0};

	broInfo.hwndOwner = this->m_hWnd;
	broInfo.pidlRoot  = NULL;
	broInfo.pszDisplayName = szDisName;
//	broInfo.lpszTitle = strTitle;
	broInfo.ulFlags   = BIF_NEWDIALOGSTYLE | BIF_DONTGOBELOWDOMAIN | BIF_BROWSEFORCOMPUTER | BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
	broInfo.lpfn      = NULL;	
	broInfo.lParam    = NULL;
//	broInfo.iImage    = IDR_MAINFRAME;

	LPITEMIDLIST pIDList = SHBrowseForFolder(&broInfo);
	if (pIDList != NULL)
	{
		memset(szDisName, 0, sizeof(szDisName));
		SHGetPathFromIDList(pIDList, szDisName);
		strFolderPath = szDisName;
		strFolderPath.Replace(L"\\", L"\\\\");

		if (m_edPath)
			m_edPath->SetWindowText(strFolderPath);
	}
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
		m_btnEnable->Create(strText, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP, CRect(15, 22, 80, 50), this, IDC_BTN02);
		m_btnEnable->SetCheck(FALSE);
	}		

	m_stSec = new CStatic();
	if (m_stSec)
	{
		strText.LoadString(IDS_WND_SEC);
		m_stSec->Create(strText, WS_CHILD | WS_VISIBLE, CRect(100, 15, 255, 30), this, IDC_ST01);
	}

	m_edSec = new CNumEdit();
	if (m_edSec)
	{
		m_edSec->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_RIGHT, CRect(100, 33, 225, 53), this, IDC_ED01);
		m_edSec->SetWindowText(L"10");
	}

	m_stPath = new CStatic();
	if (m_stPath)
	{
		strText.LoadString(IDS_WND_PATH);
		m_stPath->Create(strText, WS_CHILD | WS_VISIBLE, CRect(245, 15, 500, 30), this, IDC_ST02);
	}

	m_edPath = new CEdit();
	if (m_edPath)
	{
		m_edPath->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL, CRect(245, 33, 470, 53), this, IDC_ED02);

		CString strPath(GetDefPath());
		m_edPath->SetWindowText(strPath);
	}

	m_btnPath = new CButton();
	if (m_btnPath)
	{
		strText.LoadString(IDS_WND_PATH2);
		m_btnPath->Create(strText, WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(475, 33, 495, 53), this, IDC_BTN03);
	}

	return TRUE;
}

BOOL CSetupWnd::SaveWndParameter(CString& strParameter)
{
	CString strTemp;
	strParameter.Empty();

	if (!m_edSec || !m_edPath)
		return FALSE;

	strParameter += m_btnEnable->GetCheck() == 1 ? "1" : "0";
	strParameter += ",";

	m_edSec->GetWindowText(strTemp);
	strParameter += strTemp;
	strParameter += ",";

	m_edPath->GetWindowText(strTemp);
	strParameter += strTemp;

	return TRUE;
}

BOOL CSetupWnd::LoadWndParameter(CString& strParameter)
{
	CStringArray arData;

	if (!m_edSec || !m_edPath)
		return FALSE;

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
	m_edSec->SetWindowText(arData.GetAt($VALUE_SEC));

	CString strTemp(arData.GetAt($VALUE_PATH));
	strTemp.Replace(L"\\", L"\\\\");
	m_edPath->SetWindowText(strTemp);


	return TRUE;
}

BOOL CSetupWnd::IsEnable()
{
	return m_btnEnable->GetCheck();
}

int CSetupWnd::GetSec()
{
	CString strTemp;
	m_edSec->GetWindowText(strTemp);

	return _tstoi(strTemp);
}

CString CSetupWnd::GetPath()
{
	CString strTemp;
	m_edPath->GetWindowText(strTemp);

	return strTemp;
}

CString CSetupWnd::GetDefPath()
{
	CString strPath;
	GetModuleFileName(NULL, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	strPath.ReleaseBuffer();
	int nPos = strPath.ReverseFind('\\');
	strPath = strPath.Left(nPos);
	strPath.Replace(L"\\", L"\\\\");

	return strPath;
}