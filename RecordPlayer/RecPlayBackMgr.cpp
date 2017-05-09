// RecPlayBackMgr.cpp: implementation of the RecPlayBackMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RecordPlayer.h"
#include "RecPlayBackMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TCHAR RecPlayBackMgr::m_szPrevCapState = ' ';
TCHAR RecPlayBackMgr::m_szPrevNumState = ' ';

RecPlayBackMgr::RecPlayBackMgr() : m_nTimeStamp( 0 )
{

}

RecPlayBackMgr::~RecPlayBackMgr()
{

}

int RecPlayBackMgr::PlayFile( const CString& csFilePath_i )
{

	CString csFilePath = csFilePath_i;
	LPVOID lpData1 = reinterpret_cast<LPVOID>( csFilePath.GetBuffer( MAX_PATH ) );
	 AfxBeginThread( ThreadProc, lpData1 );
	 m_nTimeStamp = GetTickCount();
	 return m_nTimeStamp;
}

UINT RecPlayBackMgr::ThreadProc( LPVOID lpData )
{
	LPCTSTR lpctszFilePath = static_cast<TCHAR*>( lpData );
	CString csFilePath = lpctszFilePath;
	if( FALSE !=  csFilePath.IsEmpty())
	{
		return 0;
	}
	
	CFile cfFile( csFilePath,
		            CFile::modeRead);
	
	CStdioFile Fileout;
	//Fileout.Open(strFileName, CFile::modeWrite);
	CString strName; 

	int inc=0;
	CStringArray strText;
	CArchive ar (&cfFile, CArchive::load); // Load its contents into a CArchive

	// Initialise the variable which holds each line's contents
	CString strLine = " ";
	
	if(!ar.ReadString(strLine)) 
	// Read the first line of the CArchive into the variable
	return 0; // Failed, so quit out
	do // Repeat while there are lines in the file left to process
	{ CString strItemName = "";
		if(strLine.GetLength() == 0) // If the line is empty, skip it
		continue;
		strText.Add( strLine ); // A line of the file
		inc++;
	}while(ar.ReadString(strLine));
	
	int nSize = strText.GetSize();
	int nOldTIme = 0;
	for( int nID = 0; nID< nSize - 3; nID++)
	{
		CString csFind = strText.GetAt( nID );
		
		int nFind1 = csFind.Find( SEPERATOR, 0 );
		CString csX = csFind.Mid( 0, nFind1 );
		int nFind2;
		nFind1++;
		nFind2 = csFind.Find( SEPERATOR, nFind1 );
		CString csY = csFind.Mid( nFind1, ( nFind2 - nFind1 ));
		nFind2++;
		int nFind3 = csFind.Find( SEPERATOR, nFind2 );
		CString csMouseEvent = csFind.Mid( nFind2, nFind3 - nFind2 );
		nFind3++;
		int nFind4 = csFind.Find( SEPERATOR, nFind3 );
		CString csTime = csFind.Mid( nFind3, nFind4 - nFind3 );
		nFind4++;
		CString csWndPos = csFind.Mid( nFind4, csFind.GetLength() - nFind4 );
		if( -1 == csFind.Find( "KeyBD", 0 ))
		{
			int nX = atoi( csX );
			int nY = atoi( csY );
			int nMillisec = atoi( csTime );
			int nEvent = GetEvent( csMouseEvent );
			SetCursorPos( nX, nY );
			if( nMillisec > 5 )
			{
				Sleep( nMillisec - 3 );
			}
			if( -1 != csMouseEvent.Find( "WM_LBUTTONDBLCLK", 0) ||
				-1 != csMouseEvent.Find( "WM_NCLBUTTONDBLCLK", 0 ))
			{
				mouse_event( MOUSEEVENTF_LEFTDOWN, nX, nY, 0, 0 );
				mouse_event( MOUSEEVENTF_LEFTUP, nX, nY, 0, 0 );
				Sleep( 1 );
				mouse_event( MOUSEEVENTF_LEFTDOWN, nX, nY, 0, 0 );
				mouse_event( MOUSEEVENTF_LEFTUP, nX, nY, 0, 0 );
			}
			else
			{
				mouse_event( nEvent, nX, nY, 0, 0 );
			}
		}
		else
		{
			GenerateKeyEvent( csFind );
		}
	}
	return 1;
}

