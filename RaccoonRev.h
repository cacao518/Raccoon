
// RacoonRev.h : main header file for the RacoonRev application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRacoonRevApp:
// See RacoonRev.cpp for the implementation of this class
//

class CRacoonRevApp : public CWinApp
{
public:
	CRacoonRevApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRacoonRevApp theApp;
