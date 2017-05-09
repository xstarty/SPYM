// RecPlayBackMgr.h: interface for the RecPlayBackMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECPLAYBACKMGR_H__8D1D64CA_69E1_4963_8086_90B1C1AA5BF3__INCLUDED_)
#define AFX_RECPLAYBACKMGR_H__8D1D64CA_69E1_4963_8086_90B1C1AA5BF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

static TCHAR SEPERATOR = ' ';

class RecPlayBackMgr  
{
public:
	RecPlayBackMgr();
	virtual ~RecPlayBackMgr();
	int PlayFile( const CString& csFilePath_i );

private:
	//void ReplaceString( CString& csString_io, int& nPos_io );
	static int GetEvent(const CString& csEvent );
	static void GenerateKeyEvent( const CString& csKeyEvent_i );
	static void KeyEventGenerate( TCHAR chStatus, DWORD dwKeyCode_i, DWORD dwkeyStat );
	static UINT ThreadProc( LPVOID lpData );

private:
	
	CStdioFile FileHandler;
	CStringArray m_csArray;
	int m_nTimeStamp;
	static TCHAR m_szPrevCapState;
	static TCHAR m_szPrevNumState;
};

#endif // !defined(AFX_RECPLAYBACKMGR_H__8D1D64CA_69E1_4963_8086_90B1C1AA5BF3__INCLUDED_)
