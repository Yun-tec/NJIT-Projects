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
 * �����Ծ�
 */
BOOL CClientContext::AsyncSendPaper( void )
{
	DWORD	flags = 0;			//��־
	DWORD	sendBytes =0;		//�����ֽ���
	

	ZeroMemory(&m_oIO, OVERLAPPEDPLUSLEN);
	m_oIO.type = IOWritePAPER;	//��������

	WSABUF wsaBuf[2];
	m_oIO.hdr.ustype = PAPER;						//�Ծ�����
	m_oIO.hdr.usLen = m_pServerView->m_usPaperLen;	//�Ծ���

	wsaBuf[0].buf = (char*)&m_oIO.hdr;				//���Ͱ�ͷ
	wsaBuf[0].len = HEADELEN;

	wsaBuf[1].buf = m_pServerView->m_cPaper;		//�Ծ�
	wsaBuf[1].len = m_pServerView->m_usPaperLen;	//����
	//��������
	if (WSASend(m_s, 
				wsaBuf,
				2, 
				&sendBytes, 
				flags,
				&m_oIO.overlapped,
				NULL) == SOCKET_ERROR)
	{
		if(ERROR_IO_PENDING != WSAGetLastError())//�ɹ������ص�����
		{
			return FALSE;
		}
	}	
	return TRUE;
}


/*
 * ��������
 */
