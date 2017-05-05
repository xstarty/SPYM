// ExecProcess.cpp : implementation file
//

#include "stdafx.h"
#include "Rule_PrintScreen.h"
#include "ExecProcess.h"
#include <atlimage.h>

// ExecProcess

CExecProcess::CExecProcess()
{
	m_bStop = FALSE;
}

CExecProcess::~CExecProcess()
{	
}

void CExecProcess::SetData(int nSec, CString strPath)
{
	m_nSecond = nSec;
	m_strPath = strPath;
}

BOOL CExecProcess::StartProcess()
{			
	while (!m_bStop)
	{
		PrintScreen();

		Sleep(m_nSecond * 1000);
	}
	
	return  TRUE;
}

BOOL CExecProcess::StopProcess()
{
	if (m_bStop == TRUE)
		return FALSE;

	m_bStop = TRUE;

	ConvertJpegToAvi();

	return TRUE;
}

BOOL CExecProcess::ConvertJpegToAvi()
{
	CString strPath;
	strPath = m_strPath + L"\\\\";

	CTime tm = CTime::GetCurrentTime();
	CString strDate = tm.Format("%Y%m%d_%H%M%S");

	CString strCmd;
#ifdef DEBUG
	strCmd = L"Debug_JpegToAvi.bat " + strDate;
#else
	strCmd = L"JpegToAvi.bat " + strDate;
#endif // DEBUG
	
	strPath += strCmd;
	system(CT2CA(strPath));

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


