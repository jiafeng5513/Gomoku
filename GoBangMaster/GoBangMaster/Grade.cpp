// Grade.cpp : implementation file
//

#include "stdafx.h"
#include "MyFive.h"
#include "Grade.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Grade dialog


Grade::Grade(CWnd* pParent /*=NULL*/)
	: CDialog(Grade::IDD, pParent)
{
	//{{AFX_DATA_INIT(Grade)
	m_depth = 4;
	m_de_depth = 4;
	m_depth_end = 14;
	m_depth_dis = 2;
	m_de_depth_dis = 2;
	m_de_depth_end = 14;
	//}}AFX_DATA_INIT

	grade = 2;

	// ¹¥»÷
	VCDEPTH = 4;
	VCDEPTH_END = 14;
	VCDEPTH_DIS = 2;

	// ·ÀÓù
	VCDE_DEPTH = 4;
	VCDE_DEPTH_END = 14;
	VCDE_DEPTH_DIS = 2;

}


void Grade::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Grade)
	DDX_Text(pDX, IDC_DEPTH, m_depth);
	DDV_MinMaxUInt(pDX, m_depth, 1, 100);
	DDX_Text(pDX, IDC_DeDEPTH, m_de_depth);
	DDV_MinMaxUInt(pDX, m_de_depth, 1, 100);
	DDX_Text(pDX, IDC_DEPTH_END, m_depth_end);
	DDV_MinMaxUInt(pDX, m_depth_end, 1, 100);
	DDX_Text(pDX, IDC_DEPTH_DIS, m_depth_dis);
	DDV_MinMaxUInt(pDX, m_depth_dis, 1, 100);
	DDX_Text(pDX, IDC_DeDEPTH_DIS, m_de_depth_dis);
	DDV_MinMaxUInt(pDX, m_de_depth_dis, 1, 100);
	DDX_Text(pDX, IDC_DeDEPTH_END, m_de_depth_end);
	DDV_MinMaxUInt(pDX, m_de_depth_end, 1, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Grade, CDialog)
	//{{AFX_MSG_MAP(Grade)
	ON_BN_CLICKED(IDC_Grade1, OnGrade1)
	ON_BN_CLICKED(IDC_Grade2, OnGrade2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Grade message handlers

void Grade::OnGrade1() 
{
	grade = 1;
}

void Grade::OnGrade2() 
{
	grade = 2;
}

BOOL Grade::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if(1 == grade)
		CheckRadioButton(IDC_Grade1,IDC_Grade1,IDC_Grade1);
	else if(2 == grade)
		CheckRadioButton(IDC_Grade2,IDC_Grade2,IDC_Grade2);

	m_depth = VCDEPTH;
	m_depth_end = VCDEPTH_END;
	m_depth_dis = VCDEPTH_DIS;

	m_de_depth = VCDE_DEPTH;
	m_de_depth_end = VCDE_DEPTH_END;
	m_de_depth_dis = VCDE_DEPTH_DIS;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Grade::OnOK() 
{

	UpdateData(TRUE);
	
	if(m_depth<=100 && m_depth>=1)
		VCDEPTH = m_depth; 
	if(m_depth_end<=100 && m_depth_end>=1)
		VCDEPTH_END = m_depth_end;
	if(m_depth_dis<=100 && m_depth_dis>=1)
		VCDEPTH_DIS = m_depth_dis;

	if(m_de_depth<=100 && m_de_depth>=1)
		VCDE_DEPTH = m_de_depth; 
	if(m_de_depth_end<=100 && m_de_depth_end>=1)
		VCDE_DEPTH_END = m_de_depth_end;
	if(m_de_depth_dis<=100 && m_de_depth_dis>=1)
		VCDE_DEPTH_DIS = m_de_depth_dis;

	CDialog::OnOK();
}
