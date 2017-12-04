// MyFiveDoc.h : interface of the CMyFiveDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYFIVEDOC_H__E5C73AD4_29BB_4B7F_85BF_F6DE44CAE925__INCLUDED_)
#define AFX_MYFIVEDOC_H__E5C73AD4_29BB_4B7F_85BF_F6DE44CAE925__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyFiveDoc : public CDocument
{
protected: // create from serialization only
	CMyFiveDoc();
	DECLARE_DYNCREATE(CMyFiveDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFiveDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyFiveDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyFiveDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFIVEDOC_H__E5C73AD4_29BB_4B7F_85BF_F6DE44CAE925__INCLUDED_)
