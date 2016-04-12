
// SPYMDlg.h : header file
//

#pragma once

class CDllCenter;

// CSPYMDlg dialog
class CSPYMDlg : public CDialog
{
// Construction
public:
	CSPYMDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPYM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

private:
	BOOL InitDLLCenter();

private:
	CDllCenter* m_pDllCenter;
};