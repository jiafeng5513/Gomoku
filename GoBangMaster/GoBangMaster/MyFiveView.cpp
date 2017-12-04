// MyFiveView.cpp : implementation of the CMyFiveView class
//

#include "stdafx.h"
#include "MyFive.h"

#include "MyFiveDoc.h"
#include "MyFiveView.h"
#include "MainFrm.h"
#include "Mmsystem.h"
#include "AboutDlg.h"
#include "Grade.h"
#include "AIThread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMyFiveView

IMPLEMENT_DYNCREATE(CMyFiveView, CView)

BEGIN_MESSAGE_MAP(CMyFiveView, CView)
	//{{AFX_MSG_MAP(CMyFiveView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_P2P, OnP2p)
	ON_UPDATE_COMMAND_UI(ID_P2P, OnUpdateP2p)
	ON_COMMAND(ID_P2C, OnP2c)
	ON_UPDATE_COMMAND_UI(ID_P2C, OnUpdateP2c)
	ON_COMMAND(ID_START, OnStart)
	ON_COMMAND(ID_Regret, OnRegret)
	ON_COMMAND(IDC_SET, OnSet)
	ON_COMMAND(ID_NetBattle, OnNetBattle)
	ON_UPDATE_COMMAND_UI(ID_NetBattle, OnUpdateNetBattle)
	ON_COMMAND(ID_About, OnAbout)
	ON_WM_TIMER()
	ON_COMMAND(ID_ComputerPlayChess, OnComputerPlayChess)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFiveView construction/destruction

extern CMyFiveApp theApp;  //声明

CMyFiveView::CMyFiveView():fiveChess(theApp.fiveChess) // 这里初始化引用
{
	InitBmp();
	InitGame();

	//初始化单机模式的人机对战
	isNetBattle = false;
	isP2C = true;
	isP2P = false;

	gameResult = 0;
	m_pThrd = NULL;
}

CMyFiveView::~CMyFiveView()
{
	StopAIThread();
}

BOOL CMyFiveView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyFiveView drawing

