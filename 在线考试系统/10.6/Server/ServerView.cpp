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
m_sListen(INVALID_SOCKET),	//监听套接字
m_hCompPort(NULL),			//完成端口
m_bRunning(FALSE),			//服务器运行状态
m_nThreadNum(0),			//子线程数量
m_usPaperLen(0)				//试卷长度
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));	//创建记录集指针
	ZeroMemory(m_cPaper, PAPER_BUFFER_SIZE);			//试题缓冲区
	ZeroMemory(m_cAnswer, ANSWER_BUFFER_SIZE);			//答案缓冲区
	ZeroMemory(m_hThread, MAX_SUBTHREAD_SIZE);			//子线程句柄
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
 * 初始化
 */
void CServerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

 	InitListCtrlSetting();					//初始化列表视图控件
 
 	InitListCtl();							//更新列表视图控件

 	ReadPaper(m_cPaper, m_cAnswer);			//读取试卷
	m_usPaperLen = strlen(m_cPaper);		//试卷长度
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
 * 初始化列表视图控件
 */
void CServerView::InitListCtrlSetting( void )
{
	CListCtrl &listCtl = GetListCtrl();							//得到控件
	
	LONG lStyle = GetWindowLong(listCtl.m_hWnd, GWL_STYLE);		//获取风格
	lStyle |= LVS_REPORT;										//增加汇报风格
	SetWindowLong(listCtl.m_hWnd, GWL_STYLE, lStyle);			//设置style
	
	DWORD dwStyle = ListView_GetExtendedListViewStyle(listCtl);	//得到扩展风格
	dwStyle|= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;			//增加选择整行网格线风格
	ListView_SetExtendedListViewStyle (listCtl, dwStyle);
	
	CRect clientRect;
	listCtl.GetClientRect(&clientRect);//获取客户区域
	
	int			nCol;	//列
	LV_COLUMN	lvCol;	//结构
	lvCol.mask = LVCF_FMT |LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	//第一列
	nCol = 0;
	lvCol.iSubItem = nCol;
	lvCol.pszText = "学号";
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = (float)clientRect.Width()/100 * 25;
	listCtl.InsertColumn(nCol, &lvCol);
	
	//第二列
	nCol = 1;
	lvCol.iSubItem = nCol;
	lvCol.pszText = "姓名";
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = (float)clientRect.Width()/100 * 25;
	listCtl.InsertColumn(nCol, &lvCol);
	
	//第三列
	nCol = 2;
	lvCol.iSubItem = nCol;
	lvCol.pszText = "状态";
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = (float)clientRect.Width()/100 * 25;
	listCtl.InsertColumn(nCol, &lvCol);
	
	//第四列
	nCol = 3;
	lvCol.iSubItem = nCol;
	lvCol.pszText = "成绩";
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = (float)clientRect.Width()/100 * 25;
	listCtl.InsertColumn(nCol, &lvCol);
}


/*
 * 更新列表视图控件
 */
