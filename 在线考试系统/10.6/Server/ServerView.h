// ServerView.h : interface of the CServerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERVIEW_H__189ABD11_563A_4983_9E6D_3C8EE3761420__INCLUDED_)
#define AFX_SERVERVIEW_H__189ABD11_563A_4983_9E6D_3C8EE3761420__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "ServerDoc.h"


class  CServerDoc;

class CServerView : public CListView
{
protected: // create from serialization only
	CServerView();
	DECLARE_DYNCREATE(CServerView)
		
		// Attributes
public:
	CServerDoc* GetDocument();
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerView)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
protected:
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CServerView)
	afx_msg void OnServerStart();
	afx_msg void OnUpdateServerStart(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG	
	DECLARE_MESSAGE_MAP()
		

public:
	int		Calculate(char *pAnswer);												//�������
	void	OnLookup( u_short usSN, CString &strName, BOOL & bFind);				//��֤����
	void	UpdateStudentState(u_short usID, u_short usState, u_short usGrade = 0);	//���¿���״̬
protected:
	void	InitListCtrlSetting(void);				//��ʼ���б���ͼ�ؼ�
	void	InitListCtl(void);						//�����б���ͼ�ؼ�
	BOOL	ReadPaper(char *pPaper , char *pAnswer);//��ȡ�Ծ�
	void	SaveStudentGrade(void);					//����ѧ���ɼ�
	
	static	DWORD WINAPI AcceptThread(void *pParam);//���ܿͻ�������
	static	DWORD WINAPI ServiceThread(void *pParam);//�����߳�

public:
	char			m_cPaper[PAPER_BUFFER_SIZE];	//���⻺����
	u_short			m_usPaperLen;					//���⻺��������
	BOOL			m_bRunning;						//����������״̬
	
protected:
	HANDLE			m_hCompPort;					//��ɶ˿ھ��
	SOCKET			m_sListen;						//�����׽���
	_RecordsetPtr	m_pRecordSet;					//��¼������
	char			m_cAnswer[ANSWER_BUFFER_SIZE];	//�𰸻�����
	vector<PSTUDENTINFO>	m_vectStudent;			//������Ϣ
	HANDLE			m_hThread[MAX_SUBTHREAD_SIZE];	//���߳�=�����߳�+�����߳�
	int				m_nThreadNum;					//ʵ���߳�����
	HANDLE			m_hEvent;						//�����¼����
};

#ifndef _DEBUG  // debug version in ServerView.cpp
inline CServerDoc* CServerView::GetDocument()
   { return (CServerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERVIEW_H__189ABD11_563A_4983_9E6D_3C8EE3761420__INCLUDED_)
