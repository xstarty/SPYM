// ExecProcess.cpp : implementation file
//

#include "stdafx.h"
#include "Rule_PrintScreen.h"
#include "ExecProcess.h"
#include <atlimage.h>

// ExecProcess

#define CALLBACK_ID 1
#define CALLBACK_FREQUENCY 1000

BEGIN_MESSAGE_MAP(CExecProcess, CWnd)
	//{{AFX_MSG_MAP(CExecWnd)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CExecProcess::CExecProcess()
{

}

CExecProcess::~CExecProcess()
{	
}

void CExecProcess::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
		case CALLBACK_ID:
		{
			PrintScreen();

			return;
		}
	}

	CWnd::OnTimer(nIDEvent);
	
}

void CExecProcess::SetData(int nSec, CString strPath)
{
	m_nSecond = nSec;
	m_strPath = strPath;
}

BOOL CExecProcess::StartProcess()
{	
	SetTimer(CALLBACK_ID, CALLBACK_FREQUENCY, NULL);
	
	
	return  TRUE;
}

BOOL CExecProcess::StopProcess()
{
	KillTimer(CALLBACK_ID);

	return TRUE;
}

BOOL CExecProcess::PrintScreen()
{
	// create a screen and a memory device context
	HDC hDCScreen = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	HDC hDCMem = ::CreateCompatibleDC(hDCScreen);

	// create a compatible bitmap and select it in the memory DC
	CRect lprc;
	::SetRect(lprc, ::GetSystemMetrics(SM_XVIRTUALSCREEN), ::GetSystemMetrics(SM_YVIRTUALSCREEN), ::GetSystemMetrics(SM_CXVIRTUALSCREEN), ::GetSystemMetrics(SM_CYVIRTUALSCREEN));
	HBITMAP hBitmap = ::CreateCompatibleBitmap(hDCScreen, lprc.Width(), lprc.Height());
	HBITMAP hBmpOld = (HBITMAP)::SelectObject(hDCMem, hBitmap);

	// bit-blit from screen to memory device context
	// note: CAPTUREBLT flag is required to capture layered windows
	DWORD dwRop = SRCCOPY | CAPTUREBLT;
	BOOL bRet = ::BitBlt(hDCMem, lprc.left, lprc.top, lprc.Width(), lprc.Height(), hDCScreen, 0, 0, dwRop);

	// attach bitmap handle to this object	
	CImage imgData;
	imgData.Attach(hBitmap);

	CString strFull = m_strPath;

	CTime tm = CTime::GetCurrentTime();
	CString strDate = tm.Format("\\%Y%m%d_%H%M%S");

	strFull += strDate;
	strFull += ".jpg";

	HRESULT hr = imgData.Save(strFull);
	// restore the memory DC and perform cleanup
	::SelectObject(hDCMem, hBmpOld);
	::DeleteDC(hDCMem);
	::DeleteDC(hDCScreen);

	if (FAILED(hr))
		return  FALSE;

	return TRUE;
}

// ExecProcess message handlers


