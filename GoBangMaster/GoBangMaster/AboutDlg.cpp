// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyFive.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AboutDlg dialog


AboutDlg::AboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(AboutDlg)
	m_Content = _T("");
	//}}AFX_DATA_INIT

	m_Content = TEXT("用户须知: \r\n\r\n\
	在菜单栏上：设置 --> 对战模式 --> 开始游戏。\r\n\r\n\
        若是“网络模式”，请先确保双方电脑已经网络互连。\r\n\
然后在右边进行相应设置，一方作为服务器，另一方作为客户端：\r\n\r\n\
(1)若是作为服务器，先输入端口号，然后点击“启动服务”按钮。\r\n\
(2)若是作为客户端，完成步骤(1)后,还需要输入服务器的IP和端口号，然后点击“连接”按钮。\
连接成功后便可网络对战了。注意的是，双方的禁手规则必须相同，执棋颜色不能冲突。\
当然程序会检查并提醒玩家的，下面一个框可以和对方聊天。\r\n\r\n\
        若是“单机模式”，不解释。\r\n\r\n\
注意:\r\n\
(1)目前程序只能处理一重禁手，对于多重禁手需要玩家自己判断。\r\n\
(2)AI并不是地毯式的，仅仅融合了本人的部分五子棋思想，或许很容易被高手打败。\r\n\
(3)建议玩家下禁手规则的五子棋。\r\n\r\n\
如有问题或建议可与我联系：darksword@foxmail.com \r\n\r\n\
					李芥\r\n\
						2013.2.13\
	");  

}


void AboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AboutDlg)
	DDX_Text(pDX, IDC_Content, m_Content);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AboutDlg, CDialog)
	//{{AFX_MSG_MAP(AboutDlg)
	ON_BN_CLICKED(IDC_BOKE, OnBoke)
	ON_BN_CLICKED(IDC_OK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AboutDlg message handlers

void AboutDlg::OnBoke() 
{
	ShellExecute(NULL,"open",_T("http://www.darkswordzone.com/"),NULL,NULL,SW_SHOWNORMAL);
}


void AboutDlg::OnOk() 
{
	this->OnCancel();
}