void RecPlayBackMgr::GenerateKeyEvent( const CString& csKeyEvent_i )
{
	//"KeyBD,S,0,0,0,0,0,0,0,0,0,0,1,53,1297"
	
	CString csStringRev = csKeyEvent_i;
	csStringRev.MakeReverse();
	int nPos = csStringRev.Find( SEPERATOR, 0 );
	CString csStringR = csStringRev.Left( nPos );
	csStringR.MakeReverse();
	int nHexVal = csStringRev.Find( SEPERATOR, nPos + 1 );
	CString csMidHex = csStringRev.Mid( nPos + 1, ( nHexVal - nPos - 1));
	csMidHex.MakeReverse();
	char *stopstring = 0;
	DWORD dwData  = strtol( csMidHex.operator LPCTSTR(), &stopstring, 16 );
	int nDelay = atoi( csStringR.operator LPCTSTR());
//**************************************************************************
	int nGetTickCount = GetTickCount();
	// This whole operation is done with a time constraint
	// so such a operation is done with time limit.
	int nCurrentPos = 0;
	int nExtenKeyPos = 0;
	nCurrentPos = csKeyEvent_i.Find( SEPERATOR, nCurrentPos );
	nCurrentPos++;
	nCurrentPos = csKeyEvent_i.Find( SEPERATOR, nCurrentPos );
	nCurrentPos++;
	TCHAR tcCapital;
	TCHAR tcLShift;
	TCHAR tcLCtrl;
	TCHAR tcLWnd;
	TCHAR tcLAlt;
	TCHAR tcRAlt;
	TCHAR tcRWin;
	TCHAR tcMenu;
	TCHAR tcRCtrl;
	TCHAR tcRShift;
	TCHAR tcNumLock;

/***********************Capital Letter ***************************/
	// CAPSTATE
	//nExtenKeyPos = nCurrentPos;
	//nCurrentPos = csKeyEvent_i.Find( SEPERATOR, nCurrentPos );
	//nCurrentPos++;
	tcCapital = csKeyEvent_i.GetAt( nCurrentPos );
	if( m_szPrevCapState != tcCapital )
	{
		CString csData;
		csData.Format( "Previous=%c and Current=%c", m_szPrevCapState, tcCapital  );
		OutputDebugString( csData );
		KeyEventGenerate( tcCapital, VK_CAPITAL, KEYEVENTF_EXTENDEDKEY | 0 );
		KeyEventGenerate( tcCapital, VK_CAPITAL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP);
	}
/***********************\\Capital Letter ***************************/

/***********************Left Shift Up***************************/
	//LSHIFT
	nCurrentPos = csKeyEvent_i.Find( SEPERATOR, nCurrentPos );
	nCurrentPos++;
	tcLShift = csKeyEvent_i.GetAt( nCurrentPos );
	KeyEventGenerate( tcLShift, VK_LSHIFT, KEYEVENTF_EXTENDEDKEY | 0 );

/***********************\\Left Shift Up***************************/

/***********************Left Control Up***************************/
	// LCTRL
	nCurrentPos = csKeyEvent_i.Find( SEPERATOR, nCurrentPos );
	nCurrentPos++;
	tcLCtrl = csKeyEvent_i.GetAt( nCurrentPos );
	KeyEventGenerate( tcLCtrl, VK_LCONTROL, KEYEVENTF_EXTENDEDKEY | 0 );
/***********************\\Left Control Up***************************/

/***********************Left Window Up/Down***************************/
	//, LWINDOW
	nCurrentPos = csKeyEvent_i.Find( SEPERATOR, nCurrentPos );
	nCurrentPos++;
	tcLWnd = csKeyEvent_i.GetAt( nCurrentPos );
	KeyEventGenerate( tcLWnd, VK_LWIN, KEYEVENTF_EXTENDEDKEY | 0 );
	KeyEventGenerate( tcLWnd, VK_LWIN, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP );
/***********************\\Left Window Up/Down***************************/	

/***********************Left Alt up***************************/	
	//  LALT, RALT, RWIN, MENU,RCTRL, RSHIFT,NUMLCOK
	nCurrentPos = csKeyEvent_i.Find( SEPERATOR, nCurrentPos );
	nCurrentPos++;
	tcLAlt = csKeyEvent_i.GetAt( nCurrentPos );
	KeyEventGenerate( tcLAlt, VK_LMENU, KEYEVENTF_EXTENDEDKEY | 0 );
/***********************\\Left Alt up***************************/

	/***********************Right Alt up***************************/
	// , RALT
	nCurrentPos = csKeyEvent_i.Find( SEPERATOR, nCurrentPos );
	nCurrentPos++;
	tcRAlt = csKeyEvent_i.GetAt( nCurrentPos );
	KeyEventGenerate( tcRAlt, VK_RMENU, KEYEVENTF_EXTENDEDKEY | 0 );
	/***********************\\Right Alt up***************************/
/***********************Right Window up/Down***************************/
	// RWIN
	nCurrentPos = csKeyEvent_i.Find( SEPERATOR, nCurrentPos );
	nCurrentPos++;
	tcRWin = csKeyEvent_i.GetAt( nCurrentPos );
	KeyEventGenerate( tcRWin, VK_RWIN, KEYEVENTF_EXTENDEDKEY | 0 );
	KeyEventGenerate( tcRWin, VK_RWIN, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP );
/***********************\\Right Window up/Down***************************/

/***********************Menu up/Down***************************/
	// MENU
	nCurrentPos = csKeyEvent_i.Find( SEPERATOR, nCurrentPos );
	nCurrentPos++;
	tcMenu = csKeyEvent_i.GetAt( nCurrentPos );
	KeyEventGenerate( tcMenu, VK_APPS, KEYEVENTF_EXTENDEDKEY | 0 );
	KeyEventGenerate( tcMenu, VK_APPS, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP );
/***********************\\Menu up/Down***************************/

/***********************Right Control up **************************/
	// RCTRL
	nCurrentPos = csKeyEvent_i.Find( SEPERATOR, nCurrentPos );
	nCurrentPos++;
	tcRCtrl = csKeyEvent_i.GetAt( nCurrentPos );
	KeyEventGenerate( tcRCtrl, VK_RCONTROL, KEYEVENTF_EXTENDEDKEY | 0 );
/***********************\\Right Control up***************************/

/***********************Right Shift up **************************/
	// RSHIFT
	nCurrentPos = csKeyEvent_i.Find( SEPERATOR, nCurrentPos );
	nCurrentPos++;
	tcRShift = csKeyEvent_i.GetAt( nCurrentPos );
	KeyEventGenerate( tcRShift, VK_RSHIFT, KEYEVENTF_EXTENDEDKEY | 0 );
/***********************\\Right Shift up **************************/

/***********************NUM Lock Up/Down**************************/
	// NUMLCOK
	nCurrentPos = csKeyEvent_i.Find( SEPERATOR, nCurrentPos );
	nCurrentPos++;
	tcNumLock = csKeyEvent_i.GetAt( nCurrentPos );
	if( m_szPrevNumState != tcNumLock )
	{
		KeyEventGenerate( tcNumLock, VK_NUMLOCK, KEYEVENTF_EXTENDEDKEY | 0 );
		KeyEventGenerate( tcNumLock, VK_NUMLOCK, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP );
	}
	/***********************\\NUM Lock Up/Down**************************/

	// Key 
	KeyEventGenerate( '1', dwData, KEYEVENTF_EXTENDEDKEY | 0 );
	KeyEventGenerate( '1', dwData, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP );

	// Down of the various Key that is needed
	//LSHIFT
	KeyEventGenerate( tcLShift, VK_LSHIFT, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP );
	// ,LCTRL
	KeyEventGenerate( tcLCtrl, VK_LCONTROL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP );
	//  LALT
	KeyEventGenerate( tcLAlt, VK_LMENU, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP );
	// RALT
	KeyEventGenerate( tcRAlt, VK_RMENU, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP );
	// RCTRL
	KeyEventGenerate( tcRCtrl, VK_RCONTROL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP );
	// RSHIFT
	KeyEventGenerate( tcRShift, VK_RSHIFT, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP );


	int nFinalTickCount = GetTickCount();
	int nDifference = nFinalTickCount - nGetTickCount;
//**************************************************************************
	if(( nDifference + 10 ) < nDelay &&
		( 0 < nDelay - ( nDifference + 10 )))

	{
		nDelay = nDelay - ( nDifference + 5 );
		Sleep( nDelay );
	}
	m_szPrevCapState = tcCapital;
	m_szPrevNumState = tcNumLock;
}

