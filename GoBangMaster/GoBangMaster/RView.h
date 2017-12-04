#if !defined(AFX_RVIEW_H__10C169A3_EB04_4EEE_986A_1C08B46B3C6A__INCLUDED_)
#define AFX_RVIEW_H__10C169A3_EB04_4EEE_986A_1C08B46B3C6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

/*
     为了简单起见，直接发送一个结构体消息即可，各种内容都放到这个里面，
	 不处理粘包问题。
*/

#pragma  pack(push,1)
struct NetMsg
{
	int type;   //消息类型，为1表示登录消息，为2表示下棋的位置消息，为3表示聊天消息,为4表示悔棋消息
	char name[20]; //昵称(登录消息）
	bool is_black;  //为true表示执黑
	bool is_forbid;  // 为true表示为有禁手
	int row,col;  //下棋的位置
	char str[100];  //聊天的字符串
};
#pragma  pack(pop)


//一旦指定的网络事件发生时，操作系统就会发送该自定义消息通知调用线程。
#define UM_SOCK WM_USER+1  //定义自定义消息

class RView : public CFormView
{
protected:
	RView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(RView)

// Form Data
public:
	//{{AFX_DATA(RView)
	enum { IDD = IDD_RVIEW };
	CString	m_ChatContent;
	UINT	m_LocalPort;
	UINT	m_SrvPort;
	CString	m_name;
	CString	m_SendMsg;
	CString	m_IP;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RView)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~RView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(RView)
	afx_msg void OnStartServer();
	afx_msg void OnLink();
	afx_msg void OnSend();   
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnSock(WPARAM,LPARAM); //自定义的消息响应函数，响应 UM_SOCK 消息
//////////////////////////////////////////////////////////////////////////
public:
    NetMsg sendNetMsg;  //自己发出去的网络消息
	NetMsg recvNetMsg;  //对方发来的网络消息

public:
	void SendMsg(int type);   // 用这个向另一端发送消息，可以由 CMyFiveChess 类调用
	BOOL haveLink;  //为true表示已经在连接中

private:
	SOCKET srvSocket; //服务器套接字用来监听（接收连接）
	SOCKET clientSocket; //客户端套接字用来主动连接

	SOCKET sSocket;  //若作为服务器则还需要保存客户端的套接字，与之通信

	BOOL isSrv;  //为true表示作为服务器，否则为客户端

	BOOL InitSocket(); //创建并初始化套接字
	void DealMsg();   //处理接收到的消息

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RVIEW_H__10C169A3_EB04_4EEE_986A_1C08B46B3C6A__INCLUDED_)
