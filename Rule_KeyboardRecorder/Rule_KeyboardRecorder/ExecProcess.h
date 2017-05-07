#pragma once

// ExecProcess

class CExecProcess : public CWinThread
{
public:
	CExecProcess();
	virtual ~CExecProcess();

public:
	void SetData(CString strPath);
	BOOL StartProcess();
	BOOL StopProcess();

private:
	BOOL KeyboardRecorder();

private:
	CString m_strPath;
	BOOL m_bStop;
};


