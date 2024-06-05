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
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	//װ��ͼ����Դ
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
	

	m_pClientSocket = new CClientSocket(this);		//����ͨѶ�����

	if (FALSE == ConnectServerAndGetPaper())		//���ӷ���������ȡ�Ծ�
	{
		EndDialog(FALSE);
		return FALSE;
	}
				
	
	InitPaperTitle();					//��ʼ���Ծ�ͷ

	InitPaper();						//��ʼ���Ծ�

	BeginExam();						//��ʼ���

	GetClientRect(m_rect);				//��ȡ�ͻ���
	SetScrollRange(SB_VERT, 0, 100);	//���ô�ֱ��������Χ
	SetTimer(1,1000 * 60, NULL);		//����ʱ���ʱ
	SetTimer(2,1000 * 7, NULL);			//����ʱ��
	m_bPulsePacket = TRUE;				//����������
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
 * ���ӷ������������Ծ�
 */
BOOL CClientDlg::ConnectServerAndGetPaper( void )
{
	if (IDOK != m_loginDlg.DoModal())//��ʾ��¼�������Ի���
	{
		return FALSE;
	}

	//���ӷ�����
	if (FALSE == m_pClientSocket->ConnectServer(m_loginDlg.m_lservIP, 
												m_loginDlg.m_servPort))
	{
		AfxMessageBox(_T("���ӷ�����ʧ�ܣ�"));
		return FALSE;
	}

	//���Ϳ���ѧ��
	HDR hdr;			//��ͷ
	hdr.ustype = STATE;	//״̬
	hdr.usLen =  sizeof(u_short) + m_loginDlg.m_strSN.GetLength();//״̬+ѧ�ų���	
	m_pClientSocket->SendPacket(&hdr, 
								CClientDlg::LOGIN,
								(char*)(LPCSTR)m_loginDlg.m_strSN);
	//���õ�¼״̬
	m_state = CClientDlg::LOGIN;

	//�����Ծ�
	return (m_pClientSocket->RecvPaper(m_strName, &m_listItem));
}

/*
 * ��ʼ���Ծ�
 */
void CClientDlg::InitPaper( void )
{
	//��ȡ�ͻ�������
	CRect clientRect;
	GetClientRect(&clientRect);	
	
	//����ÿ�����ⴰ��
	POSITION pos = NULL;
	int i = 0;
	for (pos = m_listItem.GetHeadPosition(); pos != NULL;)
	{
		CItem *pItem = (CItem*)m_listItem.GetNext(pos);
		
		//����ÿ����������
		CRect rect;
		rect.left = ITEMLEFT;						//��߽�
		rect.top = PAPERHEADHEIGHT + ITEMHEIGHT * i;//�ϱ߽�
		rect.right = rect.left + clientRect.Width() - ITEMLEFT;	//�ұ߽�
		rect.bottom = rect.top + ITEMHEIGHT;					//�±߽�
		
		//�������ⴰ��
		pItem->Create(IDD_ITEM_DLG,this);
		pItem->MoveWindow(rect,TRUE);
		pItem->ShowWindow(SW_SHOW );
		i++;
	}
	//�������ⴰ����ͻ����߶�֮��
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
 * ��ʱ����Ϣ
 */
void CClientDlg::OnTimer(UINT nIDEvent) 
{
	if (1== nIDEvent)//���Լ�ʱ
	{
		static int nTimes = 1;			//����ʱ�䣨���ӣ�
		int nLeftTime = 60 - nTimes;	//ʣ��ʱ��

		//��ʾʣ��ʱ��
		CString strLeftTime;
		strLeftTime.Format("%s%d%s", _T("ʣ��ʱ��: "),nLeftTime, _T("����"));
		CStatic *pStatic;
		pStatic = (CStatic*)GetDlgItem(IDC_CLIENT_TIME_STATIC);
		pStatic->SetWindowText(strLeftTime);
		nTimes++;

		//���Խ���ʱ�����
		if (60 == nTimes)
		{
			m_bPulsePacket = FALSE;//ֹͣ����������
			AfxMessageBox(_T("����ʱ�������"));
			SendResult();			//���ʹ�����
			OnOK();					//�˳�
			return;

		}
	}else if (2 == nIDEvent)//������ʱ��
	{
		//����������
		if(TRUE == m_bPulsePacket)
		{
			m_pClientSocket->SendPulsePacket();		
			
		}
	}
	CDialog::OnTimer(nIDEvent);
}

/*
 * ��������
 */
CClientDlg::~CClientDlg( void )
{
	//���������ռ�ڴ�
	POSITION pos = NULL;
	for (pos = m_listItem.GetHeadPosition(); pos != NULL;)
	{
		CItem *pItem = (CItem*)m_listItem.GetNext(pos);
		delete pItem;
		pItem = NULL;
	}
	m_listItem.RemoveAll();//ɾ���������нڵ�

	delete m_pClientSocket;//ɾ��ͨѶ�����
	m_pClientSocket = NULL;
}

/*
 * ��ʼ���
 */
void CClientDlg::BeginExam( void )
{
	//���Ϳ�ʼ������ݰ�
	HDR	hdr;
	hdr.ustype = STATE;//״̬
	hdr.usLen = sizeof(u_short);	
	m_pClientSocket->SendPacket( &hdr,CClientDlg::DOING, NULL);

	m_state = CClientDlg::DOING;	//����״̬
}

/*
 * ��ʼ���Ծ�ͷ
 */
void CClientDlg::InitPaperTitle( void )
{
	//�Ծ�����
	SetWindowText(_T("����ϵͳ---Client"));

	//ѧ��
	CString strTemp;
	strTemp = _T("ѧ��: ");
	strTemp += m_loginDlg.m_strSN;
	m_ctlSN.SetWindowText(strTemp);

	//����
	strTemp.Empty();
	strTemp = _T("������");
		strTemp += m_strName;
	m_ctlName.SetWindowText(strTemp);

	//ʣ��ʱ��
	strTemp.Empty();
	CString strLeftTime;
	strLeftTime.Format("%s%s", _T("ʣ��ʱ��: "), _T("60����"));
	strTemp += strLeftTime;
	m_ctlTime.SetWindowText(strTemp);

	//�ָ���
	m_ctlSep.SetWindowText("=======================================================================================================================");
	
}


/*
 * ���ʹ�����
 */
void CClientDlg::SendResult( void )
{
	CString	strResult;	//������
	CString	strItem;	//ÿ��������

	//��ȡ����������
	POSITION pos = NULL;
	for (pos = m_listItem.GetHeadPosition(); pos != NULL;)
	{
		CItem *pItem = (CItem*)m_listItem.GetNext(pos);
		strItem.Format("%d",pItem->m_nSelected);//��ȡ��	
		strResult += strItem;					//��������
	}

	//���ʹ�����
	HDR hdr;
	hdr.ustype = STATE;
	hdr.usLen = sizeof(u_short) + strResult.GetLength();
	m_pClientSocket->SendPacket(&hdr, CClientDlg::DONE, (char*)(LPCSTR)strResult);	
}

/*
 * ����
 */
void CClientDlg::OnOK() 
{
	if(IDYES == AfxMessageBox(_T("��ȷ�Ͻ�����"),MB_YESNO ))
	{
		SendResult();//���ʹ�����
	}else
	{
		return;
	}
	KillTimer(1);	//���ٶ�ʱ��
	KillTimer(2);	//���ٶ�ʱ��
	CDialog::OnOK();//���ٴ���
}


