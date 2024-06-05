// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "Item.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent),
	m_bPulsePacket(FALSE),
	m_state(UNKNOWN),
	m_pClientSocket(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	//装载图标资源
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDlg)
	DDX_Control(pDX, IDC_CLIENT_SEP_STATIC, m_ctlSep);
	DDX_Control(pDX, IDC_CLIENT_TIME_STATIC, m_ctlTime);
	DDX_Control(pDX, IDC_CLIENT_SN_STATIC, m_ctlSN);
	DDX_Control(pDX, IDC_CLIENT_NAME_STATIC, m_ctlName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	//{{AFX_MSG_MAP(CClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_VSCROLL()
	ON_WM_TIMER()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	

	m_pClientSocket = new CClientSocket(this);		//创建通讯类对象

	if (FALSE == ConnectServerAndGetPaper())		//连接服务器，获取试卷
	{
		EndDialog(FALSE);
		return FALSE;
	}
				
	
	InitPaperTitle();					//初始化试卷头

	InitPaper();						//初始化试卷

	BeginExam();						//开始答卷

	GetClientRect(m_rect);				//获取客户区
	SetScrollRange(SB_VERT, 0, 100);	//设置垂直滚动条范围
	SetTimer(1,1000 * 60, NULL);		//考试时间计时
	SetTimer(2,1000 * 7, NULL);			//心跳时间
	m_bPulsePacket = TRUE;				//发送心跳包
	return TRUE; 
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/*
 * 连接服务器并接收试卷
 */
BOOL CClientDlg::ConnectServerAndGetPaper( void )
{
	if (IDOK != m_loginDlg.DoModal())//显示登录服务器对话框
	{
		return FALSE;
	}

	//连接服务器
	if (FALSE == m_pClientSocket->ConnectServer(m_loginDlg.m_lservIP, 
												m_loginDlg.m_servPort))
	{
		AfxMessageBox(_T("连接服务器失败！"));
		return FALSE;
	}

	//发送考生学号
	HDR hdr;			//包头
	hdr.ustype = STATE;	//状态
	hdr.usLen =  sizeof(u_short) + m_loginDlg.m_strSN.GetLength();//状态+学号长度	
	m_pClientSocket->SendPacket(&hdr, 
								CClientDlg::LOGIN,
								(char*)(LPCSTR)m_loginDlg.m_strSN);
	//设置登录状态
	m_state = CClientDlg::LOGIN;

	//接收试卷
	return (m_pClientSocket->RecvPaper(m_strName, &m_listItem));
}

/*
 * 初始化试卷
 */
void CClientDlg::InitPaper( void )
{
	//获取客户端区域
	CRect clientRect;
	GetClientRect(&clientRect);	
	
	//创建每个试题窗口
	POSITION pos = NULL;
	int i = 0;
	for (pos = m_listItem.GetHeadPosition(); pos != NULL;)
	{
		CItem *pItem = (CItem*)m_listItem.GetNext(pos);
		
		//设置每道试题区域
		CRect rect;
		rect.left = ITEMLEFT;						//左边界
		rect.top = PAPERHEADHEIGHT + ITEMHEIGHT * i;//上边界
		rect.right = rect.left + clientRect.Width() - ITEMLEFT;	//右边界
		rect.bottom = rect.top + ITEMHEIGHT;					//下边界
		
		//创建试题窗口
		pItem->Create(IDD_ITEM_DLG,this);
		pItem->MoveWindow(rect,TRUE);
		pItem->ShowWindow(SW_SHOW );
		i++;
	}
	//所有试题窗口与客户区高度之差
	m_offsetHeight = PAPERHEADHEIGHT + 
					ITEMHEIGHT * (m_listItem.GetCount()) -
					clientRect.Height();
	
}

void CClientDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nCurPos;  
	int nPrevPos;  
	CRect rect;  
	
	GetClientRect(&rect);  
	nPrevPos   =   GetScrollPos(SB_VERT);  
	nCurPos   =   nPrevPos;  
	switch(nSBCode)  
	{  
	case SB_PAGELEFT:
		{
			nCurPos = nPrevPos - 20;  
			if(nCurPos < 0)
			{
				nCurPos = 0;  
			}			
			SetScrollPos(SB_VERT, nCurPos);  
			break;
		}  
	case SB_PAGERIGHT:
		{
			nCurPos = nPrevPos + 20;  
			if(nCurPos > 100)
			{
				nCurPos = 100;  
			}				
			SetScrollPos(SB_VERT, nCurPos);
			break;  
		}		
	case SB_THUMBPOSITION:
		{
			SetScrollPos(SB_VERT, nPos);  
			break;
		}
	case SB_THUMBTRACK:  
		break;  
	case SB_LINELEFT:
		{
			nCurPos = nPrevPos - 5;  
			if(nCurPos < 0)
			{
				nCurPos = 0;  
			}				
			SetScrollPos(SB_VERT, nCurPos);  
			break;
		}  
	case SB_LINERIGHT:
		{
			nCurPos = nPrevPos + 5;  
			if(nCurPos > 100)
			{
				nCurPos = 100;  
			}				
			SetScrollPos(SB_VERT, nCurPos);  
			break;
		}		  
	case SB_ENDSCROLL:  
		break;  
	}  
	nCurPos = GetScrollPos(SB_VERT);  
	ScrollWindow(0, m_offsetHeight * (nPrevPos -  nCurPos) /100);  
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar); 
}

