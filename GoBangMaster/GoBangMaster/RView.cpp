// RView.cpp : implementation file
//

#include "stdafx.h"
#include "MyFive.h"
#include "RView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RView

IMPLEMENT_DYNCREATE(RView, CFormView)

RView::RView()
	: CFormView(RView::IDD)
{
	//{{AFX_DATA_INIT(RView)
	m_ChatContent = _T("");
	m_LocalPort = 6666;
	m_SrvPort = 7777;
	m_name = _T("路人甲");
	m_SendMsg = _T("");
	m_IP = _T("127.0.0.1");
	//}}AFX_DATA_INIT

	isSrv = TRUE; 
	haveLink = FALSE;	
}

RView::~RView()
{
	if(clientSocket)
		closesocket(clientSocket);
	if(srvSocket)
		closesocket(srvSocket);
	if(sSocket)
		closesocket(sSocket);
}

void RView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RView)
	DDX_Text(pDX, IDC_ChatContent, m_ChatContent);
	DDX_Text(pDX, IDC_LocalPort, m_LocalPort);
	DDX_Text(pDX, IDC_SrvPort, m_SrvPort);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_SendMsg, m_SendMsg);
	DDX_Text(pDX, IDC_IP, m_IP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RView, CFormView)
	//{{AFX_MSG_MAP(RView)
	ON_BN_CLICKED(IDC_StartServer, OnStartServer)
	ON_BN_CLICKED(IDC_LINK, OnLink)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_SOCK,OnSock)  //自定义
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RView diagnostics

#ifdef _DEBUG
void RView::AssertValid() const
{
	CFormView::AssertValid();
}

void RView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// RView message handlers

//创建并初始化套接字
BOOL RView::InitSocket()
{
	//建立sockete操作，建立流式套接字
	srvSocket = socket(AF_INET,SOCK_STREAM,0);
	clientSocket = socket(AF_INET,SOCK_STREAM,0);
	if(INVALID_SOCKET == srvSocket || INVALID_SOCKET == clientSocket)
	{
		AfxMessageBox(TEXT("创建套接字失败"));
		return FALSE;
	}

	//套接字 srvSocket 与本地地址相连
	SOCKADDR_IN addrSock;
	addrSock.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSock.sin_family = AF_INET;
	addrSock.sin_port = htons(m_LocalPort);

	//套接字 clientSocket 与本地地址相连
	SOCKADDR_IN addrSock2;
	addrSock2.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSock2.sin_family = AF_INET;
	addrSock2.sin_port = htons(m_LocalPort+rand()%1000); //

	if(SOCKET_ERROR == bind(srvSocket,(SOCKADDR*)&addrSock,sizeof(SOCKADDR)) ||
		SOCKET_ERROR == bind(clientSocket,(SOCKADDR*)&addrSock2,sizeof(SOCKADDR))
		)
	{
		AfxMessageBox(TEXT("绑定失败"));
		return FALSE;
	}

	//注册网络事件
	if(SOCKET_ERROR == WSAAsyncSelect(srvSocket,m_hWnd,UM_SOCK,FD_READ|FD_ACCEPT|FD_CLOSE|FD_CONNECT) ||
		SOCKET_ERROR == WSAAsyncSelect(clientSocket,m_hWnd,UM_SOCK,FD_READ|FD_ACCEPT|FD_CLOSE|FD_CONNECT)
		)
	{
		AfxMessageBox(TEXT("注册网络事件失败"));
		return FALSE;
	}

	//设置监听状态
	listen(srvSocket,10);	

	return TRUE;
}

//启动服务
void RView::OnStartServer() 
{
	UpdateData(TRUE);
	if(m_LocalPort<1025 || m_LocalPort>65535)
	{
		AfxMessageBox(TEXT("端口号在 1025 --- 65535 之间"));
		return ;
	}
	if(InitSocket())
	{
		GetDlgItem(IDC_LocalPort)->EnableWindow(FALSE);
		GetDlgItem(IDC_StartServer)->EnableWindow(FALSE);
	
		GetDlgItem(IDC_LINK)->EnableWindow(TRUE);
		GetDlgItem(IDC_IP)->EnableWindow(TRUE);
		GetDlgItem(IDC_SrvPort)->EnableWindow(TRUE);
		GetDlgItem(IDC_NAME)->EnableWindow(TRUE);

		AfxMessageBox(TEXT("启动服务成功"));
	}
}


// UM_SOCK消息的响应函数
LRESULT RView::OnSock(WPARAM wParam,LPARAM lParam)
{
	int len;
	int iErrorCode;

	switch (LOWORD(lParam))
	{
	case FD_ACCEPT:
		{
			if(haveLink)  //已经连接
				break;
			SOCKADDR_IN addrClient;
			len = sizeof(SOCKADDR);
			sSocket = accept(srvSocket,(SOCKADDR*)&addrClient,&len);
			haveLink = TRUE; //已连接
			isSrv = TRUE;  //作为服务器

			SendMsg(1); //发送登录消息

			AfxMessageBox(TEXT("被客户端连接成功"));
		}
		break;

	case FD_READ:
		{
			int iRecv = 0,nCharRecv;
			if(isSrv)  //作为服务器，那么与客户端通信的套接字是 sSocket 
			{
				while(iRecv<sizeof(NetMsg)) //刚好接收一个结构体
				{
					nCharRecv = recv(sSocket,(char*)(&recvNetMsg+iRecv),sizeof(NetMsg)-iRecv,0);
					iRecv += nCharRecv;
				}
			}
			else  // 这里通信的套接字是 clientSocket
			{
				while(iRecv<sizeof(NetMsg))
				{
					nCharRecv = recv(clientSocket,(char*)(&recvNetMsg+iRecv),sizeof(NetMsg)-iRecv,0);
					iRecv += nCharRecv;
				}
			}
			DealMsg();  //处理消息
		}
		break;

	case FD_CLOSE:
		{	
			if(isSrv)
				closesocket(sSocket);
			else
				closesocket(clientSocket);

			AfxMessageBox(TEXT("断开连接"));
		}
		break;

	case FD_CONNECT:
		{
			iErrorCode = WSAGETSELECTERROR(lParam);
			if(0 == iErrorCode)
			{
				haveLink = TRUE; //已连接
				isSrv = FALSE;  //作为客户端

				SendMsg(1); //发送登录消息
				AfxMessageBox(TEXT("连接服务器成功"));
			}
			else
				AfxMessageBox(TEXT("连接服务器失败"));
		}
		break;
	}

	return true;
}


