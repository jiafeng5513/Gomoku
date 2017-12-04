#if !defined(AFX_GRADE_H__661B7D51_E92E_4CC6_B665_8ECD963331FE__INCLUDED_)
#define AFX_GRADE_H__661B7D51_E92E_4CC6_B665_8ECD963331FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Grade.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Grade dialog

class Grade : public CDialog
{
// Construction
public:
	Grade(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Grade)
	enum { IDD = IDD_Grade };
	UINT	m_depth;
	UINT	m_de_depth;
	UINT	m_depth_end;
	UINT	m_depth_dis;
	UINT	m_de_depth_dis;
	UINT	m_de_depth_end;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Grade)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Grade)
	afx_msg void OnGrade1();
	afx_msg void OnGrade2();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int grade;  // grade 为 1 表示等级为 grade1 , 为 2 表示 为 grade2

	// VC 攻击采取的策略是先从小深度开始搜，没有找到必赢点就不断的加大搜索深度，但不一定要加1，
	// 而是加一个特定的值，这就是递增值。同样也有防御深度。。。这样可以大大提高AI的攻击和防守能力。
	// 但花费的时间可能较多，所以设置为用户自己输入，这样可以满足不同的用户。
	// 用这个程序在QQ游戏中下棋就需要快一点，平时玩的就需要很高的棋力，这些都可以在这里设置改变速度和棋力。

	int VCDEPTH;  // AI 进行 VC 攻击的开始深度
	int VCDEPTH_END;  // AI 进行VC攻击的最高深度
	int VCDEPTH_DIS;  // AI 进行 VC 攻击的深度 递增值

	int VCDE_DEPTH;  // AI 进行 VC 防御的开始深度
	int VCDE_DEPTH_END; // AI 进行 VC 防御的最高深度
	int VCDE_DEPTH_DIS; // AI 进行 VC 防御的深度 递增值
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADE_H__661B7D51_E92E_4CC6_B665_8ECD963331FE__INCLUDED_)
