// MyFiveDoc.cpp : implementation of the CMyFiveDoc class
//

#include "stdafx.h"
#include "MyFive.h"

#include "MyFiveDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFiveDoc

IMPLEMENT_DYNCREATE(CMyFiveDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyFiveDoc, CDocument)
	//{{AFX_MSG_MAP(CMyFiveDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFiveDoc construction/destruction

CMyFiveDoc::CMyFiveDoc()
{
	// TODO: add one-time construction code here

}

CMyFiveDoc::~CMyFiveDoc()
{
}

BOOL CMyFiveDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMyFiveDoc serialization

void CMyFiveDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyFiveDoc diagnostics

#ifdef _DEBUG
void CMyFiveDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyFiveDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyFiveDoc commands
