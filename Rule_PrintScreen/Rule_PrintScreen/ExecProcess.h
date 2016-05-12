#pragma once

// ExecProcess

class CExecProcess : public CWinThread
{
public:
	CExecProcess();
	virtual ~CExecProcess();

public:
	void SetData(int nSec, CString strPath);
	BOOL StartProcess();
	BOOL StopProcess();

private:
	BOOL PrintScreen();

private:
	int m_nSecond;
	CString m_strPath;
	BOOL m_bStop;
};


