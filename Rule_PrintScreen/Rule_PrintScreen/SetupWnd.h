#pragma once


// CSetupWnd

class CSetupWnd : public CWnd
{
	DECLARE_DYNAMIC(CSetupWnd)

public:
	CSetupWnd();
	virtual ~CSetupWnd();

protected:
	DECLARE_MESSAGE_MAP()

public:
	BOOL InitWnd(CWnd *pParent);

private:	
	CButton* m_btnGroup;
	CButton* m_btnEnable;
};


