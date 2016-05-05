#pragma once


// CSetupWnd

class CSetupWnd : public CWnd
{
	DECLARE_DYNAMIC(CSetupWnd)

	enum {
		$VALUE_ENABLE = 0,
		$VALUE_MAX
	};

public:
	CSetupWnd();
	virtual ~CSetupWnd();

protected:
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

public:
	BOOL InitWnd(CWnd *pParent);
	
	BOOL SaveWndParameter(CString& strParameter);
	BOOL LoadWndParameter(CString& strParameter);

private:	
	CButton* m_btnGroup;
	CButton* m_btnEnable;

	CStatic* m_stSec;
	CEdit* m_edSec;

	CStatic* m_stPath;
	CEdit* m_edPath;
	CButton* m_btnPath;
};


