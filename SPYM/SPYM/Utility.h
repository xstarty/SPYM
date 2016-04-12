#pragma once


// Utility

class Utility
{

public:
	Utility();
	virtual ~Utility();

public:
	static void ShowMessageBox(HWND Hwnd, CString strText, int nType);
};