void CServerView::InitListCtl( void )
{
	CListCtrl &listCtl = GetListCtrl();				//得到列表视图控件

	try 
	{
		int nIndex=0;								//记录序号		
		CString strSQL = "select * from Student";	//读取所有记录
		m_pRecordSet->Open(_variant_t(strSQL),
							theApp.m_pConnection.GetInterfacePtr(), 
							adOpenDynamic,
							adLockOptimistic,
							adCmdText);
		
		while (!m_pRecordSet->adoEOF)//记录尾
		{
			_variant_t var;			//临时变量
			CString strSN;			//学号
			CString strName;		//姓名
			CString strState;		//状态
			CString	strGrade;		//成绩
			
			//读取“学号”字段
			var = m_pRecordSet->GetCollect("STD_SN");			
			if (var.vt != VT_NULL)
			{
				strSN = (LPCSTR)_bstr_t(var);
			}
			//读取“姓名”字段
			var = m_pRecordSet->GetCollect("STD_NAME");	
			if (var.vt != VT_NULL)
			{
				strName = (LPCSTR)_bstr_t(var);
 			}

			//读取“状态”字段
			var = m_pRecordSet->GetCollect("STD_STATE");
			if (var.vt != VT_NULL)
			{
				strState = (LPCSTR)_bstr_t(var);
			}
			//读取“成绩”字段
			var = m_pRecordSet->GetCollect("STD_GRADE");
			if (var.vt != VT_NULL)
			{
				strGrade = (LPCSTR)_bstr_t(var);
			}
			//插入数据项			
			listCtl.InsertItem(nIndex, strSN);			//学号
			listCtl.SetItemText(nIndex, 1, strName);	//姓名
			listCtl.SetItemText(nIndex, 2, strState);	//状态
			listCtl.SetItemText(nIndex, 3, strGrade);	//成绩

			//定义考生信息
			PSTUDENTINFO  pStdIn = new STUDENTINFO;
			pStdIn->strSN = strSN;						//学号
			pStdIn->strName = strName;					//姓名
			pStdIn->strState = strState;				//状态
			pStdIn->usGrade = atoi(strGrade);			//成绩
			m_vectStudent.push_back(pStdIn);			//加入vector中

			m_pRecordSet->MoveNext();					//移动记录
			nIndex++;									//记录序号加1
		}		
		m_pRecordSet->Close();//关闭		
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	catch(...)
	{
		AfxMessageBox("访问数据库服务器时发生异常。");
		return;
	}
}

/*
 * 读取试卷
 */
BOOL CServerView::ReadPaper( char *pPaper , char *pAnswer)
{
	ASSERT(pAnswer && pAnswer);
	
	char	*pPaperItem = pPaper;	//试题缓冲区指针
	char	*pAnswerItem = pAnswer;	//答案缓冲区指针
	try 
	{
		int nIndex=0;							//记录序号		
		CString strSQL = "select * from Paper";	//读取所有记录
		m_pRecordSet->Open(_variant_t(strSQL),
							theApp.m_pConnection.GetInterfacePtr(), 
							adOpenDynamic,
							adLockOptimistic,
							adCmdText);
		
		while (!m_pRecordSet->adoEOF)	//记录尾
		{
			_variant_t var;				//临时变量
			char *pTemp = NULL;
			int	len = 0;		;
			strcpy(pPaperItem, "<");	//试题开始分隔符
			pPaperItem++;
			
			//读取“题号”字段
			var = m_pRecordSet->GetCollect("PAP_ID");			
			if (var.vt != VT_NULL)
			{
				pTemp =  _com_util::ConvertBSTRToString(_bstr_t(var));
				len = strlen(pTemp);			//字符串长度
				strcpy(pPaperItem, pTemp);		//复制字符串
				strcpy(pPaperItem + len, "|");	//复制分隔符
				pPaperItem += len + 1;			//移动指针
				len = 0;
				delete pTemp;
				pTemp = NULL;
			}
			pTemp = NULL;
			//读取“问题”字段
			var = m_pRecordSet->GetCollect("PAP_ITEM");	
			if (var.vt != VT_NULL)
			{
				pTemp =  _com_util::ConvertBSTRToString(_bstr_t(var));
				len = strlen(pTemp);			//字符串长度
				strcpy(pPaperItem, pTemp);		//复制字符串
				strcpy(pPaperItem + len, "|");	//复制分隔符
				pPaperItem += len + 1;			//移动指针
				len = 0;
				delete pTemp;	
				pTemp = NULL;
			}
			
			//读取答案a字段
			var = m_pRecordSet->GetCollect("PAP_ANSWERA");
			if (var.vt != VT_NULL)
			{
				pTemp =  _com_util::ConvertBSTRToString(_bstr_t(var));
				len = strlen(pTemp);			//字符串长度
				strcpy(pPaperItem, pTemp);		//复制字符串
				strcpy(pPaperItem + len, "|");	//复制分隔符
				pPaperItem += len + 1;			//移动指针
				len = 0;
				delete pTemp;
				pTemp = NULL;
			}
			//读取答案b字段
			var = m_pRecordSet->GetCollect("PAP_ANSWERB");
			if (var.vt != VT_NULL)
			{
				pTemp =  _com_util::ConvertBSTRToString(_bstr_t(var));
				len = strlen(pTemp);			//字符串长度
				strcpy(pPaperItem, pTemp);		//复制字符串
				strcpy(pPaperItem + len, "|");	//复制分隔符
				pPaperItem += len + 1;			//移动指针
				len = 0;
				delete pTemp;
				pTemp = NULL;
			}	
			
			//读取答案c字段
			var = m_pRecordSet->GetCollect("PAP_ANSWERC");
			if (var.vt != VT_NULL)
			{
				pTemp =  _com_util::ConvertBSTRToString(_bstr_t(var));
				len = strlen(pTemp);			//字符串长度
				strcpy(pPaperItem, pTemp);		//复制字符串
				strcpy(pPaperItem + len, "|");	//复制分隔符
				pPaperItem += len + 1;			//移动指针
				len = 0;
				delete pTemp;
				pTemp = NULL;
			}			
			//读取答案d字段
			var = m_pRecordSet->GetCollect("PAP_ANSWERD");
			if (var.vt != VT_NULL)
			{
				pTemp =  _com_util::ConvertBSTRToString(_bstr_t(var));
				len = strlen(pTemp);			//字符串长度
				strcpy(pPaperItem, pTemp);		//复制字符串
				strcpy(pPaperItem + len, ">");	//复制试题结束分隔符
				pPaperItem += len + 1;			//移动指针
				len = 0;
				delete pTemp;
				pTemp = NULL;
			}
			//读取正确答案字段
			var = m_pRecordSet->GetCollect("PAP_ANSWER");
			if (var.vt != VT_NULL)
			{
				pTemp =  _com_util::ConvertBSTRToString(_bstr_t(var));
				len = strlen(pTemp);			//字符串长度
				strcpy(pAnswerItem, pTemp);		//复制字符串
				pAnswerItem++;					//复制分隔符
				delete pTemp;					//移动指针
				pTemp = NULL;
			}			
			m_pRecordSet->MoveNext();			//移动记录
			nIndex++;							//记录序号加1
		}		
		m_pRecordSet->Close();//关闭
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
		return FALSE;
	}
	catch(...)
	{
		AfxMessageBox("访问数据库服务器时发生异常。");
		return FALSE;
	}
	return TRUE;
}


/*
 * 接收客户端连接请求
 */
DWORD WINAPI CServerView::AcceptThread( void *pParam )
{
	CServerView	*pServView = (CServerView*)pParam;	//主窗口指针
	HANDLE		hComPort = pServView->m_hCompPort;	//完成端口
	SOCKET		sListen = pServView->m_sListen;		//监听套接字
	SOCKET		sAccept = INVALID_SOCKET;			//接受套接字
	while(pServView->m_bRunning)
	{
		DWORD dwRet;
		dwRet = WSAWaitForMultipleEvents(1,			//等待网络事件
										&pServView->m_hEvent,
										FALSE,
										100,
										FALSE);		
		if(!pServView->m_bRunning)					//服务器停止服务
			break;
		
		if (dwRet == WSA_WAIT_TIMEOUT)				//函数调用超时
			continue;
		
		WSANETWORKEVENTS events;					//查看发生的网络事件
		int nRet = WSAEnumNetworkEvents(pServView->m_sListen,
										pServView->m_hEvent,//事件对象被重置
										&events);		
		if (nRet == SOCKET_ERROR)
		{
			AfxMessageBox(_T("WSAEnumNetworkEvents函数错误"));
			break;
		}

		if ( events.lNetworkEvents & FD_ACCEPT)		//发生FD_ACCEPT网络事件
		{
			if ( events.iErrorCode[FD_ACCEPT_BIT] == 0 && pServView->m_bRunning)
			{
				//接受客户端请求
				SOCKADDR_IN servAddr;
				int	serAddrLen = sizeof(servAddr);	   
				if ((sAccept = WSAAccept(sListen, 
										(SOCKADDR*)&servAddr,
										&serAddrLen,
										NULL, 
										0)) == SOCKET_ERROR)
				{
					AfxMessageBox(_T("WSAAccept函数错误"));
					break;
				}
				//创建客户端节点
				CClientContext *pClient = new CClientContext(sAccept,pServView);				
				
				if (CreateIoCompletionPort((HANDLE)sAccept,	//套接字与完成端口关联起来
											hComPort,
											(DWORD) pClient,//完成键
											0) == NULL)
				{
					return -1;
				}
				
				//加入管理客户端链表
				CClientManager *pClientMgr = CClientManager::GetClientManager();
				pClientMgr->AddClient(pClient);
				
				if(!pClient->AsyncRecvHead())				//接收数据
				{
					pClientMgr->DeleteClient(pClient);
				}				
			}
		}		
	}

	//释放资源
	CClientManager *pClientMgr = CClientManager::GetClientManager();
	pClientMgr->DeleteAllClient();
	pClientMgr->ReleaseManager();
	return 0;
}

/*
 * 服务线程
 */
DWORD WINAPI CServerView::ServiceThread( void *pParam )
{
	CServerView *pServerView = (CServerView*)pParam;//主窗口指针
	HANDLE		hComPort = pServerView->m_hCompPort;//完成端口
	
	DWORD			dwIoSize;		//传输字节数
	CClientContext	*pClient;		//客户端指针
	LPOVERLAPPED	lpOverlapped;	//重叠结构指针
	bool			bExit = FALSE;	//服务线程退出
	while (!bExit)
	{
		dwIoSize = -1;
		lpOverlapped = NULL;
		pClient = NULL;
		//等待I/O操作结果
		BOOL bIORet = GetQueuedCompletionStatus(hComPort,
												&dwIoSize,
												(LPDWORD) &pClient,
												&lpOverlapped,
												INFINITE);
		//失败的操作完成
		if (FALSE == bIORet && NULL != pClient)
		{	
			//客户端断开
			if (CClientContext::DOING == pClient->m_eState 
				|| CClientContext::LOGIN == pClient->m_eState)
			{
				pClient->m_eState = CClientContext::DISCON;
				pClient->SaveDisConnectState();
			}
			CClientManager *pClientMgr = CClientManager::GetClientManager();
			pClientMgr->DeleteClient(pClient);				
		}
		//成功的操作完成
		if(bIORet && lpOverlapped && pClient) 
		{				
			CClientManager *pClientMgr = CClientManager::GetClientManager();
			pClientMgr->ProcessIO(pClient, lpOverlapped, dwIoSize);				
		}	
		//服务器退出
		if(pClient == NULL&& lpOverlapped ==NULL && !pServerView->m_bRunning)
		{
			bExit = TRUE;
		}		
	}
	return 0;
}
/*
 * 启动服务
 */
void CServerView::OnServerStart() 
{
	int	reVal;						//返回值
	CServerAddrDlg  servAddrDlg;	//服务器地址对话框
	if (IDOK != servAddrDlg.DoModal())
	{
		return;
	}
	
	//初始化套接字动态库
	WSADATA wsaData;	
	if ((reVal = WSAStartup(0x0202, &wsaData)) != 0)
	{
		AfxMessageBox(_T("初始化套接字动态库错误!"));
		return ;
	}
	
	//创建套接字
	if ((m_sListen = WSASocket(AF_INET,
								SOCK_STREAM,
								0,
								NULL, 
								0,
								WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		AfxMessageBox(_T("创建套接字错误!"));
		WSACleanup();
		return ;
	} 
	
	//绑定套接字
	SOCKADDR_IN	servAddr;//服务器地址
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(servAddrDlg.m_dwServIP);
	servAddr.sin_port = htons(servAddrDlg.m_shServPort);	
	if (bind(m_sListen, (SOCKADDR*)&servAddr, sizeof(servAddr)) 
											== SOCKET_ERROR)
	{
		AfxMessageBox(_T("绑定套接字错误!"));
		closesocket(m_sListen);
		WSACleanup();
		return ;
	}
	
	//监听
	if(listen(m_sListen, SOMAXCONN) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("监听套接字错误!"));
		closesocket(m_sListen);
		WSACleanup();
		return ;
	}
	
	m_bRunning = TRUE;	//服务器运行
	
	//创建接受客户端连接事件对象
	m_hEvent = WSACreateEvent();
	if ( m_hEvent == WSA_INVALID_EVENT )
	{	
		closesocket(m_sListen);
		WSACleanup();
		return ;
	}
	
	//为监听套接字注册FD_ACCEPT网络事件
	int nRet = WSAEventSelect(m_sListen,
								m_hEvent,
								FD_ACCEPT);	
	if ( nRet == SOCKET_ERROR )
	{
		AfxMessageBox(_T("注册网络事件错误!"));
		closesocket(m_sListen);
		WSACleanup();
		return ;
	}
	
	// 创建完成端口
	if ((m_hCompPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		AfxMessageBox(_T("创建完成端口失败!"));
		WSACloseEvent(m_hEvent);
		closesocket(m_sListen);
		WSACleanup();
		return ;
	}
	
	//创建接受客户端请求线程
	DWORD dwThreadID;
	m_hThread[0] = CreateThread(NULL, 
								0,
								AcceptThread,
								this, 
								0,  
								&dwThreadID);
	if (NULL == m_hThread[0])
	{
		AfxMessageBox(_T("创建接受客户端线程失败!"));
		WSACloseEvent(m_hEvent);
		closesocket(m_sListen);
		WSACleanup();
		return;
	}
	m_nThreadNum = 1;
	
	// 获取CPU数量
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	
	// 创建服务线程
	for(int i = 0; i < SystemInfo.dwNumberOfProcessors * 2; i++)
	{
		if ((m_hThread[m_nThreadNum++] = CreateThread(NULL, 
														0,
														ServiceThread,
														this,
														0, 
														&dwThreadID)) == NULL)
		{
			AfxMessageBox(_T("创建服务线程失败!"));
			WSACloseEvent(m_hEvent);
			closesocket(m_sListen);
			WSACleanup();
			return ;
		}
		
	}
	//设置定时器，每1分钟发送WM_TIMER消息
	SetTimer(1, 1000 * 60 * 1,NULL);
}

void CServerView::OnUpdateServerStart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bRunning);	
}

/*
 * 计算分数
 */
int CServerView::Calculate( char *pAnswer )
{
	ASSERT(pAnswer);
	int nResult = 0;
	char *pTemp = pAnswer;
	for (int i= 0; i < ANSWER_BUFFER_SIZE; i++,pTemp++)
	{
		while('-' == *pTemp)	//没有选择答案"-1"
		{
			pTemp = pTemp +2;	//移动2个字节
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

		if (*cAnswer == m_cAnswer[i])	//选择正确
		{
			nResult++;					//计分
		}

	}
	return (2 * nResult);//100百分制
}

/*
 * 跟踪客户端连接状态
 */
void CServerView::OnTimer(UINT nIDEvent) 
{
	if (1 == nIDEvent)
	{
		//遍历客户端链表
		POSITION pos;
		CObList *pLientList = &CClientManager::GetClientManager()->m_clientList;
		for (pos = pLientList->GetHeadPosition(); pos != NULL;)
		{
			CClientContext *pClient = (CClientContext*)pLientList->GetNext(pos);
			//考生处于答题状态
			if ((CClientContext::DOING == pClient->m_eState) && TRUE == pClient->m_bPulse)
			{
				//比较时间
				CTime curTime = CTime::GetCurrentTime();
				int		nDay = curTime.GetDay() - pClient->m_time.GetDay();			//日
				int		nHour = curTime.GetHour() - pClient->m_time.GetHour();		//小时
				int		nMinute= curTime.GetMinute() - pClient->m_time.GetMinute();	//分钟
				int		nSecond = curTime.GetSecond() - pClient->m_time.GetSecond();//秒
				CTimeSpan spanTime(nDay, nHour, nMinute, nSecond);
				if ( spanTime.GetMinutes() > 1)//大于1分钟
				{
					//更新考生状态
					pClient->m_eState = CClientContext::DISCON;
					UpdateStudentState(pClient->m_nStdSN, CClientContext::DISCON);
					//删除该客户端
					CClientManager *pClientMgr = CClientManager::GetClientManager();
					pClientMgr->DeleteClient(pClient);	
				}
				
			}
		}
	}
	CListView::OnTimer(nIDEvent);
}

/*
 * 验证学号
 */
void CServerView::OnLookup( u_short usSN, CString &strName, BOOL &bFind)
{
	bFind = FALSE;//验证是否成功
	//遍历vector
	vector<PSTUDENTINFO> ::iterator itor = m_vectStudent.begin();
	for (itor; itor != m_vectStudent.end();)
	{		
		PSTUDENTINFO pStdIn = *itor;
		int nSN = atoi(pStdIn->strSN);
		//学号相同并且考生状态为“N”
		if (usSN == nSN && !pStdIn->strState.Compare("N"))
		{
			strName = pStdIn->strName;//考生姓名
			bFind = TRUE;
			break;
		}else
		{
			++itor;
		}
	}
}

/*
 * 更新考生状态
 */
void CServerView::UpdateStudentState( u_short usID, u_short usState, u_short usGrade)
{
	CListCtrl &listCtl = GetListCtrl();	//得到列表视图控件

	CString strState;
	CString strGrade;
	switch(usState)
	{
	case CClientContext::LOGIN://登录状态
		{
			strState = _T("Login");
			break;
		}
	case CClientContext::DOING://答卷状态
		{
			strState = _T("Doing");
			break;
		}
	case CClientContext::DONE://交卷状态
		{
			strState = _T("Done");
			strGrade.Format("%d", usGrade);	//考生成绩		
			break;
		}
	case CClientContext::DISCON://断线状态
		{
			strState = _T("DisCon");
			break;
		}
	default:break;	
	}
	//查找学生
	PSTUDENTINFO pStdIn = NULL;
	int nIndex = 0;
	vector<PSTUDENTINFO> ::iterator itor = m_vectStudent.begin();
	for (itor; itor != m_vectStudent.end();)
	{
		pStdIn = *itor;
		int nSN = atoi(pStdIn->strSN);
		if (usID == nSN)
		{
			pStdIn->strState = strState;				//更新考生状态
			listCtl.SetItemText(nIndex, 2, strState);	//更新界面
			if (!pStdIn->strState.Compare(_T("Done")))	//考生交卷
			{
				pStdIn->usGrade = usGrade;				//更新考生成绩
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
 * 保存考生成绩和状态
 */
void CServerView::SaveStudentGrade( void )
{	
	CString strGrade;
	//遍历考生信息集合
	vector<PSTUDENTINFO> ::iterator itor = m_vectStudent.begin();
	for (itor; itor != m_vectStudent.end();)
	{
		PSTUDENTINFO pStdIn = *itor;//考生信息
		//成绩
		strGrade.Empty();
		strGrade.Format("%d", pStdIn->usGrade);
		
		try
		{
			//SQL语句
			CString strSQL = "select * from Student where STD_SN = " ;
			strSQL += "'";
			strSQL += pStdIn->strSN;
			strSQL += "'";
			
			m_pRecordSet->Open(_variant_t(strSQL), 
								theApp.m_pConnection.GetInterfacePtr(),
								adOpenDynamic,
								adLockOptimistic,
								adCmdText);
			
			m_pRecordSet->PutCollect("STD_STATE", _variant_t(pStdIn->strState));//状态
			m_pRecordSet->PutCollect("STD_GRADE", _variant_t(strGrade));		//成绩
			m_pRecordSet->Update();												//更新数据
			m_pRecordSet->Close();
		}		
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
		catch(...)
		{
			AfxMessageBox("访问数据库服务器时发生异常.");
			return;
		}
		
		delete pStdIn;				//释放内存资源
		pStdIn = NULL;
		m_vectStudent.erase(itor);	//删除元素
	}
}

/*
 * 服务器退出
 */
void CServerView::OnDestroy() 
{	
	KillTimer(1);//销毁定时器
	if (TRUE == m_bRunning)
	{
		m_bRunning = FALSE;//服务器运行状态
		for (int i = 0; i < m_nThreadNum ; i++)
		{
			CloseHandle(m_hThread[i]);//关闭子线程句柄
			if (i < m_nThreadNum -1)
			{
				//向服务线程发送退出通知
				PostQueuedCompletionStatus(m_hCompPort, 0, (DWORD) NULL, NULL);	
			}
		}
		//等待子线程退出
		::WaitForMultipleObjects(m_nThreadNum, m_hThread, TRUE, INFINITE);

		SaveStudentGrade();			//保存考生状态和成绩

		closesocket(m_sListen);		//关闭监听套接字
		WSACloseEvent(m_hEvent);	//关闭事件
		WSACleanup();				//释放套接字资源
		CloseHandle(m_hCompPort);	//关闭完成端口
	}else
	{
		//释放考生信息内存
		CString strGrade;
		vector<PSTUDENTINFO> ::iterator itor = m_vectStudent.begin();
		for (itor; itor != m_vectStudent.end();)
		{
			PSTUDENTINFO pStdIn = *itor;		
			delete pStdIn;
			pStdIn = NULL;
			m_vectStudent.erase(itor);			
		}
		//释放客户端管理内存
		CClientManager::ReleaseManager();
	}
	CListView::OnDestroy();		
}




