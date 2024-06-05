// ClientContext.cpp: implementation of the CClientContext class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "ClientContext.h"
#include "ServerView.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern CServerApp theApp;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientContext::CClientContext(SOCKET s, CServerView* pServView)
:m_s(s),
m_eState(CClientContext::UNKNOWN),
m_pServerView(pServView),
m_nGrade(-1),
m_bPulse(FALSE)

{
	ZeroMemory(&m_iIO, OVERLAPPEDPLUSLEN);
	ZeroMemory(&m_oIO, OVERLAPPEDPLUSLEN);

}

CClientContext::~CClientContext()
{
	CancelIo((HANDLE)m_s);
	closesocket(m_s);
	m_pServerView = NULL;
}
/*
 * 发送试卷
 */
BOOL CClientContext::AsyncSendPaper( void )
{
	DWORD	flags = 0;			//标志
	DWORD	sendBytes =0;		//发送字节数
	

	ZeroMemory(&m_oIO, OVERLAPPEDPLUSLEN);
	m_oIO.type = IOWritePAPER;	//操作类型

	WSABUF wsaBuf[2];
	m_oIO.hdr.ustype = PAPER;						//试卷类型
	m_oIO.hdr.usLen = m_pServerView->m_usPaperLen;	//试卷长度

	wsaBuf[0].buf = (char*)&m_oIO.hdr;				//发送包头
	wsaBuf[0].len = HEADELEN;

	wsaBuf[1].buf = m_pServerView->m_cPaper;		//试卷
	wsaBuf[1].len = m_pServerView->m_usPaperLen;	//长度
	//发送数据
	if (WSASend(m_s, 
				wsaBuf,
				2, 
				&sendBytes, 
				flags,
				&m_oIO.overlapped,
				NULL) == SOCKET_ERROR)
	{
		if(ERROR_IO_PENDING != WSAGetLastError())//成功发起重叠操作
		{
			return FALSE;
		}
	}	
	return TRUE;
}


/*
 * 接收数据
 */
BOOL CClientContext::AsyncRecvHead( void )
{
	DWORD	flags = 0;		//标志
	DWORD	recvBytes =0;	//发送字节数

	ZeroMemory(&m_iIO, OVERLAPPEDPLUSLEN);
	m_iIO.type = IOReadHead;//操作类型

	WSABUF wsaBuf;
	wsaBuf.buf = (char*)&m_iIO.hdr;	//接收包头
	wsaBuf.len = HEADELEN;			//长度
	
	//读取数据
	if (WSARecv(m_s, 
				&wsaBuf,
				1, 
				&recvBytes, 
				&flags,
				&m_iIO.overlapped,
				NULL) == SOCKET_ERROR)
	{
		if(ERROR_IO_PENDING != WSAGetLastError())
		{
			return FALSE;
		}
	}	
	return TRUE;	
}

/*
 * 接收包头完毕处理
 */
void CClientContext::OnRecvHeadCompleted( DWORD dwIOSize )
{
	if (PULSE == m_iIO.hdr.ustype)			//处理心跳包
	{	
		m_bPulse = TRUE;					//接收到心跳包
		m_time = CTime::GetCurrentTime();	//时间
		AsyncRecvHead();					//继续接收包头
		
	}else if (STATE == m_iIO.hdr.ustype)	//接收包体
	{
		AsyncRecvBody(m_iIO.hdr.usLen);
	}
}



/*
 * 发送数据完毕处理
 */
void CClientContext::OnSendCompleted( DWORD dwIOSize )
{
	//
}
/*
 * 发送验证学号失败信息
 */
BOOL CClientContext::AsyncSendFailLoginMsg( void )
{
	DWORD	flags = 0;			//标志
	DWORD	sendBytes =0;		//发送字节数
	

	ZeroMemory(&m_oIO, OVERLAPPEDPLUSLEN);
	m_oIO.type = IOWriteUnLogin;		//操作类型

	WSABUF wsaBuf;
	m_oIO.hdr.ustype = STNAME;			//数据包类型
	m_oIO.hdr.usLen = 0;				//试卷长度0

	wsaBuf.buf = (char*)&m_oIO.hdr;		//发送包头
	wsaBuf.len = HEADELEN;

	//发送数据
	if (WSASend(m_s, 
				&wsaBuf,
				1, 
				&sendBytes, 
				flags,
				&m_oIO.overlapped,
				NULL) == SOCKET_ERROR)
	{
		if(ERROR_IO_PENDING != WSAGetLastError())//成功发起重叠操作
		{
			return FALSE;
		}
	}	
	return TRUE;
}



/*
 * 发送考生姓名
 */
