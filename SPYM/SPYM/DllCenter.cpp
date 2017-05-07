// CDllCenter.cpp : implementation file
//

#include "stdafx.h"
#include "DllCenter.h"
#include "Utility.h"

#include "json/json.h"
#pragma comment(lib, "lib_json.lib") 
 
#define $WndHieghtPos 80
#define $SaveDataFile "\\DllSI"

#define $INITWND _T("InitWnd")
#define $PRETRNASMSG _T("PreTranlateMsg")
#define $FINISHWND _T("FinishWnd")
#define $SAVEWNDPARAMETER _T("SaveWndParameter")
#define $LOADWNDPARAMETER _T("LoadWndParameter")
#define $GETGUID _T("GetGUID")
#define $EXEC _T("Exec")
#define $EXECSTOP _T("ExecStop")

// CDllCenter

CDllCenter::CDllCenter(CWnd *pParent)
{
	m_pParent = pParent;
	m_nDllUseCount = 0;		
	m_bThreadStop = FALSE;
}

CDllCenter::~CDllCenter()
{
}

BOOL CDllCenter::LoadDll()
{
	Json::Value jsRoot;

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
		CString strGUID;
		strGUID = GetGUID(hr, arDllPath.GetAt(i));
		
		BOOL bCheck = CheckDllUse(strGUID, hr);

		if (!bCheck)
			return FALSE;

		// Init DLL
		fpInitWnd fpWnd = (fpInitWnd)LoadDllFun(hr, arDllPath.GetAt(i), $INITWND);
		if (fpWnd == NULL)
			return FALSE;

		CWnd *pWnd = fpWnd(m_pParent);
		if (!pWnd)
			return FALSE;

		MoveDllWndPos(pWnd, m_nDllUseCount);
	}

	LoadDllParameter();
	
	return TRUE;
}

BOOL CDllCenter::FinishDll()
{	
	StopThread();

	std::pair<HINSTANCE, CString> Info;
	POSITION pos = m_mapDll.GetStartPosition();
	while (pos != NULL)
	{
		int nIndex = -1;
		m_mapDll.GetNextAssoc(pos, nIndex, Info);
		{
			CString strTemp;
			fpFinish fpFin = (fpFinish)LoadDllFun(Info.first, strTemp, $FINISHWND);
			if (fpFin == NULL)
				return FALSE;

			if (!fpFin())
				return FALSE;

			FreeLibrary(Info.first);

			Info.first = NULL;
		}
	}
	
	return TRUE;
}

BOOL CDllCenter::SaveDllParameter()
{
	// Use Json format
	CString strSaveData;
	strSaveData = "{\"DllData\":";
	
	CString strItem;
	strItem = "[{\"GUID\":\"%s\",\"Value\":\"%s\"}],";

	std::pair<HINSTANCE, CString> Info;
	POSITION pos = m_mapDll.GetStartPosition();
	while (pos != NULL)
	{
		int nIndex = -1;
		m_mapDll.GetNextAssoc(pos, nIndex, Info);
		{
			CString strTemp;
			fpSaveDll fpSave = (fpSaveDll)LoadDllFun(Info.first, strTemp, $SAVEWNDPARAMETER);
			if (fpSave == NULL)
				return FALSE;

			CString strGUID = GetGUID(Info.first, strTemp);

			TCHAR* tc = new TCHAR[512];

			if (!fpSave(tc))
			{
				delete[] tc;
				return FALSE;
			}
				

			CString strData(tc);
			delete[] tc;		

			if (strGUID.IsEmpty() || strData.IsEmpty())
				continue;

			strTemp.Format(strItem, strGUID, strData);
			strSaveData += strTemp;
		}
	}

	strSaveData.TrimRight(',');	
	strSaveData += "}";

	BOOL bSave = SaveFile(strSaveData);

	return TRUE;
}

