// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "Client.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_servPort = SERVERPORT;
	m_strSN = "0001";
	//}}AFX_DATA_INIT
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Control(pDX, IDC_CLIENT_IPADDRESS, m_ctlServIP);
	DDX_Text(pDX, IDC_CLIENT_PORT_EDIT, m_servPort);
	DDV_MinMaxInt(pDX, m_servPort, 5000, 8000);
	DDX_Text(pDX, IDC_CLIENT_SN_EDIT, m_strSN);
	DDV_MaxChars(pDX, m_strSN, 4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers


BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	
	m_ctlServIP.SetAddress(127, 0, 0, 1);

	return TRUE; 
}


void CLoginDlg::OnOK(void)
{
	UpdateData(TRUE);
	CString strIP;
	m_ctlServIP.GetWindowText(strIP);

	if (strIP.IsEmpty() ||  m_servPort == 0 || m_strSN.IsEmpty())
	{
		AfxMessageBox(_T("字段不能为空"));
		return;
	}
	if(SNLEN != m_strSN.GetLength())
	{
		AfxMessageBox(_T("学号为4位数字"));
		return;
	}

	m_ctlServIP.GetAddress(m_lservIP);
	CDialog::OnOK();
}
