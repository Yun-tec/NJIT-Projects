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
	int		Calculate(char *pAnswer);												//计算分数
	void	OnLookup( u_short usSN, CString &strName, BOOL & bFind);				//验证考生
	void	UpdateStudentState(u_short usID, u_short usState, u_short usGrade = 0);	//更新考生状态
protected:
	void	InitListCtrlSetting(void);				//初始化列表视图控件
	void	InitListCtl(void);						//更新列表视图控件
	BOOL	ReadPaper(char *pPaper , char *pAnswer);//读取试卷
	void	SaveStudentGrade(void);					//保存学生成绩
	
	static	DWORD WINAPI AcceptThread(void *pParam);//接受客户端请求
	static	DWORD WINAPI ServiceThread(void *pParam);//服务线程

public:
	char			m_cPaper[PAPER_BUFFER_SIZE];	//试题缓冲区
	u_short			m_usPaperLen;					//试题缓冲区长度
	BOOL			m_bRunning;						//服务器运行状态
	
protected:
	HANDLE			m_hCompPort;					//完成端口句柄
	SOCKET			m_sListen;						//监听套接字
	_RecordsetPtr	m_pRecordSet;					//记录集对象
	char			m_cAnswer[ANSWER_BUFFER_SIZE];	//答案缓冲区
	vector<PSTUDENTINFO>	m_vectStudent;			//考生信息
	HANDLE			m_hThread[MAX_SUBTHREAD_SIZE];	//子线程=监听线程+服务线程
	int				m_nThreadNum;					//实际线程数量
	HANDLE			m_hEvent;						//监听事件句柄
};

#ifndef _DEBUG  // debug version in ServerView.cpp
inline CServerDoc* CServerView::GetDocument()
   { return (CServerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERVIEW_H__189ABD11_563A_4983_9E6D_3C8EE3761420__INCLUDED_)
