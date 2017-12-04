#if !defined(AFX_GAMEINFO_H__D6A73F83_0601_4720_B0D0_F8DD63714A7F__INCLUDED_)
#define AFX_GAMEINFO_H__D6A73F83_0601_4720_B0D0_F8DD63714A7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GameInfo.h : header file
//

#include "Grade.h"
/////////////////////////////////////////////////////////////////////////////
// GameInfo dialog

class GameInfo : public CDialog
{
// Construction
public:
	GameInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GameInfo)
	enum { IDD = IDD_GameInfo };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GameInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GameInfo)
	afx_msg void Onblack();
	afx_msg void Onwhite();
	afx_msg void OnhaveForbid();
	afx_msg void OnnoForbid();
	virtual BOOL OnInitDialog();
	afx_msg void OnhaveVoice();
	afx_msg void OnnoVoice();
	afx_msg void OnAIGrade();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	bool isBlack;  // 为true表示执黑，否则执白
	bool isForbid;  //为true表示有禁手，否则无禁手
	bool isVoice;  //为 true表示播放声音，否则不播放
	Grade m_grade;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEINFO_H__D6A73F83_0601_4720_B0D0_F8DD63714A7F__INCLUDED_)
