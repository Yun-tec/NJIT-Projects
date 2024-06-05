// ClientSocket.cpp: implementation of the CClientSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "client.h"
#include "ClientSocket.h"
#include "Item.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/*
 * 连接服务器
 */
BOOL CClientSocket::ConnectServer( DWORD &dwServIP, short &sServPort)
{
	int nRet;//返回值
	
	//服务器地址
	SOCKADDR_IN	servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = htonl(dwServIP);
	servAddr.sin_port = htons(sServPort);
	int nServLen = sizeof(servAddr);
	
	//连接服务器
	nRet = connect(m_s, (SOCKADDR*)&servAddr, nServLen);	
	if (SOCKET_ERROR == nRet)
	{
		return FALSE;
	}
	//使Nagle算法无效
	const char chOpt = 1;
	nRet = setsockopt(m_s, IPPROTO_TCP, TCP_NODELAY, &chOpt, sizeof(char));
	if (SOCKET_ERROR == nRet)
	{
		return FALSE;
	}
	return TRUE;
}


CClientSocket::CClientSocket(CClientDlg *pClientDlg)
{	
	int		nRet;//返回值
	WSADATA	wsaData;
	//初始化套接字动态库	
	if ((nRet = WSAStartup(0x0202, &wsaData)) != 0)
	{
		return ;
	}	
	//创建套接字
	if ((m_s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		AfxMessageBox("创建套接字错误。");
		WSACleanup();		
	} 
	m_pClientDlg = pClientDlg;
}

CClientSocket::~CClientSocket()
{
	closesocket(m_s);
	WSACleanup();
}


/*
 * 发送数据
 */
BOOL CClientSocket::SendPacket( PHDER pHdr, CClientDlg::state state, char* pSend)
{
	int nRet;													//返回值
	ZeroMemory(m_buf, BUFFER_SIZE);								//清除缓冲区
	memcpy(m_buf, (char*)pHdr, HEADELEN);						//拷贝包头
	
	u_short usState = (u_short)state;
	memcpy(m_buf + HEADELEN, (char*)&usState, sizeof(u_short));	//拷贝状态

	//拷贝数据
	if (NULL != pSend)
	{
		int nSendLen = strlen(pSend);
		memcpy(m_buf + HEADELEN + sizeof(u_short), pSend, nSendLen);//数据
	}
	
	//发送数据
	nRet = send(m_s, m_buf, HEADELEN + pHdr->usLen, 0);
	if (SOCKET_ERROR == nRet )
	{
		AfxMessageBox(_T("SendPacket数据失败。"));
		return FALSE;
	}	
	return TRUE;
}

/*
 * 发送心跳包
 */
BOOL CClientSocket::SendPulsePacket(void)
{
	int nRet;
	ZeroMemory(m_buf, BUFFER_SIZE);//清空发送数据缓冲区
	HDR hdr;
	hdr.ustype = PULSE;//包类型心跳包
	hdr.usLen = 0;
	nRet = send(m_s, (char*)&hdr, HEADELEN, 0);
	if (SOCKET_ERROR == nRet )
	{
		AfxMessageBox(_T("SendPulsePacket数据失败。"));
		return FALSE;
	}
	return TRUE;
}



/*
 * 接收试卷
 */
BOOL CClientSocket::RecvPaper(CString &strName, CObList *pItemList)
{
	//1.接收姓名
	int nRet;	//返回值
	
	//接收包头
	HDR hdr;	
	ZeroMemory(&hdr, HEADELEN);
	nRet = recv(m_s, (char*)&hdr, HEADELEN, 0);
	ASSERT(STNAME == hdr.ustype);
	if (0 == hdr.usLen)//考生登录失败
	{
		AfxMessageBox(_T("用户重复登录或者学号错误！"));
		return FALSE;
	}
	
	//接收姓名
	char recvBuf[BUFFER_SIZE];
	ZeroMemory(recvBuf, BUFFER_SIZE);
	nRet = recv(m_s, recvBuf, hdr.usLen, 0);
	strName = recvBuf;
	
	//2.接收试卷
	//接收包头
	ZeroMemory(&hdr, HEADELEN);
	nRet = recv(m_s, (char*)&hdr, HEADELEN, 0);
	if (SOCKET_ERROR == nRet)
	{
		return FALSE;
	}

	//接收试卷
	int nCount = 0;				//已经接收字节数
	int nTotal = hdr.usLen;		//试卷长度
	CItem	*pItem = NULL;
	while (nCount < nTotal)
	{
		int nRead = 0;			//接收字符数量
		char	c;				//接收字符
		nRead = recv(m_s, &c, 1, 0);
		if (SOCKET_ERROR == nRead)
		{
			AfxMessageBox(_T("接收试卷失败。"));
			break;
		}else
		{
			if (c == '<')		//开始符号
			{
				pItem = new CItem(m_pClientDlg);
			}else if (c == '>')	//结束符号
			{
				pItemList->AddTail(pItem);//一道试题				
			}else
			{
				pItem->m_strItem += c;
			}
			nCount++;
		}
	}
	return TRUE;
}