void CMyFiveView::OnDraw(CDC* pDC)
{
	CMyFiveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	// 设置窗口标题
	AfxGetMainWnd()->SetWindowText("超级五子棋 1.3");

	//自定义重绘界面
	/*******************************************************/

	CDC boardDC,blackDC,whiteDC;
	
	boardDC.CreateCompatibleDC(pDC);//创建内存设备环境
	boardDC.SelectObject(&boardBmp); //将位图选入内存设备环境

	blackDC.CreateCompatibleDC(pDC);
	blackDC.SelectObject(&blackBmp);

	whiteDC.CreateCompatibleDC(pDC);
	whiteDC.SelectObject(&whiteBmp);

	//把位图从内存设备环境中复制到指定的设备环境（例如显示器）中
	pDC->BitBlt(0,0,boardWidth,boardHeight,&boardDC,0,0,SRCCOPY); //棋盘

	//下面画出棋子
	// TransparentBlt()位于Msimg32.lib
	
	extern CMyFiveApp theApp; //

	char cChess;
	int i,j;
	for(i=0;i<SIZE;++i)
	{
		for(j=0;j<SIZE;++j)
		{
			if( !theApp.AIHaveCalculated )// 若是人机对战，且 AI 正在计算中
				cChess = paintMap[i][j];
			else
				cChess = fiveChess.GetStatus(i,j);

			if(cChess == BLACK_CHESS)
			{
				TransparentBlt(pDC->m_hDC,j*WIDTH+OFFSETX,i*WIDTH+OFFSETY,
					chessWidth,chessHeight,blackDC.m_hDC,0,0,
					chessWidth,chessHeight,CLR_MAGENTA); //最后一参数：使这种颜色透明
			}
			else if(cChess == WHITE_CHESS)
			{
				TransparentBlt(pDC->m_hDC,j*WIDTH+OFFSETX,i*WIDTH+OFFSETY,
					chessWidth,chessHeight,whiteDC.m_hDC,0,0,
					chessWidth,chessHeight,CLR_MAGENTA); //最后一参数：使这种颜色透明
			}
		}
	}
	

	if(nowX != -1 && nowY != -1)
	{
		//给当前鼠标指向点画框
		if(blackFall)
			pDC->Draw3dRect(nowX,nowY,chessWidth,chessHeight,CLR_BLACK,CLR_BLACK);
		else
			pDC->Draw3dRect(nowX,nowY,chessWidth,chessHeight,CLR_WHITE,CLR_WHITE);

		//给上一个棋子画框
		if( preX != -1 || preY != -1 )
		{
			if( tempX != -1 || tempY != -1 )
			{
				pDC->Draw3dRect(preX,preY,chessWidth,chessHeight,CLR_RED,CLR_RED);
			}
		}
	}

	// 下面给棋子画上数字
	int x,y;
	CString msg;

	for(i=0;i<fiveChess.chessPro.size();++i)
	{
		x = fiveChess.chessPro[i].first;
		y = fiveChess.chessPro[i].second;
		msg.Format(TEXT("%d"),i+1);

		if(BLACK_CHESS == fiveChess.GetStatus(x,y))
		{
		//	pDC->SetBkColor(CLR_BK_BLACK); // 设置字体背景颜色
			pDC->SetBkMode(TRANSPARENT); // 使背景透明
			pDC->SetTextColor(CLR_WHITE);
		}
		else
		{
		//	pDC->SetBkColor(CLR_BK_WHITE);
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(CLR_BLACK);
			
		}
		if(i<=8)
			pDC->TextOut(y*WIDTH+OFFSETX+12,x*WIDTH+OFFSETY+9,msg);
		else if(i>=9 && i<=98)
			pDC->TextOut(y*WIDTH+OFFSETX+9,x*WIDTH+OFFSETY+9,msg);
		else
			pDC->TextOut(y*WIDTH+OFFSETX+5,x*WIDTH+OFFSETY+9,msg);
	}

	// 显示坐标
	for(i=0;i<SIZE;++i)
	{
		if(fiveChess.GetStatus(0,i) != SPACE)
			continue;
		msg.Format(TEXT("%c"),'A'+i);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(CLR_BLACK);
		pDC->TextOut(i*WIDTH+OFFSETX+11,2,msg);
	}
	for(i=0;i<SIZE;++i)
	{
		if(fiveChess.GetStatus(i,0) != SPACE)
			continue;
		msg.Format(TEXT("%d"),i+1);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(CLR_BLACK);
		if(i<=8)
			pDC->TextOut(4,i*WIDTH+OFFSETY+7,msg);
		else
			pDC->TextOut(1,i*WIDTH+OFFSETY+7,msg);
	}

	// 打印比赛结果
	if( gameResult ) // gameResult 不为0时，就表示产生了比赛结果
		PrintResult();


	ReleaseDC(&boardDC);
	ReleaseDC(&blackDC);
	ReleaseDC(&whiteDC);
}

/////////////////////////////////////////////////////////////////////////////
// CMyFiveView printing

BOOL CMyFiveView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation

	return DoPreparePrinting(pInfo);
}

void CMyFiveView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyFiveView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMyFiveView diagnostics

#ifdef _DEBUG
void CMyFiveView::AssertValid() const
{
	CView::AssertValid();
}

