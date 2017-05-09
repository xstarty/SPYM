// RecordPlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RecordPlayer.h"
#include "RecordPlayerDlg.h"
#include "../MouseHookDll/MouseHookDll.h"
#include "../KeyboardHookDll/KeyboardHookDll.h"
#include "RecPlayBackMgr.h"
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// mouse event.
static UINT UWM_MOUSEDBCLICK = ::RegisterWindowMessage( UWM_MOUSELDBCLICK_MSG );
static UINT UWM_MOUSELBUTTONUP = ::RegisterWindowMessage(UWM_MOUSELBUTTONUP_MSG);;
static UINT UWM_MOUSELBUTTONDOWN = ::RegisterWindowMessage(UWM_MOUSELBUTTONDOWN_MSG);
static UINT UWM_MOUSERBUTTONUP = ::RegisterWindowMessage(UWM_MOUSERBUTTONUP_MSG);
static UINT UWM_MOUSERBUTTONDOWN = ::RegisterWindowMessage(UWM_MOUSERBUTTONDOWN_MSG);
static UINT UWM_MOUSEMOVE = ::RegisterWindowMessage(UWM_MOUSEMOVE_MSG);
// keyboard Stroke
static UINT UWM_KEYBOARDSTROKE = ::RegisterWindowMessage(UWM_KEYBOARD_MSG);

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordPlayerDlg dialog