BOOL CClientContext::AsyncSendStudentName( void )
{
	DWORD	flags = 0;			//标志
	DWORD	sendBytes =0;		//发送字节数
	
	ZeroMemory(&m_oIO, OVERLAPPEDPLUSLEN);
	m_oIO.type = IOWriteSTNAME;					//操作类型
	
	WSABUF wsaBuf[2];
	m_oIO.hdr.ustype = STNAME;					//包类型
	m_oIO.hdr.usLen = m_strName.GetLength();	//数据长度	
	//发送包头
	wsaBuf[0].buf = (char*)&m_oIO.hdr;				
	wsaBuf[0].len = HEADELEN;
	//包体
	wsaBuf[1].buf = (char*)(LPCSTR)m_strName;	//姓名		
	wsaBuf[1].len = m_strName.GetLength();		//长度
	
	//发送数据
	if (WSASend(m_s, 
		wsaBuf,
		2, 
		&sendBytes, 
		flags,
		&m_oIO.overlapped,
		NULL) == SOCKET_ERROR)
	{
		if(ERROR_IO_PENDING != WSAGetLastError())//成功发起重叠操作
		{
			return FALSE;
		}
	}	
	return TRUE;
}



/*
 * 退出
 */
BOOL CClientContext::AsyncRecvIOOK(void) 
{
	DWORD	flags = 0;		//标志
	DWORD	recvBytes =0;	//发送字节数

	ZeroMemory(&m_iIO, OVERLAPPEDPLUSLEN);
	m_iIO.type = IOEXIT;				//读取数据

	WSABUF wsaBuf;
	wsaBuf.buf = (char*)&m_iIO.hdr;	//接收包头
	wsaBuf.len = HEADELEN;

	//读取数据
	if (WSARecv(m_s, 
				&wsaBuf,
				1, 
				&recvBytes, 
				&flags,
				&m_iIO.overlapped,
				NULL) == SOCKET_ERROR)
	{
		if(ERROR_IO_PENDING != WSAGetLastError())
		{
			return FALSE;
		}
	}	
	return TRUE;	
}

/*
 * 接收包体
 */
BOOL CClientContext::AsyncRecvBody(int nLen )
{
	DWORD	flags = 0;			//标志
	DWORD	recvBytes =0;		//发送字节数
	
	ZeroMemory(&m_iIO, OVERLAPPEDPLUSLEN);
	m_iIO.type = IOReadBody;	//操作类型
	
	WSABUF wsaBuf;	
	wsaBuf.buf = m_iIO.recvBuf;	//接收包体
	wsaBuf.len = nLen;			//缓冲区长度

	//读取数据
	if (WSARecv(m_s, 
				&wsaBuf,
				1, 
				&recvBytes, 
				&flags,
				&m_iIO.overlapped,
				NULL) == SOCKET_ERROR)
	{
		if(ERROR_IO_PENDING != WSAGetLastError())
		{
			return FALSE;
		}
	}	
	return TRUE;	
}

/*
 * 接收包体完成
 */
void CClientContext::OnRecvBodyCompleted( DWORD dwIOSize )
{
	//获取考生状态
	u_short usStudentState;
	memcpy(&usStudentState, m_iIO.recvBuf, sizeof(u_short));//读取两个字节

	switch(usStudentState)
	{
	case LOGIN://登录状态
		{
			//获取学号
			char cSN[SNLEN];//学号
			memcpy(cSN, m_iIO.recvBuf + sizeof(u_short), 4);
			m_nStdSN = atoi(cSN);

			//考生验证
			BOOL bFind = TRUE;
			m_pServerView->OnLookup(m_nStdSN, m_strName, bFind);
			
			if( FALSE == bFind)//验证失败
			{
				AsyncSendFailLoginMsg();//发送验证失败消息
				AsyncRecvIOOK();		//断开客户端
			}else				//验证成功
			{
				AsyncSendStudentName();	//发送姓名
				
				//设置状态
				m_eState = LOGIN;
				m_pServerView->UpdateStudentState(m_nStdSN, LOGIN);

				AsyncRecvHead();//接收包头
			}			
			break;
		}
	case DOING://答题状态
		{
			//设置状态
			m_eState = DOING;
			m_pServerView->UpdateStudentState(m_nStdSN, DOING);
			AsyncRecvHead();			
			break;
		}
	case DONE://交卷状态
		{
			//计算成绩
			char cResult[BUFFER_SIZE];	
			strcpy(cResult, m_iIO.recvBuf + sizeof(u_short));
			m_nGrade = m_pServerView->Calculate(cResult);

			//设置状态
			m_eState = DONE;
			m_pServerView->UpdateStudentState(m_nStdSN, DONE, m_nGrade);
			AsyncRecvIOOK();//客户端退出			
			break;
		}
	default:break;
	}	
}

void CClientContext::SaveDisConnectState( void )
{
	m_pServerView->UpdateStudentState(m_nStdSN, DISCON);
}