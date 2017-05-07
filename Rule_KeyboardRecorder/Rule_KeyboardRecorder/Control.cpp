// Control.cpp : implementation file
//

#include "stdafx.h"
#include "Control.h"

#define $MAX_NUM 86400.00

// CNumEdit

BEGIN_MESSAGE_MAP(CNumEdit, CEdit)
	ON_WM_CHAR()
	ON_WM_CREATE()
END_MESSAGE_MAP()

CNumEdit::CNumEdit(void)
{
	dMaxValue = $MAX_NUM;
	dMinValue = 0;
}

CNumEdit::~CNumEdit(void)
{
}
void CNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString str;
	GetWindowText(str);

	int nSource, nDestination;
	GetSel(nSource, nDestination);

	
	// Ensure decimal point should appear at most once
	if (nChar == '.')
	{
		// If the original string has a decimal point, the decimal point is not entered, to ensure that the maximum input a decimal point
		if (str.Find('.') != -1)
		{
		}
		// Otherwise, enter the decimal point
		else
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}
	// Ensure negative sign can occur only once, and only appear in the first character
	else if (nChar == '-')
	{
		// Not yet entered any string
		if (str.IsEmpty())
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		else
		{
			// Now select the entire contents
			if (nSource == 0 && nDestination == str.GetLength() || nSource == 0 && nDestination == 0 && str.Left(1).Compare(L"-"))
			{
				CEdit::OnChar(nChar, nRepCnt, nFlags);
			}
			else
			{
			}
		}
	}
	// In addition to the decimal point and negative sign, but also allows you to enter numbers, Backspace, Delete
	else if ((nChar >= '0' && nChar <= '9' || nChar == 8)/* || (nChar == 0x08) || (nChar == 0x10) */)
	{
		// If the first character is input, and there are '-' is not input
		if (nSource == 0 && nDestination == 0 && !str.IsEmpty() && str.Find('-') != -1)
		{
		}
		else
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}

	}
	// Other keys do not respond
	else
	{
	}

	GetWindowText(str);
	if (_ttof(str) > dMaxValue)
	{
		str.Format(L"%.0f", dMaxValue);
		SetWindowText(str);
	}

	/*
	GetWindowText(str);
	if (_ttof(str) > dMaxValue)
	{
		CString msg;
		msg.Format(L"The value can not be greater than %0.1f", dMaxValue);
		CDlgMessage(L"Parameter error", msg).DoModal();
		SetWindowText(L"");
	}
	else if (_ttof(str) < dMinValue)
	{
		CString msg;
		msg.Format(L"The value can not be less than %0.1f", dMinValue);
		CDlgMessage(L"Parameter error", msg).DoModal();
		SetWindowText(L"");
	}
	*/
}

void CNumEdit::setRange(double min, double max)
{
	dMaxValue = max;
	dMinValue = min;
}

// CNumEdit message handlers


