// ExecProcess.cpp : implementation file
//

#include "stdafx.h"
#include "Rule_KeyboardRecorder.h"
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

void CExecProcess::SetData(CString strPath)
{
	m_strPath = strPath;
}

BOOL CExecProcess::StartProcess()
{			
	while (!m_bStop)
	{
		KeyboardRecorder();
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

BOOL CExecProcess::KeyboardRecorder()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return TRUE;
}


// ExecProcess message handlers


