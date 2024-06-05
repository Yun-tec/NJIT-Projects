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
	BOOL	ConnectServer(DWORD &dwServIP, short &sServPort);				//���ӷ�����
	BOOL	SendPacket(PHDER pHdr, CClientDlg::state state, char* pSend);	//�������ݰ�
	BOOL	SendPulsePacket(void);											//����������
	BOOL	RecvPaper(CString &strName, CObList *pItemList);				//�����Ծ�

private:
	SOCKET	m_s;				//�׽���
	char	m_buf[BUFFER_SIZE];	//������
	CClientDlg *m_pClientDlg;	//������ָ��
private:
	CClientSocket(void);
};

#endif // !defined(AFX_CLIENTSOCKET_H__72D181B1_FD53_4DE0_A9AC_BF00EFF8F673__INCLUDED_)