//连接
void RView::OnLink() 
{
	if(haveLink)
	{
		AfxMessageBox(TEXT("已经连接"));
		return ;
	}

	UpdateData(TRUE);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(m_IP); //服务器的IP
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(m_SrvPort);  //服务器的端口

	//向服务器发出连接请求
	connect(clientSocket,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
}


/*   
    NetMsg 结构体如下：
	int type;   //消息类型
	char name[20]; //昵称(登录消息）
	bool is_black;  //为true表示执黑
	bool is_forbid;  // 为true表示为有禁手
	int row,col;  //下棋的位置
	char str[100];  //聊天的字符串
*/

//发送消息，包括下棋登录消息，下棋的消息，聊天消息，悔棋消息
void RView::SendMsg(int type)
{		
	if(!haveLink)
		return ;
	
	// .... 对 netMsg 赋值
	memset(&sendNetMsg,0,sizeof(sendNetMsg)); //
	sendNetMsg.type = type;
	char *pName = m_name.GetBuffer(m_name.GetLength()+1);
	memcpy(sendNetMsg.name,pName,m_name.GetLength()+1);

	char *pChat = m_SendMsg.GetBuffer(m_SendMsg.GetLength()+1);
	memcpy(sendNetMsg.str,pChat,m_SendMsg.GetLength()+1);

	CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();  
	sendNetMsg.row = pFrm->m_MyFiveView->fiveChess.currentX;
	sendNetMsg.col = pFrm->m_MyFiveView->fiveChess.currentY;
	sendNetMsg.is_black = pFrm->m_MyFiveView->gameInfo.isBlack;
	sendNetMsg.is_forbid = pFrm->m_MyFiveView->gameInfo.isForbid;
	
	
	int iSend = 0,nCharSend;
	if(isSrv)
	{
		while(iSend<sizeof(NetMsg))
		{
			nCharSend = send(sSocket,(char*)(&sendNetMsg+iSend),sizeof(NetMsg)-iSend,0);
			iSend += nCharSend;
		}
	}
	else
	{
		while(iSend<sizeof(NetMsg))
		{
			nCharSend = send(clientSocket,(char*)(&sendNetMsg+iSend),sizeof(NetMsg)-iSend,0);
			iSend += nCharSend;
		}
	}
}


//点击发送按钮，发送的是聊天消息
void RView::OnSend() 
{
	GetDlgItemText(IDC_SendMsg,m_SendMsg);
	if("" == m_SendMsg)
	{
		AfxMessageBox(TEXT("发送消息不能为空"));
		return ;
	}
	SendMsg(3);  //发送聊天消息

	CString temp;
	temp.Format(TEXT("我说：%s"),sendNetMsg.str);
	temp += "\r\n";

	CString sTime;
	SYSTEMTIME st;
	GetLocalTime(&st);  //得到系统时间
	sTime.Format(TEXT("%d:%d:%d"),st.wHour,st.wMinute,st.wSecond);
	sTime += "\r\n";

	m_ChatContent = sTime + temp + m_ChatContent;
	SetDlgItemText(IDC_ChatContent,m_ChatContent);

	SetDlgItemText(IDC_SendMsg,"");
}

//处理接收到的消息
void RView::DealMsg()
{
	CString temp;
	if(1 == recvNetMsg.type) //处理登录消息
	{
		temp.Format(TEXT("%s和你已连接!"),recvNetMsg.name);
		temp += "\r\n";
		
		CString sTime;
		SYSTEMTIME st;
		GetLocalTime(&st);  //得到系统时间
		sTime.Format(TEXT("%d:%d:%d"),st.wHour,st.wMinute,st.wSecond);
		sTime += "\r\n";
		
		m_ChatContent = sTime + temp + m_ChatContent;
		SetDlgItemText(IDC_ChatContent,m_ChatContent);
	}
	else if( 2 == recvNetMsg.type)  //处理下棋消息
	{
		CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
		pFrm->m_MyFiveView->DealNetBattle();  
	}
	else if(3 == recvNetMsg.type) //处理聊天消息
	{
		temp.Format(TEXT("%s说：%s"),recvNetMsg.name,recvNetMsg.str);
		temp += "\r\n";
		
		CString sTime;
		SYSTEMTIME st;
		GetLocalTime(&st);  //得到系统时间
		sTime.Format(TEXT("%d:%d:%d"),st.wHour,st.wMinute,st.wSecond);
		sTime += "\r\n";
		
		m_ChatContent = sTime + temp + m_ChatContent;
		SetDlgItemText(IDC_ChatContent,m_ChatContent);
	}
	else if(4 == recvNetMsg.type) //处理悔棋消息
	{
		CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
		pFrm->m_MyFiveView->fiveChess.RetractChess();
		pFrm->m_MyFiveView->Invalidate(0); //更新棋盘
	}
}

