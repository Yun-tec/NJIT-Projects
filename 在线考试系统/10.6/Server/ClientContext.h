// ClientContext.h: interface for the CClientContext class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTCONTEXT_H__DE6218FD_8A39_46C1_A9D2_EB6BC7082509__INCLUDED_)
#define AFX_CLIENTCONTEXT_H__DE6218FD_8A39_46C1_A9D2_EB6BC7082509__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ServerView.h"
#include <ctime>



class CClientContext :public CObject
{
public:
	CClientContext(SOCKET s, CServerView* pServView);
	virtual ~CClientContext();
	enum state
	{
		LOGIN,	//��¼״̬
		DOING,	//���״̬
		DONE,	//����
		DISCON,	//����״̬
		UNKNOWN	//δ֪״̬
	};

public:
	BOOL		AsyncSendPaper(void);					//�����Ծ�
	BOOL		AsyncSendStudentName(void);				//����ѧ������
	BOOL		AsyncSendFailLoginMsg(void);			//������֤ʧ��
	void		OnSendCompleted(DWORD dwIOSize);		//�����������	

	BOOL		AsyncRecvHead( void );					//���հ�ͷ
	BOOL		AsyncRecvBody(int nLen);				//���հ���
	void		OnRecvHeadCompleted( DWORD dwIOSize );	//���հ�ͷ���
	void		OnRecvBodyCompleted( DWORD dwIOSize );	//���հ������
	BOOL		AsyncRecvIOOK(void);					//�ͻ����˳�
	
	void		SaveDisConnectState(void);				//����Ͽ�״̬

public:
	IO_OPERATION_DATA	m_iIO;			//����չ�ص��ṹ���ݽṹ
	IO_OPERATION_DATA	m_oIO;			//д��չ�ص��ṹ���ݽṹ
	state				m_eState;		//����״̬
	CTime				m_time;			//����ʱ��
	BOOL				m_bPulse;		//������
	UINT				m_nStdSN;		//ѧ��
protected:
	SOCKET				m_s;			//�׽���
	CServerView			*m_pServerView;	//������ָ��
	CString				m_strName;		//��������
	int					m_nGrade;		//�ɼ�
};

#endif // !defined(AFX_CLIENTCONTEXT_H__DE6218FD_8A39_46C1_A9D2_EB6BC7082509__INCLUDED_)