void CMyFiveView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyFiveDoc* CMyFiveView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyFiveDoc)));
	return (CMyFiveDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyFiveView message handlers

//初始化游戏
void CMyFiveView::InitGame()
{
	fiveChess.InitChessMap();

	startGame = false;

	preX = preY = nowX = nowY = tempX = tempY = -1;
	moveX = moveY = -1;

	extern CMyFiveApp theApp;
	theApp.AIHaveCalculated = true;
}

//加载并初始化位图信息
void CMyFiveView::InitBmp()
{
	BITMAP bm; //定义位图结构
	//载入棋盘
	boardBmp.LoadBitmap(IDB_board); //载入位图资源
	boardBmp.GetBitmap(&bm);  //得到位图信息
	boardWidth = bm.bmWidth; //棋盘宽度
	boardHeight = bm.bmHeight; //棋盘高度

	blackBmp.LoadBitmap(IDB_black);
	whiteBmp.LoadBitmap(IDB_white);

	blackBmp.GetBitmap(&bm);
	chessWidth = bm.bmWidth; //棋子宽度
	chessHeight = bm.bmHeight; //棋子高度
}


void CMyFiveView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!startGame)  //未开始便返回
		return ;

	extern CMyFiveApp theApp;
	if(!theApp.AIHaveCalculated)
	{
		AfxMessageBox(TEXT("AI线程正在计算，请稍等"));
		return ;
	}

	if(point.x-OFFSETX < 0 || point.y-OFFSETY < 0) //出界
		return ;

	int nX = static_cast<int>( (point.x-OFFSETX) / WIDTH );
	int nY = static_cast<int>( (point.y-OFFSETY) / WIDTH );

	if( nX > 14 || nY > 14) //出界
		return ;

	char cChess = fiveChess.GetStatus(nY,nX);
	if( cChess != SPACE) //位置上已有棋子则返回
		return ;

	//////////////////////////////////////////////////////////////////////////

	//人机对战
	if(isP2C)
	{
		if(gameInfo.isBlack && blackFall) //我执黑并且该黑棋落子
		{
			blackFall = false;
			cChess = BLACK_CHESS;
		}
		else if(!gameInfo.isBlack && !blackFall)   //我执白并且该白棋落子
		{
			blackFall = true;
			cChess = WHITE_CHESS;
		}
		else  //否则就该对方下棋
			return ;
	}

	//人人对战
	else if(isP2P)
	{
		if(blackFall) //黑棋下落
		{
			blackFall = false;
			cChess = BLACK_CHESS;
		}
		else
		{
			blackFall = true;
			cChess = WHITE_CHESS;
		}
	}

	//网络模式的人人对战
	else if(isNetBattle)
	{
		if(gameInfo.isBlack && blackFall) //我执黑并且该黑棋落子
		{
			blackFall = false;
			cChess = BLACK_CHESS;
		}
		else if(!gameInfo.isBlack && !blackFall)   //我执白并且该白棋落子
		{
			blackFall = true;
			cChess = WHITE_CHESS;
		}
		else  //否则就该对方下棋
			return ;
	}
	//////////////////////////////////////////////////////////////////////////
	UpdateRect(); //更新上一次落子位置所画的框

	//
	char ch; 
	fiveChess.SetChess(nY,nX,cChess);
	
	if(isNetBattle) //若是网络模式还需向对方发送我的落子消息
	{
		CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
		pFrm->m_RView->SendMsg(2);
	}

	ch = fiveChess.JudgeWin(cChess); //判断是否赢棋

	//有禁手并且当前落子为黑棋，就需要判断这点是否是禁手
	if(gameInfo.isForbid && BLACK_CHESS == cChess) //
	{
		int res = fiveChess.IsKinjite(nY,nX);

		if( res ) // 禁手
		{
			startGame = false;
			/*
			CString msg;
			if( 1 == res )
				msg.Format(TEXT("长连禁手，黑负"));
			else if( 2 == res )
				msg.Format(TEXT("三三禁手，黑负"));
			else if( 3 == res )
				msg.Format(TEXT("四四禁手，黑负"));
			AfxMessageBox(msg);
			*/
			gameResult = res;
			PrintResult(); //
		}
	}
	if(startGame)  
	{
		if(ch == cChess || ch == PEACE_CHESS) //赢棋或和棋
		{
			startGame = false;
		}
		//赢棋
		if(!startGame) 
		{
			/*
			CString msg;
			if(BLACK_CHESS == ch)
				msg.Format(TEXT("五连珠，黑赢"));
			else if(WHITE_CHESS == ch)
				msg.Format(TEXT("五连珠，白赢"));
			else if(PEACE_CHESS == ch)
				msg.Format(TEXT("和棋"));
			AfxMessageBox(msg);
			*/
			int res; 
			if( BLACK_CHESS == ch )
				res = 4;
			else if( WHITE_CHESS == ch )
				res = 5;
			else if( PEACE_CHESS == ch )
				res = 6;

			gameResult = res;
			PrintResult(); //
		}
	}

	// 若是单机模式的人机对战
	if(isP2C)
		DealP2C();

	CView::OnLButtonDown(nFlags, point);
}

void CMyFiveView::OnMouseMove(UINT nFlags, CPoint point) 
{	
	if(!startGame) //未开始
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		return ;
	}

	extern CMyFiveApp theApp;
	
	if(!theApp.AIHaveCalculated) // 人机对战，AI 正在计算
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		return ;
	}


	if(point.x-OFFSETX < 0 || point.y-OFFSETY < 0) //出界
		return ;

	int nX = static_cast<int>( (point.x-OFFSETX) / WIDTH );
	int nY = static_cast<int>( (point.y-OFFSETY) / WIDTH );

	if( nX > 14 || nY > 14) //出界
		return ;
	
	// 更换光标
	if(fiveChess.GetStatus(nY,nX) != SPACE)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS)); 
		return ;
	}
	else
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_hand));
	}


	nowX = nX*WIDTH + OFFSETX;
	nowY = nY*WIDTH + OFFSETY;


	CRect rect;
	//更新上一次光标处
	rect.left = moveX;
	rect.top = moveY;
	rect.right = rect.left + WIDTH;
	rect.bottom = rect.top + WIDTH;
	InvalidateRect(&rect,0);

	//更新当前光标处
	rect.left = nowX;
	rect.top = nowY;
	rect.right = rect.left + WIDTH;
	rect.bottom = rect.top + WIDTH;

	InvalidateRect(&rect,0);

	//更新 moveX,moveY
	moveX = nowX;
	moveY = nowY;

	CView::OnMouseMove(nFlags, point);
}

