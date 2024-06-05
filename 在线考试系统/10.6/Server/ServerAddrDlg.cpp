// ServerAddrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerAddrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerAddrDlg dialog


CServerAddrDlg::CServerAddrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerAddrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerAddrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CServerAddrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerAddrDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_SERVER_IPADDRESS, m_ctlServIP);
	DDX_Control(pDX, IDC_SERVER_PORT_EDIT, m_ctlServPort);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerAddrDlg, CDialog)
	//{{AFX_MSG_MAP(CServerAddrDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerAddrDlg message handlers

void CServerAddrDlg::OnOK() 
{
	CString strServPort;
	m_ctlServPort.GetWindowText(strServPort);//端口字符串
	if (strServPort.IsEmpty())
	{
		AfxMessageBox("请填写服务器端口。");
		return;
	}
	//获取服务器IP
	m_ctlServIP.GetAddress(m_dwServIP);
	if (0 == m_dwServIP)
	{
		AfxMessageBox("请填写服务器地址。");
		return;
	}
	//获取服务器端口
	m_shServPort = atoi(strServPort);
	
	CDialog::OnOK();
}

BOOL CServerAddrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//初始化变量
	m_ctlServIP.SetAddress(127, 0, 0, 1);
	CString strServIP;
	strServIP.Format("%d",SERVERPORT);
	m_ctlServPort.SetWindowText(strServIP);	

	return TRUE; 
}