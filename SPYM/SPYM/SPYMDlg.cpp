
// SPYMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SPYM.h"
#include "SPYMDlg.h"
#include "afxdialogex.h"
#include "DllCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSPYMDlg dialog


CSPYMDlg::CSPYMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SPYM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nStatus = $STATUS_STOP;
}

void CSPYMDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_BUTTON_START, m_bStart);
	CDialog::DoDataExchange(pDX);	
}

BEGIN_MESSAGE_MAP(CSPYMDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSPYMDlg message handlers

BOOL CSPYMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//ShowWindow(SW_MINIMIZE);

	// Init Caption
	CString strText;
	strText.LoadString(IDS_CAPTION);
	SetWindowText(strText);

	BOOL bInit = TRUE;
	bInit &= InitDLLCenter();
	
	return bInit;  // return TRUE  unless you set the focus to a control
}

void CSPYMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSPYMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

afx_msg void CSPYMDlg::OnStart()
{
	CString strText;
	if (m_nStatus == $STATUS_STOP)	// Stop -> Start
	{
		if (m_pDllCenter)
		{
			m_pDllCenter->SaveDllParameter();

			if (!m_pDllCenter->StartThread())
				return;				
		}

		m_nStatus = $STATUS_START;
		strText.LoadString(IDS_STOP);
	}
	else if (m_nStatus == $STATUS_START)
	{		
		if (m_pDllCenter)
		{
			if (!m_pDllCenter->StopThread())
				return;
		}
		
		m_nStatus = $STATUS_STOP;
		strText.LoadString(IDS_START);
	}

	m_bStart.SetWindowText(strText);
}

void CSPYMDlg::OnDestroy()
/*---------------------------------------------------------------------------*/
{
	if (m_pDllCenter)
	{
		m_pDllCenter->FinishDll();

		delete m_pDllCenter;
		m_pDllCenter = NULL;
	}
	CDialog::OnDestroy();
}

BOOL CSPYMDlg::InitDLLCenter()
{
	m_pDllCenter = new CDllCenter(this);
	if (!m_pDllCenter)
		return FALSE;

	m_pDllCenter->LoadDll();
	
	return TRUE;
}