BOOL CDllCenter::LoadDllParameter()
{
	// Paser Json data
	CString strSaveData;
	LoadFile(strSaveData);
	std::string stdData(CW2A(strSaveData.GetString()));

	CMap<CString, LPCTSTR, CString, LPCTSTR> m_mapData;
	if (!strSaveData.IsEmpty())
	{
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(stdData, root))
		{
			Json::Value arrayObj = root["DllData"];
			for (unsigned int i = 0; i < arrayObj.size(); i++)
			{
				std::string strGUID;
				std::string strValue;
				if (arrayObj[i].isMember("GUID"))
					strGUID = arrayObj[i]["GUID"].asString();

				if (arrayObj[i].isMember("Value"))
					strValue = arrayObj[i]["Value"].asString();

				m_mapData.SetAt(CA2T(strGUID.c_str()), CA2T(strValue.c_str()));
			}
		}
	}

	//////////////////////////////////////////////
	
	std::pair<HINSTANCE, CString> Info;
	POSITION pos = m_mapDll.GetStartPosition();
	while (pos != NULL)
	{
		int nIndex = -1;
		m_mapDll.GetNextAssoc(pos, nIndex, Info);
		{
			CString strTemp;
			fpLoadDll fpLoad = (fpLoadDll)LoadDllFun(Info.first, strTemp, $LOADWNDPARAMETER);
			if (fpLoad == NULL)
				return FALSE;

			CString strValue;
			if (m_mapData.Lookup(Info.second, strValue))
			{
				TCHAR* tc = new TCHAR[512];
				lstrcpy(tc, strValue);

				if (!fpLoad(tc))
				{
					delete[] tc;
					return FALSE;
				}					

				delete[] tc;
			}			
		}
	}

	return TRUE;
}

