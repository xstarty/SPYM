#pragma once

#include <utility>
#include <afxtempl.h>

typedef CWnd*	(*fpInitWnd)(CWnd* pParent);
typedef TCHAR*	(*fpGetGUID)(TCHAR* tc);
typedef BOOL	(*fpFinish)();

// CDllCenter

class CDllCenter
{
public:
	CDllCenter(CWnd *pParent);
	virtual ~CDllCenter();

public:
	BOOL LoadDll();
	BOOL FinishDll();

private:
	BOOL GetDllPath(CStringArray& arDllPath);
	HINSTANCE LoadDllLibrary(CString strDllPath);
	void* LoadDllFun(HINSTANCE hr, CString strDllPath, CString strFun);
	BOOL CheckDllUse(TCHAR* tc, HINSTANCE hr);
	BOOL MoveDllWndPos(CWnd* pWnd, int nPos);

private:
	CWnd* m_pParent;	
	CMap<CString, LPCTSTR, std::pair<HINSTANCE, int>, std::pair<HINSTANCE, int>> m_mapDll;
	int m_nDllUseCount;
};