void RecPlayBackMgr::KeyEventGenerate( TCHAR chStatus, DWORD dwKeyCode_i, DWORD dwkeyStat )
{
	if( '1' == chStatus )
	{
		keybd_event( dwKeyCode_i, dwKeyCode_i,dwkeyStat, 0 );
	}
}

int RecPlayBackMgr::GetEvent(const CString& csEvent )
{
	if( "WM_MOUSEMOVE" == csEvent ||
		"WM_NCMOUSEMOVE" == csEvent)
	{
		return MOUSEEVENTF_MOVE;
	}
	if( "WM_LBUTTONUP" == csEvent ||
		"WM_NCLBUTTONUP" == csEvent)
	{
		return MOUSEEVENTF_LEFTUP;
	}
	if( "WM_LBUTTONDOWN" == csEvent ||
		"WM_NCLBUTTONDOWN" == csEvent)
	{
		return MOUSEEVENTF_LEFTDOWN;
	}
	if( "WM_RBUTTONUP" == csEvent ||
		"WM_NCRBUTTONUP" == csEvent)
	{
		return MOUSEEVENTF_RIGHTUP;
	}
	if( "WM_RBUTTONDOWN" == csEvent ||
		"WM_NCRBUTTONDOWN" == csEvent)
	{
		return MOUSEEVENTF_RIGHTDOWN;
	}
	if( "WM_LBUTTONDBLCLK" == csEvent ||
		 "WM_NCLBUTTONDBLCLK" == csEvent )
	{
		return MOUSEEVENTF_LEFTUP;
	}
	return 0;
	
}



//void RecPlayBackMgr::ReplaceString( CString& csString_io, int& nPos_io )
//{
//	csString_io.SetAt( nPos_io, _T( '1' )); 
//}

