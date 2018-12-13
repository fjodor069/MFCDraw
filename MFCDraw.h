
// MFCDraw.h : main header file for the MFCDraw application
//
// from: Packt Publishing
//
// 2-12-2018	RvS
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "..\..\Utility\check.h"
// CMFCDrawApp:
// See MFCDraw.cpp for the implementation of this class
//

class CMFCDrawApp : public CWinApp
{
public:
	CMFCDrawApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCDrawApp theApp;
