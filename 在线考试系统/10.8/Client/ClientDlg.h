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
		LOGIN,	//��¼״̬
		DOING,	//���״̬
		DONE,	//����
		DISCON,	//����״̬
		UNKNOWN
	};
// Dialog Data
	//{{AFX_DATA(CClientDlg)
	enum { IDD = IDD_CLIENT_DIALOG };
	CStatic	m_ctlSep;
	CStatic	m_ctlTime;	//ʱ��ؼ�
	CStatic	m_ctlSN;	//ѧ�ſؼ�
	CStatic	m_ctlName;	//�����ؼ�
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
	BOOL	ConnectServerAndGetPaper(void);	//���ӷ������������Ծ�
	void	InitPaper(void);				//��ʼ���Ծ�
	void	BeginExam(void);				//��ʼ���
	void	InitPaperTitle(void);			//��ʼ���Ծ�ͷ
	void	SendResult(void);				//���ʹ�����
protected:
	CClientSocket	*m_pClientSocket;	//ͨѶ��ָ��
	CObList			m_listItem;			//��������
	CRect			m_rect;				//�Ի���ͻ���
	state			m_state;			//�û�״̬
	CString			m_strName;			//����
	CLoginDlg		m_loginDlg;			//��¼����
	BOOL			m_bPulsePacket;		//����������
			

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
