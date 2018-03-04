// InjectDll.h : main header file for the INJECTDLL application
//

#if !defined(AFX_INJECTDLL_H__D1F6A2CA_D992_4585_8C0F_4A18232574B6__INCLUDED_)
#define AFX_INJECTDLL_H__D1F6A2CA_D992_4585_8C0F_4A18232574B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CInjectDllApp:
// See InjectDll.cpp for the implementation of this class
//

class CInjectDllApp : public CWinApp
{
public:
	CInjectDllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInjectDllApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CInjectDllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INJECTDLL_H__D1F6A2CA_D992_4585_8C0F_4A18232574B6__INCLUDED_)
