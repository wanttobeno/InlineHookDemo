// CallApiDlg.h : header file
//

#if !defined(AFX_CALLAPIDLG_H__1D4E06BA_949B_4147_8825_82BDBC07A08E__INCLUDED_)
#define AFX_CALLAPIDLG_H__1D4E06BA_949B_4147_8825_82BDBC07A08E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCallApiDlg dialog

class CCallApiDlg : public CDialog
{
// Construction
public:
	CCallApiDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCallApiDlg)
	enum { IDD = IDD_CALLAPI_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCallApiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCallApiDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLAPIDLG_H__1D4E06BA_949B_4147_8825_82BDBC07A08E__INCLUDED_)
