// CDllCenter.cpp : implementation file
//

#include "stdafx.h"
#include "DllCenter.h"
#include "Utility.h"

#include "json/json.h"
#pragma comment(lib, "lib/lib_json.lib")

#define $WndHieghtPos 30
#define $SaveDataFile "\\DllSI"

// CDllCenter

CDllCenter::CDllCenter(CWnd *pParent)
{
	m_pParent = pParent;
	m_nDllUseCount = 0;
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
		fpInitWnd fpWnd = (fpInitWnd)LoadDllFun(hr, arDllPath.GetAt(i), _T("InitWnd"));
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
	std::pair<HINSTANCE, CString> Info;
	POSITION pos = m_mapDll.GetStartPosition();
	while (pos != NULL)
	{
		int nIndex = -1;
		m_mapDll.GetNextAssoc(pos, nIndex, Info);
		{
			CString strTemp;
			fpFinish fpFin = (fpFinish)LoadDllFun(Info.first, strTemp, _T("FinishWnd"));
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
			fpSaveDll fpSave = (fpSaveDll)LoadDllFun(Info.first, strTemp, _T("SaveWndParameter"));
			if (fpSave == NULL)
				return FALSE;

			CString strGUID = GetGUID(Info.first, strTemp);

			TCHAR* tc = new TCHAR[512];

			if (!fpSave(tc))
				return FALSE;						

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
			fpLoadDll fpLoad = (fpLoadDll)LoadDllFun(Info.first, strTemp, _T("LoadWndParameter"));
			if (fpLoad == NULL)
				return FALSE;

			CString strValue;
			if (m_mapData.Lookup(Info.second, strValue))
			{
				TCHAR* tc = new TCHAR[512];
				lstrcpy(tc, strValue);

				if (!fpLoad(tc))
					return FALSE;

				delete[] tc;
			}			
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

		setlocale(LC_CTYPE, ("chs"));           // 解决 Unicode 亂碼
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
	fpGetGUID fpGUID = (fpGetGUID)LoadDllFun(hr, strDllPath, _T("GetGUID"));
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

	pWnd->MoveWindow(0, $WndHieghtPos * (m_nDllUseCount - 1), rect.right, rect.bottom, TRUE);

	return FALSE;
}


