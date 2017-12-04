// MyFive.h : main header file for the MYFIVE application
//

#if !defined(AFX_MYFIVE_H__4FFDA070_465B_42C1_8644_11463F353EC2__INCLUDED_)
#define AFX_MYFIVE_H__4FFDA070_465B_42C1_8644_11463F353EC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "FiveChess.h"

/////////////////////////////////////////////////////////////////////////////
// CMyFiveApp:
// See MyFive.cpp for the implementation of this class
//

class CMyFiveApp : public CWinApp
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CMyFiveApp();
	~CMyFiveApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFiveApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMyFiveApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
public:
	// 其他类都可以修改这个对象，这样方便很多
	FiveChess fiveChess;
	bool AIHaveCalculated; // AI 计算完成标志 
	int grade;   // AI 等级

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFIVE_H__4FFDA070_465B_42C1_8644_11463F353EC2__INCLUDED_)
