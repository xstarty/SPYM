// CDllCenter.cpp : implementation file
//

#include "stdafx.h"
#include "DllCenter.h"
#include "Utility.h"
#include "Resource.h"

#define $WndHieghtPos 30

// CDllCenter

CDllCenter::CDllCenter(CWnd *pParent)
{
	m_pParent = pParent;
	m_nDllUseCount = 0;
}

CDllCenter::~CDllCenter()
{
}

BOOL CDllCenter::LoadDLL()
{
	m_mapDll.RemoveAll();

	CStringArray arDllPath;
	if (!GetDllPath(arDllPath))
		return FALSE;

	int nCount = arDllPath.GetCount();
	for (int i = 0; i < nCount; i++)
	{	
		// Load DLL
		HINSTANCE hr = LoadDllLibrary(arDllPath.GetAt(i));
		if (hr == NULL)
			return FALSE;

		
		// Check DLL
		fpGetGUID fpGUID = (fpGetGUID)LoadDllFun(hr, arDllPath.GetAt(i), _T("GetGUID"));
		if (fpGUID == NULL)
			return FALSE;		

		CString strText;
		strText.LoadString(IDS_GUID_START);
		TCHAR* tc = new TCHAR[strText.GetLength() + 1];

		tc = fpGUID(tc);
		
		BOOL bCheck = CheckDllUse(tc, hr);
		delete[] tc;

		if (!bCheck)
			return FALSE;

		// Init DLL
		fpInitWnd fpWnd = (fpInitWnd)LoadDllFun(hr, arDllPath.GetAt(i), _T("InitWnd"));
		if (fpWnd == NULL)
			return FALSE;

		CWnd *pWnd = fpWnd(m_pParent);
		if (!pWnd)
			return FALSE;

		MoveDllWndPos(pWnd, m_nDllUseCount);
	}

	return TRUE;
}

BOOL CDllCenter::GetDllPath(CStringArray& arDllPath)
{
	arDllPath.RemoveAll();

	CString strPath;
	GetModuleFileName(NULL, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	strPath.ReleaseBuffer();
	int nPos = strPath.ReverseFind('\\');
	strPath = strPath.Left(nPos);

	CFileFind finder;
	BOOL bWorking = finder.FindFile(strPath + _T("\\\\" + "*.dll"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		arDllPath.Add(finder.GetFileName());
	}

	return TRUE;
}

HINSTANCE CDllCenter::LoadDllLibrary(CString strDllPath)
{
	LPCWSTR lpwText = strDllPath.AllocSysString();

	HINSTANCE handler = LoadLibrary(lpwText);
	if (handler == NULL)
	{
		CString strError, strText;
		strText.LoadString(IDS_LOAD_DLL_FAIL);
		strError.Format(strText, strDllPath);

		Utility::ShowMessageBox(m_pParent->GetSafeHwnd(), strError, MB_OK);

		return NULL;
	}

	return handler;
}

void* CDllCenter::LoadDllFun(HINSTANCE hr, CString strDllPath, CString strFun)
{	
	CStringA strFunA(strFun);
	LPCSTR lpcText = strFunA;
	FARPROC fp = GetProcAddress(hr, lpcText);
	if (fp == NULL)
	{
		CString strError, strText;
		strText.LoadString(IDS_LOAD_FUN_FAIL);
		strError.Format(strText, strDllPath, strFun);

		Utility::ShowMessageBox(m_pParent->GetSafeHwnd(), strError, MB_OK);

		return NULL;
	}

	return fp;
}

BOOL CDllCenter::CheckDllUse(TCHAR * tc, HINSTANCE hr)
{
	CString strGUID(tc);
	CString strText;

	for (int i = IDS_GUID_START; i < IDS_GUID_END; i++)
	{
		strText.LoadString(i);

		if (strText.IsEmpty())
			return FALSE;

		if (strText.CompareNoCase(strGUID) != 0)
			continue;

		m_mapDll.SetAt(strGUID, std::make_pair(hr, ++m_nDllUseCount));
		
		return TRUE;
	}

	return FALSE;
}

BOOL CDllCenter::MoveDllWndPos(CWnd * pWnd, int nPos)
{	
	if (!pWnd)
		return FALSE;

	CRect rect;
	m_pParent->GetClientRect(&rect);

	pWnd->MoveWindow(0, $WndHieghtPos * (m_nDllUseCount - 1), rect.right, rect.bottom, TRUE);
	

	return FALSE;
}