BOOL CClientContext::AsyncRecvHead( void )
{
	DWORD	flags = 0;		//��־
	DWORD	recvBytes =0;	//�����ֽ���

	ZeroMemory(&m_iIO, OVERLAPPEDPLUSLEN);
	m_iIO.type = IOReadHead;//��������

	WSABUF wsaBuf;
	wsaBuf.buf = (char*)&m_iIO.hdr;	//���հ�ͷ
	wsaBuf.len = HEADELEN;			//����
	
	//��ȡ����
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
 * ���հ�ͷ��ϴ���
 */
void CClientContext::OnRecvHeadCompleted( DWORD dwIOSize )
{
	if (PULSE == m_iIO.hdr.ustype)			//����������
	{	
		m_bPulse = TRUE;					//���յ�������
		m_time = CTime::GetCurrentTime();	//ʱ��
		AsyncRecvHead();					//�������հ�ͷ
		
	}else if (STATE == m_iIO.hdr.ustype)	//���հ���
	{
		AsyncRecvBody(m_iIO.hdr.usLen);
	}
}



/*
 * ����������ϴ���
 */
void CClientContext::OnSendCompleted( DWORD dwIOSize )
{
	//
}
/*
 * ������֤ѧ��ʧ����Ϣ
 */
BOOL CClientContext::AsyncSendFailLoginMsg( void )
{
	DWORD	flags = 0;			//��־
	DWORD	sendBytes =0;		//�����ֽ���
	

	ZeroMemory(&m_oIO, OVERLAPPEDPLUSLEN);
	m_oIO.type = IOWriteUnLogin;		//��������

	WSABUF wsaBuf;
	m_oIO.hdr.ustype = STNAME;			//���ݰ�����
	m_oIO.hdr.usLen = 0;				//�Ծ���0

	wsaBuf.buf = (char*)&m_oIO.hdr;		//���Ͱ�ͷ
	wsaBuf.len = HEADELEN;

	//��������
	if (WSASend(m_s, 
				&wsaBuf,
				1, 
				&sendBytes, 
				flags,
				&m_oIO.overlapped,
				NULL) == SOCKET_ERROR)
	{
		if(ERROR_IO_PENDING != WSAGetLastError())//�ɹ������ص�����
		{
			return FALSE;
		}
	}	
	return TRUE;
}



/*
 * ���Ϳ�������
 */
BOOL CClientContext::AsyncSendStudentName( void )
{
	DWORD	flags = 0;			//��־
	DWORD	sendBytes =0;		//�����ֽ���
	
	ZeroMemory(&m_oIO, OVERLAPPEDPLUSLEN);
	m_oIO.type = IOWriteSTNAME;					//��������
	
	WSABUF wsaBuf[2];
	m_oIO.hdr.ustype = STNAME;					//������
	m_oIO.hdr.usLen = m_strName.GetLength();	//���ݳ���	
	//���Ͱ�ͷ
	wsaBuf[0].buf = (char*)&m_oIO.hdr;				
	wsaBuf[0].len = HEADELEN;
	//����
	wsaBuf[1].buf = (char*)(LPCSTR)m_strName;	//����		
	wsaBuf[1].len = m_strName.GetLength();		//����
	
	//��������
	if (WSASend(m_s, 
		wsaBuf,
		2, 
		&sendBytes, 
		flags,
		&m_oIO.overlapped,
		NULL) == SOCKET_ERROR)
	{
		if(ERROR_IO_PENDING != WSAGetLastError())//�ɹ������ص�����
		{
			return FALSE;
		}
	}	
	return TRUE;
}



/*
 * �˳�
 */
BOOL CClientContext::AsyncRecvIOOK(void) 
{
	DWORD	flags = 0;		//��־
	DWORD	recvBytes =0;	//�����ֽ���

	ZeroMemory(&m_iIO, OVERLAPPEDPLUSLEN);
	m_iIO.type = IOEXIT;				//��ȡ����

	WSABUF wsaBuf;
	wsaBuf.buf = (char*)&m_iIO.hdr;	//���հ�ͷ
	wsaBuf.len = HEADELEN;

	//��ȡ����
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
 * ���հ���
 */
BOOL CClientContext::AsyncRecvBody(int nLen )
{
	DWORD	flags = 0;			//��־
	DWORD	recvBytes =0;		//�����ֽ���
	
	ZeroMemory(&m_iIO, OVERLAPPEDPLUSLEN);
	m_iIO.type = IOReadBody;	//��������
	
	WSABUF wsaBuf;	
	wsaBuf.buf = m_iIO.recvBuf;	//���հ���
	wsaBuf.len = nLen;			//����������

	//��ȡ����
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
 * ���հ������
 */
void CClientContext::OnRecvBodyCompleted( DWORD dwIOSize )
{
	//��ȡ����״̬
	u_short usStudentState;
	memcpy(&usStudentState, m_iIO.recvBuf, sizeof(u_short));//��ȡ�����ֽ�

	switch(usStudentState)
	{
	case LOGIN://��¼״̬
		{
			//��ȡѧ��
			char cSN[SNLEN];//ѧ��
			memcpy(cSN, m_iIO.recvBuf + sizeof(u_short), 4);
			m_nStdSN = atoi(cSN);

			//������֤
			BOOL bFind = TRUE;
			m_pServerView->OnLookup(m_nStdSN, m_strName, bFind);
			
			if( FALSE == bFind)//��֤ʧ��
			{
				AsyncSendFailLoginMsg();//������֤ʧ����Ϣ
				AsyncRecvIOOK();		//�Ͽ��ͻ���
			}else				//��֤�ɹ�
			{
				AsyncSendStudentName();	//��������
				
				//����״̬
				m_eState = LOGIN;
				m_pServerView->UpdateStudentState(m_nStdSN, LOGIN);

				AsyncRecvHead();//���հ�ͷ
			}			
			break;
		}
	case DOING://����״̬
		{
			//����״̬
			m_eState = DOING;
			m_pServerView->UpdateStudentState(m_nStdSN, DOING);
			AsyncRecvHead();			
			break;
		}
	case DONE://����״̬
		{
			//����ɼ�
			char cResult[BUFFER_SIZE];	
			strcpy(cResult, m_iIO.recvBuf + sizeof(u_short));
			m_nGrade = m_pServerView->Calculate(cResult);

			//����״̬
			m_eState = DONE;
			m_pServerView->UpdateStudentState(m_nStdSN, DONE, m_nGrade);
			AsyncRecvIOOK();//�ͻ����˳�			
			break;
		}
	default:break;
	}	
}

void CClientContext::SaveDisConnectState( void )
{
	m_pServerView->UpdateStudentState(m_nStdSN, DISCON);
}