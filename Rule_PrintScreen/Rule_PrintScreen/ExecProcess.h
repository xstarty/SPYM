#pragma once


// ExecProcess

class CExecProcess : public CWnd
{
public:
	CExecProcess();
	virtual ~CExecProcess();

protected:
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

public:
	void SetData(int nSec, CString strPath);
	BOOL StartProcess();
	BOOL StopProcess();

private:
	BOOL PrintScreen();

private:
	int m_nSecond;
	CString m_strPath;
};


