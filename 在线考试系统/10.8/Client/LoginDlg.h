#if !defined(AFX_LOGINDLG_H__FD98E244_9B12_4C8D_A956_1AFAB372D25E__INCLUDED_)
#define AFX_LOGINDLG_H__FD98E244_9B12_4C8D_A956_1AFAB372D25E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

class CLoginDlg : public CDialog
{
// Construction
public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLoginDlg)
	enum { IDD = IDD_CLIENT_LOGIN_DIALOG };
	CIPAddressCtrl	m_ctlServIP;
	short	m_servPort;
	CString m_strSN;//Ñ§ºÅ
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoginDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual void OnOK(void);

public:
	DWORD	m_lservIP;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDLG_H__FD98E244_9B12_4C8D_A956_1AFAB372D25E__INCLUDED_)
