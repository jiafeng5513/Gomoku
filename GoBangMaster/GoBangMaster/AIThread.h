#if !defined(AFX_AITHREAD_H__99D9C7E2_0E61_4375_AD06_1178D23B6325__INCLUDED_)
#define AFX_AITHREAD_H__99D9C7E2_0E61_4375_AD06_1178D23B6325__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AIThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CAIThread thread

#define WM_THREAD WM_USER+10

class CAIThread : public CWinThread
{
	DECLARE_DYNCREATE(CAIThread)
protected:
	CAIThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAIThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAIThread();

	// Generated message map functions
	//{{AFX_MSG(CAIThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	afx_msg void AI(WPARAM wParam,LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AITHREAD_H__99D9C7E2_0E61_4375_AD06_1178D23B6325__INCLUDED_)
