// ServerDoc.cpp : implementation of the CServerDoc class
//

#include "stdafx.h"
#include "Server.h"

#include "ServerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerDoc

IMPLEMENT_DYNCREATE(CServerDoc, CDocument)

BEGIN_MESSAGE_MAP(CServerDoc, CDocument)
	//{{AFX_MSG_MAP(CServerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDoc construction/destruction

CServerDoc::CServerDoc()
{
	// TODO: add one-time construction code here

}

CServerDoc::~CServerDoc()
{
}

BOOL CServerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	SetTitle(_T("计算机文化基础考试系统"));
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CServerDoc serialization

void CServerDoc::Serialize(CArchive& ar)
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
// CServerDoc diagnostics

#ifdef _DEBUG
void CServerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CServerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CServerDoc commands

void CServerDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDocument::OnCloseDocument();
}
