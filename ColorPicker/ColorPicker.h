
// ColorPicker.h : main header file for the ColorPicker application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CColorPickerApp:
// See ColorPicker.cpp for the implementation of this class
//

class CColorPickerApp : public CWinApp
{
public:
	CColorPickerApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CColorPickerApp theApp;
