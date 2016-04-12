// Utility.cpp : implementation file
//

#include "stdafx.h"
#include "Utility.h"
#include "Resource.h"

// Utility

Utility::Utility()
{

}

Utility::~Utility()
{
}

void Utility::ShowMessageBox(HWND Hwnd, CString strText, int nType)
{
	CString strCaption;
	strCaption.LoadString(IDS_CAPTION);
	MessageBox(Hwnd, strText, strCaption, nType);
}

// Utility message handlers


