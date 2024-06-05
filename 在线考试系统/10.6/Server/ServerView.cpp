// ServerView.cpp : implementation of the CServerView class
//

#include "stdafx.h"
#include "Server.h"

#include "ServerDoc.h"
#include "ServerView.h"
#include "ServerAddrDlg.h"
#include "ClientContext.h"
#include "ClientManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CServerApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CServerView

IMPLEMENT_DYNCREATE(CServerView, CListView)

BEGIN_MESSAGE_MAP(CServerView, CListView)
	//{{AFX_MSG_MAP(CServerView)
	ON_COMMAND(ID_SERVER_START, OnServerStart)
	ON_UPDATE_COMMAND_UI(ID_SERVER_START, OnUpdateServerStart)
// 	ON_COMMAND(ID_SERVER_EXIT, OnServerExit)
// 	ON_UPDATE_COMMAND_UI(ID_SERVER_EXIT, OnUpdateServerExit)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerView construction/destruction

CServerView::CServerView():
m_sListen(INVALID_SOCKET),	//�����׽���
m_hCompPort(NULL),			//��ɶ˿�
m_bRunning(FALSE),			//����������״̬
m_nThreadNum(0),			//���߳�����
m_usPaperLen(0)				//�Ծ���
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));	//������¼��ָ��
	ZeroMemory(m_cPaper, PAPER_BUFFER_SIZE);			//���⻺����
	ZeroMemory(m_cAnswer, ANSWER_BUFFER_SIZE);			//�𰸻�����
	ZeroMemory(m_hThread, MAX_SUBTHREAD_SIZE);			//���߳̾��
}

CServerView::~CServerView()
{

}

BOOL CServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.cx = 50;
	cs.cy = 600;
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CServerView drawing

void CServerView::OnDraw(CDC* pDC)
{
	CServerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/*
 * ��ʼ��
 */
void CServerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

 	InitListCtrlSetting();					//��ʼ���б���ͼ�ؼ�
 
 	InitListCtl();							//�����б���ͼ�ؼ�

 	ReadPaper(m_cPaper, m_cAnswer);			//��ȡ�Ծ�
	m_usPaperLen = strlen(m_cPaper);		//�Ծ���
}

/////////////////////////////////////////////////////////////////////////////
// CServerView printing



/////////////////////////////////////////////////////////////////////////////
// CServerView diagnostics

#ifdef _DEBUG
void CServerView::AssertValid() const
{
	CListView::AssertValid();
}

void CServerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CServerDoc* CServerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CServerDoc)));
	return (CServerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CServerView message handlers

/*
 * ��ʼ���б���ͼ�ؼ�
 */
void CServerView::InitListCtrlSetting( void )
{
	CListCtrl &listCtl = GetListCtrl();							//�õ��ؼ�
	
	LONG lStyle = GetWindowLong(listCtl.m_hWnd, GWL_STYLE);		//��ȡ���
	lStyle |= LVS_REPORT;										//���ӻ㱨���
	SetWindowLong(listCtl.m_hWnd, GWL_STYLE, lStyle);			//����style
	
	DWORD dwStyle = ListView_GetExtendedListViewStyle(listCtl);	//�õ���չ���
	dwStyle|= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;			//����ѡ�����������߷��
	ListView_SetExtendedListViewStyle (listCtl, dwStyle);
	
	CRect clientRect;
	listCtl.GetClientRect(&clientRect);//��ȡ�ͻ�����
	
	int			nCol;	//��
	LV_COLUMN	lvCol;	//�ṹ
	lvCol.mask = LVCF_FMT |LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	//��һ��
	nCol = 0;
	lvCol.iSubItem = nCol;
	lvCol.pszText = "ѧ��";
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = (float)clientRect.Width()/100 * 25;
	listCtl.InsertColumn(nCol, &lvCol);
	
	//�ڶ���
	nCol = 1;
	lvCol.iSubItem = nCol;
	lvCol.pszText = "����";
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = (float)clientRect.Width()/100 * 25;
	listCtl.InsertColumn(nCol, &lvCol);
	
	//������
	nCol = 2;
	lvCol.iSubItem = nCol;
	lvCol.pszText = "״̬";
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = (float)clientRect.Width()/100 * 25;
	listCtl.InsertColumn(nCol, &lvCol);
	
	//������
	nCol = 3;
	lvCol.iSubItem = nCol;
	lvCol.pszText = "�ɼ�";
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = (float)clientRect.Width()/100 * 25;
	listCtl.InsertColumn(nCol, &lvCol);
}


/*
 * �����б���ͼ�ؼ�
 */
void CServerView::InitListCtl( void )
{
	CListCtrl &listCtl = GetListCtrl();				//�õ��б���ͼ�ؼ�

	try 
	{
		int nIndex=0;								//��¼���		
		CString strSQL = "select * from Student";	//��ȡ���м�¼
		m_pRecordSet->Open(_variant_t(strSQL),
							theApp.m_pConnection.GetInterfacePtr(), 
							adOpenDynamic,
							adLockOptimistic,
							adCmdText);
		
		while (!m_pRecordSet->adoEOF)//��¼β
		{
			_variant_t var;			//��ʱ����
			CString strSN;			//ѧ��
			CString strName;		//����
			CString strState;		//״̬
			CString	strGrade;		//�ɼ�
			
			//��ȡ��ѧ�š��ֶ�
			var = m_pRecordSet->GetCollect("STD_SN");			
			if (var.vt != VT_NULL)
			{
				strSN = (LPCSTR)_bstr_t(var);
			}
			//��ȡ���������ֶ�
			var = m_pRecordSet->GetCollect("STD_NAME");	
			if (var.vt != VT_NULL)
			{
				strName = (LPCSTR)_bstr_t(var);
 			}

			//��ȡ��״̬���ֶ�
			var = m_pRecordSet->GetCollect("STD_STATE");
			if (var.vt != VT_NULL)
			{
				strState = (LPCSTR)_bstr_t(var);
			}
			//��ȡ���ɼ����ֶ�
			var = m_pRecordSet->GetCollect("STD_GRADE");
			if (var.vt != VT_NULL)
			{
				strGrade = (LPCSTR)_bstr_t(var);
			}
			//����������			
			listCtl.InsertItem(nIndex, strSN);			//ѧ��
			listCtl.SetItemText(nIndex, 1, strName);	//����
			listCtl.SetItemText(nIndex, 2, strState);	//״̬
			listCtl.SetItemText(nIndex, 3, strGrade);	//�ɼ�

			//���忼����Ϣ
			PSTUDENTINFO  pStdIn = new STUDENTINFO;
			pStdIn->strSN = strSN;						//ѧ��
			pStdIn->strName = strName;					//����
			pStdIn->strState = strState;				//״̬
			pStdIn->usGrade = atoi(strGrade);			//�ɼ�
			m_vectStudent.push_back(pStdIn);			//����vector��

			m_pRecordSet->MoveNext();					//�ƶ���¼
			nIndex++;									//��¼��ż�1
		}		
		m_pRecordSet->Close();//�ر�		
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	catch(...)
	{
		AfxMessageBox("�������ݿ������ʱ�����쳣��");
		return;
	}
}

/*
 * ��ȡ�Ծ�
 */
BOOL CServerView::ReadPaper( char *pPaper , char *pAnswer)
{
	ASSERT(pAnswer && pAnswer);
	
	char	*pPaperItem = pPaper;	//���⻺����ָ��
	char	*pAnswerItem = pAnswer;	//�𰸻�����ָ��
	try 
	{
		int nIndex=0;							//��¼���		
		CString strSQL = "select * from Paper";	//��ȡ���м�¼
		m_pRecordSet->Open(_variant_t(strSQL),
							theApp.m_pConnection.GetInterfacePtr(), 
							adOpenDynamic,
							adLockOptimistic,
							adCmdText);
		
		while (!m_pRecordSet->adoEOF)	//��¼β
		{
			_variant_t var;				//��ʱ����
			char *pTemp = NULL;
			int	len = 0;		;
			strcpy(pPaperItem, "<");	//���⿪ʼ�ָ���
			pPaperItem++;
			
			//��ȡ����š��ֶ�
			var = m_pRecordSet->GetCollect("PAP_ID");			
			if (var.vt != VT_NULL)
			{
				pTemp =  _com_util::ConvertBSTRToString(_bstr_t(var));
				len = strlen(pTemp);			//�ַ�������
				strcpy(pPaperItem, pTemp);		//�����ַ���
				strcpy(pPaperItem + len, "|");	//���Ʒָ���
				pPaperItem += len + 1;			//�ƶ�ָ��
				len = 0;
				delete pTemp;
				pTemp = NULL;
			}
			pTemp = NULL;
			//��ȡ�����⡱�ֶ�
			var = m_pRecordSet->GetCollect("PAP_ITEM");	
			if (var.vt != VT_NULL)
			{
				pTemp =  _com_util::ConvertBSTRToString(_bstr_t(var));
				len = strlen(pTemp);			//�ַ�������
				strcpy(pPaperItem, pTemp);		//�����ַ���
				strcpy(pPaperItem + len, "|");	//���Ʒָ���
				pPaperItem += len + 1;			//�ƶ�ָ��
				len = 0;
				delete pTemp;	
				pTemp = NULL;
			}
			
			//��ȡ��a�ֶ�
			var = m_pRecordSet->GetCollect("PAP_ANSWERA");
			if (var.vt != VT_NULL)
			{
				pTemp =  _com_util::ConvertBSTRToString(_bstr_t(var));
				len = strlen(pTemp);			//�ַ�������
				strcpy(pPaperItem, pTemp);		//�����ַ���
				strcpy(pPaperItem + len, "|");	//���Ʒָ���
				pPaperItem += len + 1;			//�ƶ�ָ��
				len = 0;
				delete pTemp;
				pTemp = NULL;
			}
			//��ȡ��b�ֶ�
			var = m_pRecordSet->GetCollect("PAP_ANSWERB");
			if (var.vt != VT_NULL)
			{
				pTemp =  _com_util::ConvertBSTRToString(_bstr_t(var));
				len = strlen(pTemp);			//�ַ�������
				strcpy(pPaperItem, pTemp);		//�����ַ���
				strcpy(pPaperItem + len, "|");	//���Ʒָ���
				pPaperItem += len + 1;			//�ƶ�ָ��
				len = 0;
				delete pTemp;
				pTemp = NULL;
			}	
			
			//��ȡ��c�ֶ�
			var = m_pRecordSet->GetCollect("PAP_ANSWERC");
			if (var.vt != VT_NULL)
			{
				pTemp =  _com_util::ConvertBSTRToString(_bstr_t(var));
				len = strlen(pTemp);			//�ַ�������
				strcpy(pPaperItem, pTemp);		//�����ַ���
				strcpy(pPaperItem + len, "|");	//���Ʒָ���
				pPaperItem += len + 1;			//�ƶ�ָ��
				len = 0;
				delete pTemp;
				pTemp = NULL;
			}			
			//��ȡ��d�ֶ�
			var = m_pRecordSet->GetCollect("PAP_ANSWERD");
			if (var.vt != VT_NULL)
			{
				pTemp =  _com_util::ConvertBSTRToString(_bstr_t(var));
				len = strlen(pTemp);			//�ַ�������
				strcpy(pPaperItem, pTemp);		//�����ַ���
				strcpy(pPaperItem + len, ">");	//������������ָ���
				pPaperItem += len + 1;			//�ƶ�ָ��
				len = 0;
				delete pTemp;
				pTemp = NULL;
			}
			//��ȡ��ȷ���ֶ�
			var = m_pRecordSet->GetCollect("PAP_ANSWER");
			if (var.vt != VT_NULL)
			{
				pTemp =  _com_util::ConvertBSTRToString(_bstr_t(var));
				len = strlen(pTemp);			//�ַ�������
				strcpy(pAnswerItem, pTemp);		//�����ַ���
				pAnswerItem++;					//���Ʒָ���
				delete pTemp;					//�ƶ�ָ��
				pTemp = NULL;
			}			
			m_pRecordSet->MoveNext();			//�ƶ���¼
			nIndex++;							//��¼��ż�1
		}		
		m_pRecordSet->Close();//�ر�
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
		return FALSE;
	}
	catch(...)
	{
		AfxMessageBox("�������ݿ������ʱ�����쳣��");
		return FALSE;
	}
	return TRUE;
}


/*
 * ���տͻ�����������
 */
DWORD WINAPI CServerView::AcceptThread( void *pParam )
{
	CServerView	*pServView = (CServerView*)pParam;	//������ָ��
	HANDLE		hComPort = pServView->m_hCompPort;	//��ɶ˿�
	SOCKET		sListen = pServView->m_sListen;		//�����׽���
	SOCKET		sAccept = INVALID_SOCKET;			//�����׽���
	while(pServView->m_bRunning)
	{
		DWORD dwRet;
		dwRet = WSAWaitForMultipleEvents(1,			//�ȴ������¼�
										&pServView->m_hEvent,
										FALSE,
										100,
										FALSE);		
		if(!pServView->m_bRunning)					//������ֹͣ����
			break;
		
		if (dwRet == WSA_WAIT_TIMEOUT)				//�������ó�ʱ
			continue;
		
		WSANETWORKEVENTS events;					//�鿴�����������¼�
		int nRet = WSAEnumNetworkEvents(pServView->m_sListen,
										pServView->m_hEvent,//�¼���������
										&events);		
		if (nRet == SOCKET_ERROR)
		{
			AfxMessageBox(_T("WSAEnumNetworkEvents��������"));
			break;
		}

		if ( events.lNetworkEvents & FD_ACCEPT)		//����FD_ACCEPT�����¼�
		{
			if ( events.iErrorCode[FD_ACCEPT_BIT] == 0 && pServView->m_bRunning)
			{
				//���ܿͻ�������
				SOCKADDR_IN servAddr;
				int	serAddrLen = sizeof(servAddr);	   
				if ((sAccept = WSAAccept(sListen, 
										(SOCKADDR*)&servAddr,
										&serAddrLen,
										NULL, 
										0)) == SOCKET_ERROR)
				{
					AfxMessageBox(_T("WSAAccept��������"));
					break;
				}
				//�����ͻ��˽ڵ�
				CClientContext *pClient = new CClientContext(sAccept,pServView);				
				
				if (CreateIoCompletionPort((HANDLE)sAccept,	//�׽�������ɶ˿ڹ�������
											hComPort,
											(DWORD) pClient,//��ɼ�
											0) == NULL)
				{
					return -1;
				}
				
				//�������ͻ�������
				CClientManager *pClientMgr = CClientManager::GetClientManager();
				pClientMgr->AddClient(pClient);
				
				if(!pClient->AsyncRecvHead())				//��������
				{
					pClientMgr->DeleteClient(pClient);
				}				
			}
		}		
	}

	//�ͷ���Դ
	CClientManager *pClientMgr = CClientManager::GetClientManager();
	pClientMgr->DeleteAllClient();
	pClientMgr->ReleaseManager();
	return 0;
}

/*
 * �����߳�
 */
DWORD WINAPI CServerView::ServiceThread( void *pParam )
{
	CServerView *pServerView = (CServerView*)pParam;//������ָ��
	HANDLE		hComPort = pServerView->m_hCompPort;//��ɶ˿�
	
	DWORD			dwIoSize;		//�����ֽ���
	CClientContext	*pClient;		//�ͻ���ָ��
	LPOVERLAPPED	lpOverlapped;	//�ص��ṹָ��
	bool			bExit = FALSE;	//�����߳��˳�
	while (!bExit)
	{
		dwIoSize = -1;
		lpOverlapped = NULL;
		pClient = NULL;
		//�ȴ�I/O�������
		BOOL bIORet = GetQueuedCompletionStatus(hComPort,
												&dwIoSize,
												(LPDWORD) &pClient,
												&lpOverlapped,
												INFINITE);
		//ʧ�ܵĲ������
		if (FALSE == bIORet && NULL != pClient)
		{	
			//�ͻ��˶Ͽ�
			if (CClientContext::DOING == pClient->m_eState 
				|| CClientContext::LOGIN == pClient->m_eState)
			{
				pClient->m_eState = CClientContext::DISCON;
				pClient->SaveDisConnectState();
			}
			CClientManager *pClientMgr = CClientManager::GetClientManager();
			pClientMgr->DeleteClient(pClient);				
		}
		//�ɹ��Ĳ������
		if(bIORet && lpOverlapped && pClient) 
		{				
			CClientManager *pClientMgr = CClientManager::GetClientManager();
			pClientMgr->ProcessIO(pClient, lpOverlapped, dwIoSize);				
		}	
		//�������˳�
		if(pClient == NULL&& lpOverlapped ==NULL && !pServerView->m_bRunning)
		{
			bExit = TRUE;
		}		
	}
	return 0;
}
/*
 * ��������
 */
void CServerView::OnServerStart() 
{
	int	reVal;						//����ֵ
	CServerAddrDlg  servAddrDlg;	//��������ַ�Ի���
	if (IDOK != servAddrDlg.DoModal())
	{
		return;
	}
	
	//��ʼ���׽��ֶ�̬��
	WSADATA wsaData;	
	if ((reVal = WSAStartup(0x0202, &wsaData)) != 0)
	{
		AfxMessageBox(_T("��ʼ���׽��ֶ�̬�����!"));
		return ;
	}
	
	//�����׽���
	if ((m_sListen = WSASocket(AF_INET,
								SOCK_STREAM,
								0,
								NULL, 
								0,
								WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		AfxMessageBox(_T("�����׽��ִ���!"));
		WSACleanup();
		return ;
	} 
	
	//���׽���
	SOCKADDR_IN	servAddr;//��������ַ
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(servAddrDlg.m_dwServIP);
	servAddr.sin_port = htons(servAddrDlg.m_shServPort);	
	if (bind(m_sListen, (SOCKADDR*)&servAddr, sizeof(servAddr)) 
											== SOCKET_ERROR)
	{
		AfxMessageBox(_T("���׽��ִ���!"));
		closesocket(m_sListen);
		WSACleanup();
		return ;
	}
	
	//����
	if(listen(m_sListen, SOMAXCONN) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("�����׽��ִ���!"));
		closesocket(m_sListen);
		WSACleanup();
		return ;
	}
	
	m_bRunning = TRUE;	//����������
	
	//�������ܿͻ��������¼�����
	m_hEvent = WSACreateEvent();
	if ( m_hEvent == WSA_INVALID_EVENT )
	{	
		closesocket(m_sListen);
		WSACleanup();
		return ;
	}
	
	//Ϊ�����׽���ע��FD_ACCEPT�����¼�
	int nRet = WSAEventSelect(m_sListen,
								m_hEvent,
								FD_ACCEPT);	
	if ( nRet == SOCKET_ERROR )
	{
		AfxMessageBox(_T("ע�������¼�����!"));
		closesocket(m_sListen);
		WSACleanup();
		return ;
	}
	
	// ������ɶ˿�
	if ((m_hCompPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		AfxMessageBox(_T("������ɶ˿�ʧ��!"));
		WSACloseEvent(m_hEvent);
		closesocket(m_sListen);
		WSACleanup();
		return ;
	}
	
	//�������ܿͻ��������߳�
	DWORD dwThreadID;
	m_hThread[0] = CreateThread(NULL, 
								0,
								AcceptThread,
								this, 
								0,  
								&dwThreadID);
	if (NULL == m_hThread[0])
	{
		AfxMessageBox(_T("�������ܿͻ����߳�ʧ��!"));
		WSACloseEvent(m_hEvent);
		closesocket(m_sListen);
		WSACleanup();
		return;
	}
	m_nThreadNum = 1;
	
	// ��ȡCPU����
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	
	// ���������߳�
	for(int i = 0; i < SystemInfo.dwNumberOfProcessors * 2; i++)
	{
		if ((m_hThread[m_nThreadNum++] = CreateThread(NULL, 
														0,
														ServiceThread,
														this,
														0, 
														&dwThreadID)) == NULL)
		{
			AfxMessageBox(_T("���������߳�ʧ��!"));
			WSACloseEvent(m_hEvent);
			closesocket(m_sListen);
			WSACleanup();
			return ;
		}
		
	}
	//���ö�ʱ����ÿ1���ӷ���WM_TIMER��Ϣ
	SetTimer(1, 1000 * 60 * 1,NULL);
}

void CServerView::OnUpdateServerStart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bRunning);	
}

/*
 * �������
 */
int CServerView::Calculate( char *pAnswer )
{
	ASSERT(pAnswer);
	int nResult = 0;
	char *pTemp = pAnswer;
	for (int i= 0; i < ANSWER_BUFFER_SIZE; i++,pTemp++)
	{
		while('-' == *pTemp)	//û��ѡ���"-1"
		{
			pTemp = pTemp +2;	//�ƶ�2���ֽ�
			i++;
		}		
		char cAnswer[1];
		if ('0' == *pTemp)		//0->A
		{
			strcpy(cAnswer,"A");
		}else if ('1' == *pTemp)//1->B
		{
			strcpy(cAnswer, "B");
		}else if ('2' == *pTemp)//2->C
		{
			strcpy(cAnswer, "C");
		}else if ('3' == *pTemp)//3->D
		{
			strcpy(cAnswer, "D");
		}else
		{}

		if (*cAnswer == m_cAnswer[i])	//ѡ����ȷ
		{
			nResult++;					//�Ʒ�
		}

	}
	return (2 * nResult);//100�ٷ���
}

/*
 * ���ٿͻ�������״̬
 */
void CServerView::OnTimer(UINT nIDEvent) 
{
	if (1 == nIDEvent)
	{
		//�����ͻ�������
		POSITION pos;
		CObList *pLientList = &CClientManager::GetClientManager()->m_clientList;
		for (pos = pLientList->GetHeadPosition(); pos != NULL;)
		{
			CClientContext *pClient = (CClientContext*)pLientList->GetNext(pos);
			//�������ڴ���״̬
			if ((CClientContext::DOING == pClient->m_eState) && TRUE == pClient->m_bPulse)
			{
				//�Ƚ�ʱ��
				CTime curTime = CTime::GetCurrentTime();
				int		nDay = curTime.GetDay() - pClient->m_time.GetDay();			//��
				int		nHour = curTime.GetHour() - pClient->m_time.GetHour();		//Сʱ
				int		nMinute= curTime.GetMinute() - pClient->m_time.GetMinute();	//����
				int		nSecond = curTime.GetSecond() - pClient->m_time.GetSecond();//��
				CTimeSpan spanTime(nDay, nHour, nMinute, nSecond);
				if ( spanTime.GetMinutes() > 1)//����1����
				{
					//���¿���״̬
					pClient->m_eState = CClientContext::DISCON;
					UpdateStudentState(pClient->m_nStdSN, CClientContext::DISCON);
					//ɾ���ÿͻ���
					CClientManager *pClientMgr = CClientManager::GetClientManager();
					pClientMgr->DeleteClient(pClient);	
				}
				
			}
		}
	}
	CListView::OnTimer(nIDEvent);
}

/*
 * ��֤ѧ��
 */
void CServerView::OnLookup( u_short usSN, CString &strName, BOOL &bFind)
{
	bFind = FALSE;//��֤�Ƿ�ɹ�
	//����vector
	vector<PSTUDENTINFO> ::iterator itor = m_vectStudent.begin();
	for (itor; itor != m_vectStudent.end();)
	{		
		PSTUDENTINFO pStdIn = *itor;
		int nSN = atoi(pStdIn->strSN);
		//ѧ����ͬ���ҿ���״̬Ϊ��N��
		if (usSN == nSN && !pStdIn->strState.Compare("N"))
		{
			strName = pStdIn->strName;//��������
			bFind = TRUE;
			break;
		}else
		{
			++itor;
		}
	}
}

/*
 * ���¿���״̬
 */
void CServerView::UpdateStudentState( u_short usID, u_short usState, u_short usGrade)
{
	CListCtrl &listCtl = GetListCtrl();	//�õ��б���ͼ�ؼ�

	CString strState;
	CString strGrade;
	switch(usState)
	{
	case CClientContext::LOGIN://��¼״̬
		{
			strState = _T("Login");
			break;
		}
	case CClientContext::DOING://���״̬
		{
			strState = _T("Doing");
			break;
		}
	case CClientContext::DONE://����״̬
		{
			strState = _T("Done");
			strGrade.Format("%d", usGrade);	//�����ɼ�		
			break;
		}
	case CClientContext::DISCON://����״̬
		{
			strState = _T("DisCon");
			break;
		}
	default:break;	
	}
	//����ѧ��
	PSTUDENTINFO pStdIn = NULL;
	int nIndex = 0;
	vector<PSTUDENTINFO> ::iterator itor = m_vectStudent.begin();
	for (itor; itor != m_vectStudent.end();)
	{
		pStdIn = *itor;
		int nSN = atoi(pStdIn->strSN);
		if (usID == nSN)
		{
			pStdIn->strState = strState;				//���¿���״̬
			listCtl.SetItemText(nIndex, 2, strState);	//���½���
			if (!pStdIn->strState.Compare(_T("Done")))	//��������
			{
				pStdIn->usGrade = usGrade;				//���¿����ɼ�
				listCtl.SetItemText(nIndex, 3, strGrade );
			}
			break;
		}else
		{
			itor++;
			nIndex++;
		}
	}
}

/*
 * ���濼���ɼ���״̬
 */
void CServerView::SaveStudentGrade( void )
{	
	CString strGrade;
	//����������Ϣ����
	vector<PSTUDENTINFO> ::iterator itor = m_vectStudent.begin();
	for (itor; itor != m_vectStudent.end();)
	{
		PSTUDENTINFO pStdIn = *itor;//������Ϣ
		//�ɼ�
		strGrade.Empty();
		strGrade.Format("%d", pStdIn->usGrade);
		
		try
		{
			//SQL���
			CString strSQL = "select * from Student where STD_SN = " ;
			strSQL += "'";
			strSQL += pStdIn->strSN;
			strSQL += "'";
			
			m_pRecordSet->Open(_variant_t(strSQL), 
								theApp.m_pConnection.GetInterfacePtr(),
								adOpenDynamic,
								adLockOptimistic,
								adCmdText);
			
			m_pRecordSet->PutCollect("STD_STATE", _variant_t(pStdIn->strState));//״̬
			m_pRecordSet->PutCollect("STD_GRADE", _variant_t(strGrade));		//�ɼ�
			m_pRecordSet->Update();												//��������
			m_pRecordSet->Close();
		}		
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
		catch(...)
		{
			AfxMessageBox("�������ݿ������ʱ�����쳣.");
			return;
		}
		
		delete pStdIn;				//�ͷ��ڴ���Դ
		pStdIn = NULL;
		m_vectStudent.erase(itor);	//ɾ��Ԫ��
	}
}

/*
 * �������˳�
 */
void CServerView::OnDestroy() 
{	
	KillTimer(1);//���ٶ�ʱ��
	if (TRUE == m_bRunning)
	{
		m_bRunning = FALSE;//����������״̬
		for (int i = 0; i < m_nThreadNum ; i++)
		{
			CloseHandle(m_hThread[i]);//�ر����߳̾��
			if (i < m_nThreadNum -1)
			{
				//������̷߳����˳�֪ͨ
				PostQueuedCompletionStatus(m_hCompPort, 0, (DWORD) NULL, NULL);	
			}
		}
		//�ȴ����߳��˳�
		::WaitForMultipleObjects(m_nThreadNum, m_hThread, TRUE, INFINITE);

		SaveStudentGrade();			//���濼��״̬�ͳɼ�

		closesocket(m_sListen);		//�رռ����׽���
		WSACloseEvent(m_hEvent);	//�ر��¼�
		WSACleanup();				//�ͷ��׽�����Դ
		CloseHandle(m_hCompPort);	//�ر���ɶ˿�
	}else
	{
		//�ͷſ�����Ϣ�ڴ�
		CString strGrade;
		vector<PSTUDENTINFO> ::iterator itor = m_vectStudent.begin();
		for (itor; itor != m_vectStudent.end();)
		{
			PSTUDENTINFO pStdIn = *itor;		
			delete pStdIn;
			pStdIn = NULL;
			m_vectStudent.erase(itor);			
		}
		//�ͷſͻ��˹����ڴ�
		CClientManager::ReleaseManager();
	}
	CListView::OnDestroy();		
}




