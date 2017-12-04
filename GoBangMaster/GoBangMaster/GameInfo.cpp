// GameInfo.cpp : implementation file
//

#include "stdafx.h"
#include "MyFive.h"
#include "GameInfo.h"
#include "Grade.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GameInfo dialog


GameInfo::GameInfo(CWnd* pParent /*=NULL*/)
	: CDialog(GameInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(GameInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	isBlack = true;
	isForbid = true;
	isVoice = true;
}


void GameInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GameInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GameInfo, CDialog)
	//{{AFX_MSG_MAP(GameInfo)
	ON_BN_CLICKED(IDC_black, Onblack)
	ON_BN_CLICKED(IDC_white, Onwhite)
	ON_BN_CLICKED(IDC_haveForbid, OnhaveForbid)
	ON_BN_CLICKED(IDC_noForbid, OnnoForbid)
	ON_BN_CLICKED(IDC_haveVoice, OnhaveVoice)
	ON_BN_CLICKED(IDC_noVoice, OnnoVoice)
	ON_BN_CLICKED(IDC_AIGrade, OnAIGrade)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GameInfo message handlers

void GameInfo::Onblack() 
{
	isBlack = true;
}

void GameInfo::Onwhite() 
{
	isBlack = false;
}

void GameInfo::OnhaveForbid() 
{
	isForbid = true;
}

void GameInfo::OnnoForbid() 
{
	isForbid = false;
}

void GameInfo::OnhaveVoice() 
{
	isVoice = true;
}

void GameInfo::OnnoVoice() 
{
	isVoice = false;
}


BOOL GameInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(isBlack)
		CheckRadioButton(IDC_black,IDC_black,IDC_black);
	else
		CheckRadioButton(IDC_white,IDC_white,IDC_white);
	if(isForbid)
		CheckRadioButton(IDC_haveForbid,IDC_haveForbid,IDC_haveForbid);
	else
		CheckRadioButton(IDC_noForbid,IDC_noForbid,IDC_noForbid);
	if(isVoice)
		CheckRadioButton(IDC_haveVoice,IDC_haveVoice,IDC_haveVoice);
	else
		CheckRadioButton(IDC_noVoice,IDC_noVoice,IDC_noVoice);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void GameInfo::OnAIGrade() 
{
	m_grade.DoModal();
}