CRecordPlayerDlg::CRecordPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordPlayerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecordPlayerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRecordPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordPlayerDlg)
	DDX_Control(pDX, IDC_BTNRECORD, m_record);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRecordPlayerDlg, CDialog)
    ON_REGISTERED_MESSAGE(UWM_MOUSEMOVE, OnMyMouseMove)
	ON_REGISTERED_MESSAGE( UWM_MOUSEDBCLICK, OnMyMouseLDBClick)
	ON_REGISTERED_MESSAGE(UWM_MOUSELBUTTONUP, OnMyMouseLButtonUp)
	ON_REGISTERED_MESSAGE( UWM_MOUSELBUTTONDOWN, OnMyMouseLButtonDown)
	ON_REGISTERED_MESSAGE(UWM_MOUSERBUTTONUP, OnMyMouseRButtonUp)
	ON_REGISTERED_MESSAGE( UWM_MOUSERBUTTONDOWN, OnMyMouseRButtonDown)
	ON_REGISTERED_MESSAGE(UWM_KEYBOARDSTROKE, OnKeyStroke )
	//{{AFX_MSG_MAP(CRecordPlayerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNRECORD, OnBtnrecord)
	ON_BN_CLICKED(IDC_BPLAYBACKFILE, OnBplaybackfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordPlayerDlg message handlers

BOOL CRecordPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//HICON hIcon = AfxGetApp()->LoadIcon( IDI_ICONRECORD );
	//HICON hIcon1 = AfxGetApp()->LoadIcon( IDI_ICONPLAY );
	//m_PlayBack.SetIcon( hIcon );
	//m_record.SetIcon( hIcon1 );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRecordPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// Mouse and keyBoard Handling Functions
LRESULT CRecordPlayerDlg::OnMyMouseMove(WPARAM, LPARAM param)
{
	m_nTimeStamp = GetTickCount() - m_nTimeStamp;
	POINT point;
	GetCursorPos(&point);
	UpdateData( 1 );
//	m_x.Format( "x = %d", point.x );
	//m_y.Format( "y = %d", point.y );
	CString csFormatString;

	if( "NC" != (LPCSTR)param)
	{
		csFormatString.Format( "%d M_MOUSEMOVE", 
			point.x, point.y, m_nTimeStamp );
	}
	else
	{
		csFormatString.Format( "%d M_NCMOUSEMOVE", 
			point.x, point.y,m_nTimeStamp );
	}
	m_csArray.Add( csFormatString );
	m_nTimeStamp = GetTickCount();
	UpdateData(0);
	return 1;
} // CHookDlg::OnMyMouseMove


LRESULT CRecordPlayerDlg::OnMyMouseLButtonUp(WPARAM, LPARAM param)
{
	POINT point;
	m_nTimeStamp = GetTickCount() - m_nTimeStamp;
	GetCursorPos(&point);
	//m_listCtrl.AddString( "MOUSE_LBUTTONUP" );
	HANDLE  hWnd = ::WindowFromPoint( point );
	if( hWnd == m_hWnd )
	{
		return 1;
	}
	CString csFormatString;
	if( "NC" != (LPCSTR)param)
	{
		csFormatString.Format( "%d M_LBUTTONUP", point.x, point.y,m_nTimeStamp);
	}
	else
	{
		csFormatString.Format( "%d M_NCLBUTTONUP", point.x, point.y,m_nTimeStamp);
	}
	
	m_csArray.Add( csFormatString );
	m_nTimeStamp = GetTickCount();
	return 1;
}
LRESULT CRecordPlayerDlg::OnMyMouseLButtonDown(WPARAM, LPARAM param)
{
	POINT point;
	m_nTimeStamp = GetTickCount() - m_nTimeStamp;
	GetCursorPos(&point);
	HANDLE  hWnd = ::WindowFromPoint( point );
	if( hWnd == m_hWnd )
	{
		return 1;
	}
	//m_listCtrl.AddString( "MOUSE_LBUTTONDOWN" );
	CString csFormatString;
	if( "NC" != (LPCSTR)param)
	{
		csFormatString.Format( "%d M_LBUTTONDOWN", point.x, point.y,m_nTimeStamp);
	}
	else
	{
		csFormatString.Format( "%d M_LBUTTONDOWN", point.x, point.y,m_nTimeStamp);
	}
	
	m_csArray.Add( csFormatString );
	m_nTimeStamp = GetTickCount();
	return 1;
}
LRESULT CRecordPlayerDlg::OnMyMouseRButtonUp(WPARAM, LPARAM param)
{
	POINT point;
	m_nTimeStamp = GetTickCount() - m_nTimeStamp;
	GetCursorPos(&point);
	HANDLE  hWnd = ::WindowFromPoint( point );
	if( hWnd == m_hWnd )
	{
		return 1;
	}
	CString csFormatString;
	if( "NC" != (LPCSTR)param)
	{
		csFormatString.Format( "%d M_RBUTTONUP", point.x, point.y,m_nTimeStamp);
	}
	else
	{
		csFormatString.Format( "%d M_NCRBUTTONUP", point.x, point.y,m_nTimeStamp);
	}
	m_csArray.Add( csFormatString );
	m_nTimeStamp = GetTickCount();
	return 1;
}

LRESULT CRecordPlayerDlg::OnMyMouseRButtonDown(WPARAM, LPARAM param)
{
	POINT point;
	m_nTimeStamp = GetTickCount() - m_nTimeStamp;
	GetCursorPos(&point);
	//m_listCtrl.AddString( "MOUSE_RBUTTONDOWN" );
	HANDLE  hWnd = ::WindowFromPoint( point );
	if( hWnd == m_hWnd )
	{
		return 1;
	}
	CString csFormatString;
	if( "NC" != (LPCSTR)param)
	{
		csFormatString.Format( "%d M_RBUTTONDOWN", point.x, point.y,m_nTimeStamp);
	}
	else
	{
		csFormatString.Format( "%d M_NCRBUTTONDOWN", point.x, point.y,m_nTimeStamp);
	}

	m_csArray.Add( csFormatString );
	m_nTimeStamp = GetTickCount();
	return 1;
}

LRESULT CRecordPlayerDlg::OnMyMouseLDBClick(WPARAM, LPARAM param)
{
	POINT point;
	m_nTimeStamp = GetTickCount() - m_nTimeStamp;
	GetCursorPos(&point);
	HANDLE  hWnd = ::WindowFromPoint( point );
	if( hWnd == m_hWnd )
	{
		return 1;
	}
	//m_listCtrl.AddString( "MOUSE_DOUBLECLICK" );
	CString csFormatString;
	if( "NC" != (LPCSTR)param)
	{
		csFormatString.Format( "%d M_LBUTTONDBLCLK", point.x, point.y,m_nTimeStamp);
	}
	else
	{
		csFormatString.Format( "%d M_NCLBUTTONDBLCLK", point.x, point.y,m_nTimeStamp);
	}
	
	m_csArray.Add( csFormatString );
	m_nTimeStamp = GetTickCount();
	return 1;
}

void CRecordPlayerDlg::ReplaceString( CString& csString_io, int& nPos_io )
{
	csString_io.SetAt( nPos_io, _T( '1' )); 
}

LRESULT CRecordPlayerDlg::OnKeyStroke(WPARAM wParam, LPARAM lParam)
{
	m_nTimeStamp = GetTickCount() - m_nTimeStamp;
	CString csKeyTyped;
	CString csKeyStatus;
	csKeyStatus = "0??????????";
	if ((wParam==VK_SPACE)||(wParam==VK_RETURN)||
		(wParam>=0x2f ) &&(wParam<=0x100)) 
	{		
// CAPSTATE,LSHIFT,LCTRL, LWINDOW, LALT, RALT, RWIN, MENU,RCTRL, RSHIFT,NUMLCOK
		SHORT nCapital			= GetKeyState( VK_CAPITAL );
		// Left side
//****************************************************************************
		SHORT nVirtKeyLeftShift = GetAsyncKeyState(VK_LSHIFT);
		SHORT nLeftControl      = GetAsyncKeyState( VK_LCONTROL );
		// Hard Coded
		SHORT nLWindows             = GetAsyncKeyState( VK_LWIN );
		SHORT nLAlt             = GetAsyncKeyState( VK_LMENU );		
//****************************************************************************		
		// Right Side
		SHORT nRAlt             = GetAsyncKeyState( VK_RMENU );
		// Hard coded
		SHORT nRWin             = GetAsyncKeyState( VK_RWIN );
		SHORT nMenu             = GetAsyncKeyState( VK_APPS );
		SHORT nRightControl     = GetAsyncKeyState( VK_LCONTROL );
		SHORT nVirtKeyRightShift= GetAsyncKeyState(VK_RSHIFT);
//*****************************************************************************

		// Num Lock
		SHORT nNumLock = GetKeyState( VK_NUMLOCK );

		
		int nCurrentPos = 0;
		if( 0 != nCapital )
		{
			ReplaceString( csKeyStatus, nCurrentPos );
		}
		nCurrentPos = csKeyStatus.Find( SEPERATOR, nCurrentPos );
		nCurrentPos++;
		if( 0 != nVirtKeyLeftShift )
		{
			ReplaceString( csKeyStatus, nCurrentPos );
		}
		nCurrentPos = csKeyStatus.Find( SEPERATOR, nCurrentPos );
		nCurrentPos++;
		if( 0 != nLeftControl )
		{
			ReplaceString( csKeyStatus, nCurrentPos );
		}
		nCurrentPos = csKeyStatus.Find( SEPERATOR, nCurrentPos );
		nCurrentPos++;
		if( 0 != nLWindows )
		{
			ReplaceString( csKeyStatus, nCurrentPos );
		}
		nCurrentPos = csKeyStatus.Find( SEPERATOR, nCurrentPos );
		nCurrentPos++;
		if( 0 != nLAlt )
		{
			ReplaceString( csKeyStatus, nCurrentPos );
		}
		nCurrentPos = csKeyStatus.Find( SEPERATOR, nCurrentPos );
		nCurrentPos++;
		if( 0 != nRAlt )
		{
			ReplaceString( csKeyStatus, nCurrentPos );
		}
		nCurrentPos = csKeyStatus.Find( SEPERATOR, nCurrentPos );
		nCurrentPos++;
		if( 0 != nRWin )
		{
			ReplaceString( csKeyStatus, nCurrentPos );
		}
		nCurrentPos = csKeyStatus.Find( SEPERATOR, nCurrentPos );
		nCurrentPos++;
		if( 0 != nMenu )
		{
			ReplaceString( csKeyStatus, nCurrentPos );
		}
		nCurrentPos = csKeyStatus.Find( SEPERATOR, nCurrentPos );
		nCurrentPos++;
		if( 0 != nRightControl )
		{
			ReplaceString( csKeyStatus, nCurrentPos );
		}
		nCurrentPos = csKeyStatus.Find( SEPERATOR, nCurrentPos );
		nCurrentPos++;
		if( 0 != nVirtKeyRightShift )
		{
			ReplaceString( csKeyStatus, nCurrentPos );
		}
		nCurrentPos = csKeyStatus.Find( SEPERATOR, nCurrentPos );
		nCurrentPos++;
		if( 0 != nNumLock )
		{
			ReplaceString( csKeyStatus, nCurrentPos );
		}
		LPSTR csKey = new TCHAR[sizeof(TCHAR)];
		GetKeyNameText( lParam, csKey, 4 );
		csKeyTyped = csKey;
		csKeyTyped += SEPERATOR;
		csKeyStatus += SEPERATOR;
	}
	CString csKeyFormat;
	CString csWParam;
	csWParam.Format( "%x", wParam);
	CString csTime;
	csTime.Format( "%d", m_nTimeStamp );
	csKeyFormat = "KeyBD:";
	csKeyFormat += csKeyTyped;
	csKeyFormat += csKeyStatus;
	csKeyFormat += csWParam;
	csKeyFormat += csTime;
	csKeyFormat += "\n";
    m_csArray.Add( csKeyFormat );
	m_nTimeStamp = GetTickCount();
	return 1;
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRecordPlayerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRecordPlayerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CRecordPlayerDlg::OnBtnrecord() 
{	
	CString csText;
	m_record.GetWindowText( csText );
	if( _T( "Record" ) == csText )
	{

		if( FALSE == m_csSelFilePath.IsEmpty())
		{
			m_FileHandler.Open( m_csSelFilePath, 
							 CFile::modeCreate | CFile::modeWrite );
			BOOL bReturn = InstallHook( m_hWnd );
			//  Keyboard hook
			HINSTANCE hinstDLL; 
			typedef BOOL (CALLBACK *inshook)( HWND ); 
			inshook instkbhook;
			hinstDLL = LoadLibrary((LPCTSTR)"KeyboardHookDll.dll"); 
			instkbhook = (inshook)GetProcAddress(hinstDLL, "InstallKeyBoardHook"); 
			instkbhook( m_hWnd );
			m_nTimeStamp = GetTickCount();
			m_record.SetWindowText( _T( "Stop" ));
		}
		else
		{
			CFileDialog FileDlg( FALSE, ".txt", "", 
			                 OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,  
				             "Text Documents (*.txt)"  );
			if( IDOK == FileDlg.DoModal())
			{
				m_csSelFilePath = FileDlg.GetPathName();
				m_FileHandler.Open( m_csSelFilePath, 
							 CFile::modeCreate | CFile::modeWrite );
				
			}
			BOOL bReturn = InstallHook( m_hWnd );
			//  Keyboard hook
			HINSTANCE hinstDLL; 
			typedef BOOL (CALLBACK *inshook)( HWND ); 
			inshook instkbhook;
			hinstDLL = LoadLibrary((LPCTSTR)"KeyboardHookDll.dll"); 
			instkbhook = (inshook)GetProcAddress(hinstDLL, "InstallKeyBoardHook"); 
			instkbhook( m_hWnd );
			m_nTimeStamp = GetTickCount();
			m_record.SetWindowText( _T( "Stop" ));
		}
	}
	else
	{
		
		if( FALSE == m_csSelFilePath.IsEmpty())
		{
			UnInstallHook(	m_hWnd );
			int nSize = m_csArray.GetSize();
			CString csLineContent;
			for( int nID= 0; nID<nSize;nID++ )
			{ 
				csLineContent = m_csArray.GetAt( nID ) + L"\n";
				m_FileHandler.WriteString( csLineContent );
			}
			m_FileHandler.Close();
			m_csArray.RemoveAll();
			m_csSelFilePath.Empty();

		}
		m_record.SetWindowText( _T( "Record" ));
	}
}

void CRecordPlayerDlg::OnBplaybackfile() 
{
	CFileDialog FileDlg( TRUE, "*.txt", "*.txt", 
		                 OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,  
				         "Text Documents (*.txt)" );
	if( IDOK == FileDlg.DoModal())
	{
		ShowWindow( SW_MINIMIZE );
		RecPlayBackMgr Mgr;
		m_csSelFilePath = FileDlg.GetPathName();
		Mgr.PlayFile( m_csSelFilePath );
	}
}