//网络模式的设置
void CMyFiveView::OnNetBattle() 
{
	isNetBattle = true;
	isP2C = isP2P = false;
}

void CMyFiveView::OnUpdateNetBattle(CCmdUI* pCmdUI) 
{
	if(isNetBattle)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

//单机模式的人人对战
void CMyFiveView::OnP2p() 
{
	isP2P = true;
	isP2C = isNetBattle = false;
}

void CMyFiveView::OnUpdateP2p(CCmdUI* pCmdUI) 
{
	if(isP2P)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

//单机模式的人机对战
void CMyFiveView::OnP2c() 
{
	isP2C = true;
	isP2P = isNetBattle = false;
	fiveChess.haveKinjite = gameInfo.isForbid;
	fiveChess.cComputer = gameInfo.isBlack ? WHITE_CHESS : BLACK_CHESS;
	fiveChess.cPeople = gameInfo.isBlack ? BLACK_CHESS : WHITE_CHESS;
}

void CMyFiveView::OnUpdateP2c(CCmdUI* pCmdUI) 
{
	if(isP2C)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

//开始游戏
void CMyFiveView::OnStart() 
{	
	StopAIThread();

	fiveChess.haveKinjite = gameInfo.isForbid;
	fiveChess.cComputer = gameInfo.isBlack ? WHITE_CHESS : BLACK_CHESS;
	fiveChess.cPeople = gameInfo.isBlack ? BLACK_CHESS : WHITE_CHESS;
	
	gameResult = 0;

	if(isP2P)
	{
		// ...不需要
	}
	else if(isP2C)
	{
		if(BLACK_CHESS == fiveChess.cComputer)  //计算机执黑
		{
			// DealP2C(); //放到后面执行
		}
	}
	else if(isNetBattle)
	{
		CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
		if(!pFrm->m_RView->haveLink)
		{
			AfxMessageBox(TEXT("还未和对方连接!\r\n\n请到右边连接网络"));
			return ;
		}
		
		//再次向对方发送我的登录消息（在连接的时候就已经发送过一次登录消息）
		pFrm->m_RView->SendMsg(1);

		//若已经连接，必然已经收到对方发送的第一个消息过来
		if(gameInfo.isBlack == pFrm->m_RView->recvNetMsg.is_black)
		{
			AfxMessageBox(TEXT("双方不能执同种颜色的棋子！\r\n 请重新设置"));
			return ;
		}
		if(gameInfo.isForbid != pFrm->m_RView->recvNetMsg.is_forbid)
		{
			AfxMessageBox(TEXT("双方对于禁手的设置不一致! \r\n 请重新设置"));
			return ;
		}

	}

	InitGame();
	startGame = true;
	blackFall = true;
	Invalidate(0);

	if(isP2C && BLACK_CHESS == fiveChess.cComputer)
		DealP2C();
}

//处理单机模式人机对战
void CMyFiveView::DealP2C()
{
	if(!startGame)
		return ;
	if(!blackFall && BLACK_CHESS == fiveChess.cComputer)
		return ;
	if(blackFall && WHITE_CHESS == fiveChess.cComputer)
		return ;

	extern CMyFiveApp theApp;

	theApp.grade = gameInfo.m_grade.grade; // 得到 AI  的等级
	theApp.AIHaveCalculated = false; // 初始化，标记 AI 是否完成计算

	// VC 的攻击深度和防御深度设置在这里初始化，可以动态设置
	fiveChess.VCDEPTH = gameInfo.m_grade.VCDEPTH; 
	fiveChess.VCDEPTH_END = gameInfo.m_grade.VCDEPTH_END;
	fiveChess.VCDEPTH_DIS = gameInfo.m_grade.VCDEPTH_DIS;

	fiveChess.VCDE_DEPTH = gameInfo.m_grade.VCDE_DEPTH;
	fiveChess.VCDE_DEPTH_END = gameInfo.m_grade.VCDE_DEPTH_END;
	fiveChess.VCDE_DEPTH_DIS = gameInfo.m_grade.VCDE_DEPTH_DIS;

	memcpy(paintMap,fiveChess.chessMap,sizeof(paintMap));  // 保存当前的棋局用来 重绘界面

	CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
	pFrm->m_wndStatusBar.SetPaneText(0,TEXT("计算机正在思考......请耐心等待"),1);

	// 启动
	m_pThrd = AfxBeginThread(RUNTIME_CLASS(CAIThread));

	// 执行
	m_pThrd->PostThreadMessage(WM_THREAD,NULL,NULL);

	// 设置定时器监视 AI 是否完成计算
	SetTimer(1,200,NULL);
}

//处理网络模式人人对战，只在void RView::DealMsg()中调用该函数
// 对方落子
void CMyFiveView::DealNetBattle()
{
	CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();

	char cChess = pFrm->m_RView->recvNetMsg.is_black ? BLACK_CHESS : WHITE_CHESS;
	int row = pFrm->m_RView->recvNetMsg.row;
	int col = pFrm->m_RView->recvNetMsg.col;

	nowX = col*WIDTH + OFFSETX;
	nowY = row*WIDTH + OFFSETY;


	fiveChess.SetChess(row,col,cChess);
	char ch = fiveChess.JudgeWin(cChess); //判断是否赢棋
	UpdateRect(); //更新框

	ch = fiveChess.JudgeWin(cChess); //判断是否赢棋
	
	//有禁手并且当前落子为黑棋，就需要判断这点是否是禁手
	if(gameInfo.isForbid && BLACK_CHESS == cChess) //
	{
		int res = fiveChess.IsKinjite(row,col);
		
		if( res ) // 禁手
		{
			startGame = false;

			gameResult = res;
			PrintResult(); //
		}
	}
	if(startGame)  
	{
		if(ch == cChess || ch == PEACE_CHESS) //赢棋或和棋
		{
			startGame = false;
		}
		//赢棋
		if(!startGame) 
		{
			int res;
			if( BLACK_CHESS == ch )
				res = 4;
			else if( WHITE_CHESS == ch )
				res = 5;
			else if( PEACE_CHESS == ch )
				res = 6;

			gameResult = res;
			PrintResult(); //
		}
	}

	blackFall = gameInfo.isBlack; //现在我可以落子
}

//更新上一次落子位置所画的框
void CMyFiveView::UpdateRect()
{
	tempX = preX;
	tempY = preY;
	preX = nowX;
	preY = nowY;
	
	CRect rect;
	//更新先前光标处
	rect.left = tempX;
	rect.top = tempY;
	rect.right = rect.left + WIDTH;
	rect.bottom = rect.top + WIDTH;
	InvalidateRect(&rect,0);
	
	//更新当前光标
	rect.left = nowX;
	rect.top = nowY;
	rect.right = rect.left + WIDTH;
	rect.bottom = rect.top + WIDTH;
	InvalidateRect(&rect,0);

	if(gameInfo.isVoice)
	{
		//播放落子的声音
		PlaySound(MAKEINTRESOURCE(IDR_PUTSTONE),AfxGetResourceHandle(),
			SND_ASYNC|SND_RESOURCE|SND_NODEFAULT|SND_NOWAIT);
	}
}


//悔棋
void CMyFiveView::OnRegret() 
{
	if(fiveChess.chessPro.size()<=1)
		return ;
	fiveChess.RetractChess(); 
	Invalidate(0);

	//像对方发送悔棋消息
	CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
	pFrm->m_RView->SendMsg(4);
}


//设置
void CMyFiveView::OnSet() 
{
	gameInfo.DoModal();
}

//关于
void CMyFiveView::OnAbout() 
{
	AboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// 显示结果
void CMyFiveView::PrintResult()
{
	CDC *dc = GetDC();
	CFont font;
	font.CreatePointFont(300,"华文行楷",NULL);
	dc->SelectObject(&font);

	dc->SetBkMode(TRANSPARENT);
	dc->SetTextColor(RGB(60,130,40));
	
	CRect rect;
	GetClientRect(&rect);

	int cx = (rect.left+rect.right)/2-100 , cy = (rect.top+rect.bottom)/4;

	switch(gameResult)
	{
	case 1: 
		dc->TextOut(cx,cy,TEXT("长连禁手,黑负!"));
		break;
	case 2:
		dc->TextOut(cx,cy,TEXT("三三禁手,黑负!"));
		break;
	case 3:
		dc->TextOut(cx,cy,TEXT("四四禁手,黑负!"));
		break;
	case 4:
		dc->TextOut(cx,cy,TEXT("五连珠,黑胜!"));
		break;
	case 5:
		dc->TextOut(cx,cy,TEXT("五连珠,白胜!"));
		break;
	case 6:
		dc->TextOut(cx,cy,TEXT("和棋!"));
		break;
	}

	ReleaseDC(dc);
}

// 用来辅助 AI 后台计算
void CMyFiveView::OnTimer(UINT nIDEvent) 
{
	extern CMyFiveApp theApp;
	if( !theApp.AIHaveCalculated ) // AI 没有计算完 则返回
		return;

	// 下面处理计算机落子

	int row,col;
	row = fiveChess.comX;
	col = fiveChess.comY;
	
	char cChess = fiveChess.cComputer;
	
	nowX = col*WIDTH + OFFSETX;
	nowY = row*WIDTH + OFFSETY;
	
	fiveChess.SetChess(row,col,cChess);
	char ch = fiveChess.JudgeWin(cChess); //判断是否赢棋
	UpdateRect(); //更新框
	
	ch = fiveChess.JudgeWin(cChess); //判断是否赢棋
	
	//有禁手并且当前落子为黑棋，就需要判断这点是否是禁手
	if(gameInfo.isForbid && BLACK_CHESS == cChess) //
	{
		int res = fiveChess.IsKinjite(row,col);
		
		if( res ) // 禁手
		{
			startGame = false;
			
			gameResult = res;
			PrintResult(); //
		}
	}
	if(startGame)  
	{
		if(ch == cChess || ch == PEACE_CHESS) //赢棋或和棋
		{
			startGame = false;
		}
		//赢棋
		if(!startGame) 
		{
			int res;
			if( BLACK_CHESS == ch )
				res = 4;
			else if( WHITE_CHESS == ch )
				res = 5;
			else if( PEACE_CHESS == ch )
				res = 6;
			
			gameResult = res;
			PrintResult(); //
		}
	}
		
	blackFall = !blackFall; //现在我可以落子

	theApp.AIHaveCalculated = true; // 重置

	KillTimer(1);

	CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
	CString msg;
	if( 0 == fiveChess.AIState )
		msg.Format(TEXT("现在轮到玩家落子。"));
	else if( 1 == fiveChess.AIState )
		msg.Format(TEXT("现在轮到玩家落子。 计算机正在进行 VCF 攻击"));
	else if( 2 == fiveChess.AIState )
		msg.Format(TEXT("现在轮到玩家落子。 计算机正在阻止玩家的 VCF 攻击"));
	else if( 3 == fiveChess.AIState )
		msg.Format(TEXT("现在轮到玩家落子。 计算机正在进行 VCF 和 VCT 联合攻击"));
	else if( 4 == fiveChess.AIState )
		msg.Format(TEXT("现在轮到玩家落子。 计算机正在阻止玩家的 VCF 和 VCT 联合攻击"));

	pFrm->m_wndStatusBar.SetPaneText(0,msg,1);

	CView::OnTimer(nIDEvent);
}


//终止 AI 线程
void CMyFiveView::StopAIThread()
{
	// 结束线程
	if(m_pThrd)
	{
		HANDLE hp = m_pThrd->m_hThread;
		if(hp)
		{
			if(WaitForSingleObject(hp,1) != WAIT_OBJECT_0) 
			{
				TerminateThread(hp,0);
			}
			CloseHandle(hp);
		}
	}
}

// 计算机帮助玩家落子
// 同时也可以用这个功能来测试 AI 的棋力
void CMyFiveView::OnComputerPlayChess() 
{
	if(!startGame)
	{
		AfxMessageBox(TEXT("请先点击开始游戏"));
		return ;
	}

	extern CMyFiveApp theApp;
	if(!theApp.AIHaveCalculated)
	{
		AfxMessageBox(TEXT("AI线程正在计算，请稍等"));
		return;
	}

	if(blackFall) 
	{
		fiveChess.cComputer = BLACK_CHESS;
		fiveChess.cPeople = WHITE_CHESS;
	}
	else
	{
		fiveChess.cComputer = WHITE_CHESS;
		fiveChess.cPeople = BLACK_CHESS;
	}

	DealP2C();
}
