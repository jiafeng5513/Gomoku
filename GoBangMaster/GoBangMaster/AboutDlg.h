#if !defined(AFX_ABOUTDLG_H__0CABAFC7_0D36_40B5_A3FC_0BA8A9ECBE48__INCLUDED_)
#define AFX_ABOUTDLG_H__0CABAFC7_0D36_40B5_A3FC_0BA8A9ECBE48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AboutDlg dialog

class AboutDlg : public CDialog
{
// Construction
public:
	AboutDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(AboutDlg)
	enum { IDD = IDD_ABOUTDLG };
	CString	m_Content;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(AboutDlg)
	afx_msg void OnBoke();
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTDLG_H__0CABAFC7_0D36_40B5_A3FC_0BA8A9ECBE48__INCLUDED_)