/*
 * 定时器消息
 */
void CClientDlg::OnTimer(UINT nIDEvent) 
{
	if (1== nIDEvent)//考试计时
	{
		static int nTimes = 1;			//答题时间（分钟）
		int nLeftTime = 60 - nTimes;	//剩余时间

		//显示剩余时间
		CString strLeftTime;
		strLeftTime.Format("%s%d%s", _T("剩余时间: "),nLeftTime, _T("分钟"));
		CStatic *pStatic;
		pStatic = (CStatic*)GetDlgItem(IDC_CLIENT_TIME_STATIC);
		pStatic->SetWindowText(strLeftTime);
		nTimes++;

		//考试结束时间结束
		if (60 == nTimes)
		{
			m_bPulsePacket = FALSE;//停止发送心跳包
			AfxMessageBox(_T("考试时间结束。"));
			SendResult();			//发送答题结果
			OnOK();					//退出
			return;

		}
	}else if (2 == nIDEvent)//心跳包时间
	{
		//发送心跳包
		if(TRUE == m_bPulsePacket)
		{
			m_pClientSocket->SendPulsePacket();		
			
		}
	}
	CDialog::OnTimer(nIDEvent);
}

/*
 * 析构函数
 */
CClientDlg::~CClientDlg( void )
{
	//清除试题所占内存
	POSITION pos = NULL;
	for (pos = m_listItem.GetHeadPosition(); pos != NULL;)
	{
		CItem *pItem = (CItem*)m_listItem.GetNext(pos);
		delete pItem;
		pItem = NULL;
	}
	m_listItem.RemoveAll();//删除链表所有节点

	delete m_pClientSocket;//删除通讯类对象
	m_pClientSocket = NULL;
}

/*
 * 开始答卷
 */
void CClientDlg::BeginExam( void )
{
	//发送开始答卷数据包
	HDR	hdr;
	hdr.ustype = STATE;//状态
	hdr.usLen = sizeof(u_short);	
	m_pClientSocket->SendPacket( &hdr,CClientDlg::DOING, NULL);

	m_state = CClientDlg::DOING;	//设置状态
}

/*
 * 初始化试卷头
 */
void CClientDlg::InitPaperTitle( void )
{
	//试卷名称
	SetWindowText(_T("考试系统---Client"));

	//学号
	CString strTemp;
	strTemp = _T("学号: ");
	strTemp += m_loginDlg.m_strSN;
	m_ctlSN.SetWindowText(strTemp);

	//姓名
	strTemp.Empty();
	strTemp = _T("姓名：");
		strTemp += m_strName;
	m_ctlName.SetWindowText(strTemp);

	//剩余时间
	strTemp.Empty();
	CString strLeftTime;
	strLeftTime.Format("%s%s", _T("剩余时间: "), _T("60分钟"));
	strTemp += strLeftTime;
	m_ctlTime.SetWindowText(strTemp);

	//分割线
	m_ctlSep.SetWindowText("=======================================================================================================================");
	
}


/*
 * 发送答题结果
 */
void CClientDlg::SendResult( void )
{
	CString	strResult;	//答题结果
	CString	strItem;	//每道试题结果

	//获取所有试题结果
	POSITION pos = NULL;
	for (pos = m_listItem.GetHeadPosition(); pos != NULL;)
	{
		CItem *pItem = (CItem*)m_listItem.GetNext(pos);
		strItem.Format("%d",pItem->m_nSelected);//获取答案	
		strResult += strItem;					//加入结果中
	}

	//发送答题结果
	HDR hdr;
	hdr.ustype = STATE;
	hdr.usLen = sizeof(u_short) + strResult.GetLength();
	m_pClientSocket->SendPacket(&hdr, CClientDlg::DONE, (char*)(LPCSTR)strResult);	
}

/*
 * 交卷
 */
void CClientDlg::OnOK() 
{
	if(IDYES == AfxMessageBox(_T("您确认交卷吗"),MB_YESNO ))
	{
		SendResult();//发送答题结果
	}else
	{
		return;
	}
	KillTimer(1);	//销毁定时器
	KillTimer(2);	//销毁定时器
	CDialog::OnOK();//销毁窗口
}


