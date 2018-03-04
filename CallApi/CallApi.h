// CallApi.h : main header file for the CALLAPI application
//

#if !defined(AFX_CALLAPI_H__35DC375C_CC72_4A70_9E7B_3177B0FA0D57__INCLUDED_)
#define AFX_CALLAPI_H__35DC375C_CC72_4A70_9E7B_3177B0FA0D57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCallApiApp:
// See CallApi.cpp for the implementation of this class
//

class CCallApiApp : public CWinApp
{
public:
	CCallApiApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCallApiApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCallApiApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLAPI_H__35DC375C_CC72_4A70_9E7B_3177B0FA0D57__INCLUDED_)
