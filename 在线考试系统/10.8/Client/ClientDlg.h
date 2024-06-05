// ClientDlg.h : header file
//

#if !defined(AFX_CLIENTDLG_H__0D4DE135_DEC5_44EA_BE02_5EF219883B8A__INCLUDED_)
#define AFX_CLIENTDLG_H__0D4DE135_DEC5_44EA_BE02_5EF219883B8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

#include "LoginDlg.h"

class CItem;
class CClientSocket;

class CClientDlg : public CDialog
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor
	~CClientDlg(void);	
	enum state
	{
		LOGIN,	//登录状态
		DOING,	//答卷状态
		DONE,	//交卷
		DISCON,	//断线状态
		UNKNOWN
	};
// Dialog Data
	//{{AFX_DATA(CClientDlg)
	enum { IDD = IDD_CLIENT_DIALOG };
	CStatic	m_ctlSep;
	CStatic	m_ctlTime;	//时间控件
	CStatic	m_ctlSN;	//学号控件
	CStatic	m_ctlName;	//姓名控件
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
protected:
	BOOL	ConnectServerAndGetPaper(void);	//连接服务器并接收试卷
	void	InitPaper(void);				//初始化试卷
	void	BeginExam(void);				//开始答卷
	void	InitPaperTitle(void);			//初始化试卷头
	void	SendResult(void);				//发送答题结果
protected:
	CClientSocket	*m_pClientSocket;	//通讯类指针
	CObList			m_listItem;			//试题链表
	CRect			m_rect;				//对话框客户区
	state			m_state;			//用户状态
	CString			m_strName;			//姓名
	CLoginDlg		m_loginDlg;			//登录窗口
	BOOL			m_bPulsePacket;		//发送心跳包
			

// 	int m_nScrollPos;
// 	int m_nCurHeight;
	int m_offsetHeight;
	// Generated message map functions
	//{{AFX_MSG(CClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDLG_H__0D4DE135_DEC5_44EA_BE02_5EF219883B8A__INCLUDED_)
