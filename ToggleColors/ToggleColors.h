
// MFCApplication1.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "ToggleSettings.h"


// CMFCApplication1App:
// See MFCApplication1.cpp for the implementation of this class
//

class CToggleColors : public CWinApp
{
public:
	CToggleColors();
	~CToggleColors();
	ToggleSettings appSettings;


	// Overrides
public:
	virtual BOOL InitInstance();

	// Implementation

	DECLARE_MESSAGE_MAP()

private:
	static LRESULT CALLBACK KeyboardHook(int code, WPARAM wParam, LPARAM lParam);
	HHOOK hookHandle;
};

extern CToggleColors theApp;