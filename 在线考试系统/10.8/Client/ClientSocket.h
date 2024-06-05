// ClientSocket.h: interface for the CClientSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTSOCKET_H__72D181B1_FD53_4DE0_A9AC_BF00EFF8F673__INCLUDED_)
#define AFX_CLIENTSOCKET_H__72D181B1_FD53_4DE0_A9AC_BF00EFF8F673__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ClientDlg.h"
class CClientSocket : public CObject  
{
public:
	CClientSocket(CClientDlg *pClientDlg);
	virtual ~CClientSocket(void);

public:
	BOOL	ConnectServer(DWORD &dwServIP, short &sServPort);				//连接服务器
	BOOL	SendPacket(PHDER pHdr, CClientDlg::state state, char* pSend);	//发送数据包
	BOOL	SendPulsePacket(void);											//发送心跳包
	BOOL	RecvPaper(CString &strName, CObList *pItemList);				//接收试卷

private:
	SOCKET	m_s;				//套接字
	char	m_buf[BUFFER_SIZE];	//缓冲区
	CClientDlg *m_pClientDlg;	//主窗口指针
private:
	CClientSocket(void);
};

#endif // !defined(AFX_CLIENTSOCKET_H__72D181B1_FD53_4DE0_A9AC_BF00EFF8F673__INCLUDED_)
