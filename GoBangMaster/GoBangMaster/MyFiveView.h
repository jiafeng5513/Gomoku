// MyFiveView.h : interface of the CMyFiveView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYFIVEVIEW_H__818E6AB0_3358_4472_AFC8_3461B0B72D08__INCLUDED_)
#define AFX_MYFIVEVIEW_H__818E6AB0_3358_4472_AFC8_3461B0B72D08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FiveChess.h"
#include "GameInfo.h"


class CMyFiveView : public CView
{
protected: // create from serialization only
	CMyFiveView();
	DECLARE_DYNCREATE(CMyFiveView)

// Attributes
public:
	CMyFiveDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFiveView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyFiveView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyFiveView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnP2p();
	afx_msg void OnUpdateP2p(CCmdUI* pCmdUI);
	afx_msg void OnP2c();
	afx_msg void OnUpdateP2c(CCmdUI* pCmdUI);
	afx_msg void OnStart();
	afx_msg void OnRegret();
	afx_msg void OnSet();
	afx_msg void OnNetBattle();
	afx_msg void OnUpdateNetBattle(CCmdUI* pCmdUI);
	afx_msg void OnAbout();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnComputerPlayChess();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	//定义FiveChess 对象的引用，这里是引用了 CMyFiveApp 类中的 FiveChess对象
	FiveChess &fiveChess; 

	//对战模式的标记
	bool isNetBattle; //网络模式的人人对战
	bool isP2P;       //单机模式的人人对战
	bool isP2C;       // 单机模式的人机对战
	GameInfo gameInfo; //游戏信息，包括执黑还是执白，有禁手还是无禁手

private:
	//位图信息
	CBitmap boardBmp;   //棋盘
	CBitmap blackBmp;   //黑棋子
	CBitmap whiteBmp;   //白棋子

	unsigned int boardWidth; //棋盘宽度
	unsigned int boardHeight; //棋盘高度
	unsigned int chessWidth; //棋子宽度
	unsigned int chessHeight; //棋子高度

	//棋子位置信息
	unsigned int preX; //上一步棋的坐标
	unsigned int preY;

	unsigned int nowX; //当前这一步棋的坐标
	unsigned int nowY;
	
	unsigned int tempX; //辅助变量
	unsigned int tempY; 

	unsigned int moveX; // MouseMove()的辅助变量
	unsigned int moveY; 

	//逻辑标记

	// BLACE_CHESS 表示黑棋赢，WHITE_CHESS 表示白棋赢
	// SPACE 表示未分出胜负，PEACE_CHESS 表示和棋
	bool startGame; //表示游戏是否开始
	bool blackFall; //为true表示应该是黑棋落子，否则应该是白棋落子

	// 比赛结果标记
	int gameResult; //

	CWinThread *m_pThrd;  //用于后台 AI 计算

	char paintMap[SIZE][SIZE];  // 重绘界面时用，防止 AI 的计算更改了界面


	//下面是函数

	void InitGame(); //初始化游戏
	void InitBmp();  //加载位图
	
	//处理函数
	void DealP2C();

	void UpdateRect(); //更新上一次落子位置所画的框

	// 显示结果
	void PrintResult();

	// 终止 AI 线程
	void StopAIThread();

public:
	void DealNetBattle();

};

#ifndef _DEBUG  // debug version in MyFiveView.cpp
inline CMyFiveDoc* CMyFiveView::GetDocument()
   { return (CMyFiveDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFIVEVIEW_H__818E6AB0_3358_4472_AFC8_3461B0B72D08__INCLUDED_)
