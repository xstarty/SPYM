// RecordPlayer.h : main header file for the RECORDPLAYER application
//

#if !defined(AFX_RECORDPLAYER_H__A0C4FE2D_FB2A_4F0F_8C87_8348A99922E0__INCLUDED_)
#define AFX_RECORDPLAYER_H__A0C4FE2D_FB2A_4F0F_8C87_8348A99922E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRecordPlayerApp:
// See RecordPlayer.cpp for the implementation of this class
//

class CRecordPlayerApp : public CWinApp
{
public:
	CRecordPlayerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordPlayerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRecordPlayerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDPLAYER_H__A0C4FE2D_FB2A_4F0F_8C87_8348A99922E0__INCLUDED_)
