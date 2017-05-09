// RecordPlayerDlg.h : header file
//

#if !defined(AFX_RECORDPLAYERDLG_H__CC0A3F73_5598_4BF0_87FE_A4481D6478FD__INCLUDED_)
#define AFX_RECORDPLAYERDLG_H__CC0A3F73_5598_4BF0_87FE_A4481D6478FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRecordPlayerDlg dialog

class CRecordPlayerDlg : public CDialog
{
// Construction
public:
	CRecordPlayerDlg(CWnd* pParent = NULL);	// standard constructor
	void ReplaceString( CString& csString_io, int& nPos_io );

// Dialog Data
	//{{AFX_DATA(CRecordPlayerDlg)
	enum { IDD = IDD_RECORDPLAYER_DIALOG };
	CButton	m_record;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordPlayerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CImageList m_ImageList;

	afx_msg LRESULT OnMyMouseMove(WPARAM, LPARAM param);
	afx_msg LRESULT OnMyMouseLDBClick(WPARAM, LPARAM param);
	afx_msg LRESULT OnMyMouseLButtonUp(WPARAM, LPARAM param);
	afx_msg LRESULT OnMyMouseLButtonDown(WPARAM, LPARAM param);
	afx_msg LRESULT OnMyMouseRButtonUp(WPARAM, LPARAM param);
	afx_msg LRESULT OnMyMouseRButtonDown(WPARAM, LPARAM param);
	afx_msg LRESULT OnKeyStroke(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
	//{{AFX_MSG(CRecordPlayerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnrecord();
	afx_msg void OnBplaybackfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	CString m_csSelFilePath;
	CStdioFile m_FileHandler;
	CStringArray m_csArray;
	int m_nTimeStamp;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDPLAYERDLG_H__CC0A3F73_5598_4BF0_87FE_A4481D6478FD__INCLUDED_)
