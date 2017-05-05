#pragma once

#include <utility>
#include <afxtempl.h>
#include "Resource.h"

typedef CWnd*	(*fpInitWnd)(CWnd* pParent);
typedef BOOL	(*fpPreTranslateMessage)(void *pParam);
typedef TCHAR*	(*fpGetGUID)(TCHAR* tc);
typedef BOOL	(*fpFinish)();
typedef TCHAR*	(*fpSaveDll)(TCHAR* tc);
typedef TCHAR*	(*fpLoadDll)(TCHAR* tc);
typedef BOOL	(*fpExec)(CWnd* pParent);
typedef BOOL	(*fpExecStop)();

// CDllCenter

class CDllCenter
{
	enum
	{
		$PrintScreen = IDS_GUID_START,
		$Dll_Max
	};

public:
	CDllCenter(CWnd *pParent);
	virtual ~CDllCenter();

public:
	BOOL LoadDll();
	BOOL FinishDll();

	BOOL SaveDllParameter();
	BOOL LoadDllParameter();

	BOOL PreTranslateMessage(void *pParam);

private:	
	BOOL SaveFile(CString strData);
	BOOL LoadFile(CString &strData);	
	CString GetPath(BOOL bSavePath);

	CString GetGUID(HINSTANCE hr, CString strDllPath);
	BOOL GetDllPath(CStringArray& arDllPath);
	HINSTANCE LoadDllLibrary(CString strDllPath);
	void* LoadDllFun(HINSTANCE hr, CString strDllPath, CString strFun);
	BOOL CheckDllUse(CString strGUID, HINSTANCE hr);
	BOOL MoveDllWndPos(CWnd* pWnd, int nPos);

public:
	BOOL StartThread();
	BOOL StopThread();		
	BOOL SuspendThread();
	BOOL ResumeThread();

private:
	BOOL ExecThreadImp(fpExec fpExecDll);

private:
	CWnd* m_pParent;	
	CMap<int, int, std::pair<HINSTANCE, CString>, std::pair<HINSTANCE, CString>> m_mapDll;
	int m_nDllUseCount;
	
	CPtrArray m_arThread;
	CCriticalSection m_csStop;
	BOOL m_bThreadStop;
};