BOOL CDllCenter::PreTranslateMessage(void * pParam)
{
	std::pair<HINSTANCE, CString> Info;
	POSITION pos = m_mapDll.GetStartPosition();
	while (pos != NULL)
	{
		int nIndex = -1;
		m_mapDll.GetNextAssoc(pos, nIndex, Info);
		{
			CString strTemp;
			fpPreTranslateMessage fpPreTrans = (fpPreTranslateMessage)LoadDllFun(Info.first, strTemp, $PRETRNASMSG);
			if (fpPreTrans == NULL)
				return FALSE;

			BOOL bErr = fpPreTrans(pParam);
			if (!bErr)
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CDllCenter::SaveFile(CString strData)
{
	CStdioFile myFile;
	CString strWriteItem;
	if (myFile.Open(GetPath(TRUE), CFile::modeReadWrite | CFile::modeCreate) == NULL)
		return FALSE;
	else
	{
		myFile.WriteString(strData);
		myFile.Close();
	}
	
	return TRUE;
}

BOOL CDllCenter::LoadFile(CString &strData)
{
	CStdioFile myFile;
	CString strPathListIterm;
	if (myFile.Open(GetPath(TRUE), CFile::modeRead) == NULL)
		return FALSE;
	else
	{
		strData.Empty();

		setlocale(LC_CTYPE, ("chs"));           // Unicode solve garbled
		ULONGLONG dwLength = myFile.GetLength();
		while (myFile.ReadString(strPathListIterm))    
		{
			strData.Append(strPathListIterm);  
		}

		myFile.Close();
	}	

	return TRUE;
}

CString CDllCenter::GetPath(BOOL bSavePath)
{
	CString strPath;
	GetModuleFileName(NULL, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	strPath.ReleaseBuffer();
	int nPos = strPath.ReverseFind('\\');
	strPath = strPath.Left(nPos);
	strPath.Replace(L"\\", L"\\\\");

	if (bSavePath)
		strPath += $SaveDataFile;

	return strPath;
}

CString CDllCenter::GetGUID(HINSTANCE hr, CString strDllPath)
{
	fpGetGUID fpGUID = (fpGetGUID)LoadDllFun(hr, strDllPath, $GETGUID);
	if (fpGUID == NULL)
		return FALSE;

	CString strText;
	strText.LoadString(IDS_GUID_START);
	TCHAR* tc = new TCHAR[strText.GetLength() + 1];

	tc = fpGUID(tc);

	CString strGUID(tc);

	delete[] tc;	

	return strGUID;
}

BOOL CDllCenter::GetDllPath(CStringArray& arDllPath)
{
	arDllPath.RemoveAll();

	CString strPath(GetPath(FALSE));

	CFileFind finder;
	BOOL bWorking = finder.FindFile(strPath + _T("\\\\" + "Rule*.dll"));
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
		int nErr = GetLastError();

		CString strError, strText;
		strText.LoadString(IDS_LOAD_DLL_FAIL);
		strError.Format(strText, strDllPath, nErr);

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
		int nErr = GetLastError();

		CString strError, strText;
		strText.LoadString(IDS_LOAD_FUN_FAIL);
		strError.Format(strText, strDllPath, strFun, nErr);

		Utility::ShowMessageBox(m_pParent->GetSafeHwnd(), strError, MB_OK);

		return NULL;
	}

	return fp;
}

BOOL CDllCenter::CheckDllUse(CString strGUID, HINSTANCE hr)
{	
	CString strText;

	for (int i = IDS_GUID_START; i < IDS_GUID_END; i++)
	{
		strText.LoadString(i);

		if (strText.IsEmpty())
			return FALSE;

		if (strText.CompareNoCase(strGUID) != 0)
			continue;

		m_mapDll.SetAt(i, std::make_pair(hr, strGUID));

		m_nDllUseCount++;
		
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

	pWnd->MoveWindow(10, 10 + $WndHieghtPos * (nPos - 1), rect.right, $WndHieghtPos, TRUE);

	return FALSE;
}

BOOL CDllCenter::StartThread()
{
	StopThread();

	CSingleLock csStop(&m_csStop);
	csStop.Lock();
	{
		m_bThreadStop = FALSE;
	}		
	csStop.Unlock();

	std::pair<HINSTANCE, CString> Info;
	POSITION pos = m_mapDll.GetStartPosition();
	while (pos != NULL)
	{
		int nIndex = -1;
		m_mapDll.GetNextAssoc(pos, nIndex, Info);
		{
			CString strTemp;
			fpExec fpExecDll = (fpExec)LoadDllFun(Info.first, strTemp, $EXEC);			
			if (fpExecDll == NULL)
				continue;

			ExecThreadImp(fpExecDll);
		}
	}

	return TRUE;
}

BOOL CDllCenter::StopThread()
{	
	CSingleLock csStop(&m_csStop);
	csStop.Lock();
	{
		m_bThreadStop = TRUE;
	}
	csStop.Unlock();

	std::pair<HINSTANCE, CString> Info;
	POSITION pos = m_mapDll.GetStartPosition();
	while (pos != NULL)
	{
		int nIndex = -1;
		m_mapDll.GetNextAssoc(pos, nIndex, Info);
		{
			CString strTemp;
			fpExecStop fpStop = (fpExecStop)LoadDllFun(Info.first, strTemp, $EXECSTOP);
			if (fpStop == NULL)
				return FALSE;

			if (!fpStop())
				return FALSE;
		}
	}

	CWinThread *pThread = NULL;
	for (int i = 0; i < m_arThread.GetCount(); ++i)
	{
		pThread = (CWinThread*)m_arThread.GetAt(i);
		if (pThread == NULL)
			return TRUE;

		if (pThread->m_hThread == NULL)
			return TRUE;

		DWORD dwCode = ::WaitForSingleObject(pThread->m_hThread, 5000);
		if (dwCode != WAIT_OBJECT_0)
		{
			DWORD dwExitCode;
			if (GetExitCodeThread(pThread->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
			{
				::TerminateThread(pThread->m_hThread, -1);
			}
		}

		pThread->m_hThread = NULL;
		delete pThread;
		pThread = NULL;
	}

	m_arThread.RemoveAll();

	return TRUE;
}

BOOL CDllCenter::SuspendThread()
{
	CWinThread *pThread = NULL;
	for (int i = 0; i < m_arThread.GetCount(); ++i)
	{
		pThread = (CWinThread*)m_arThread.GetAt(i);
		if (pThread == NULL)
			continue;

		pThread->SuspendThread();
	}

	return TRUE;
}

BOOL CDllCenter::ResumeThread()
{
	CWinThread *pThread = NULL;
	for (int i = 0; i < m_arThread.GetCount(); ++i)
	{
		pThread = (CWinThread*)m_arThread.GetAt(i);
		if (pThread == NULL)
			continue;

		pThread->ResumeThread();
	}

	return TRUE;	
}

BOOL CDllCenter::ExecThreadImp(fpExec fpExecDll)
{
	CWinThread* pThread = AfxBeginThread((AFX_THREADPROC)fpExecDll, (LPVOID)m_pParent, THREAD_PRIORITY_NORMAL, CREATE_SUSPENDED, 0, NULL);
	if (NULL == pThread)
		return FALSE;
	
	pThread->m_bAutoDelete = FALSE;
	pThread->ResumeThread();

	m_arThread.Add(pThread);

	return TRUE;
}