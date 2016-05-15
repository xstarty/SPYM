// ExecProcess.cpp : implementation file
//

#include "stdafx.h"
#include "Rule_PrintScreen.h"
#include "ExecProcess.h"
#include <atlimage.h>

#include "aviUtil.h"
#include "CxImage\ximage.h"
#pragma comment(lib, "lib/cximage.lib")

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

	//include "aviUtil.h".
	//Call START_AVI("foo.avi"); // you must include the .avi extention.
	//Call ADD_FRAME_FROM_DIB_TO_AVI(yourDIB, "DIB", 30); // the dib is passed in as a HANDLE which can be obtained by loading the image with a Image library. (I used CxImage which is available on this site). Put this call in a while or for loop to add all the images you want to the AVI.
	//Call STOP_AVI(); //this closes the avi and video. 	
	CFileFind finder;
	CString strPath, strFind;
	strFind = m_strPath + L"\\\\";
	strFind += "*.dll";
	
	START_AVI(L"foo.avi");
	
	BOOL bWorking = finder.FindFile(strFind);
		
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		strPath = m_strPath + finder.GetFileName();

		TCHAR* buf = new TCHAR[strPath.GetLength() + 1];
		lstrcpy(buf, strPath);
		CxImage imgJpg(L"", CXIMAGE_FORMAT_JPG);
		delete[]buf;

		ADD_FRAME_FROM_DIB_TO_AVI(imgJpg.GetDIB(), L"DIB", 30);
	}

	STOP_AVI();	

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


