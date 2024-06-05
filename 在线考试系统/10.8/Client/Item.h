#if !defined(AFX_ITEM2_H__6D41B003_82B3_432D_85D6_5853FBFE4111__INCLUDED_)
#define AFX_ITEM2_H__6D41B003_82B3_432D_85D6_5853FBFE4111__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Item2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItem dialog

class CItem : public CDialog
{
// Construction
public:
	CItem(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CItem)
	enum { IDD = IDD_ITEM_DLG };
	//CString	m_question;
	//}}AFX_DATA

public:
	void	InitControl(void);	//��ʼ���ؼ�
public:
	CStatic		m_static;		//��ʾ���⾲̬�ؼ�
	CButton		m_bt[4];		//��ʾ�𰸵�ѡ��ť
	CString		m_strItem;		//����
	int			m_nIndex;		//�������
	CFont		m_font;			//���� 
	int			m_nSelected;	//��ѡ�����
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItem)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnSelected(UINT nCmdID);//ѡ���
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEM2_H__6D41B003_82B3_432D_85D6_5853FBFE4111__INCLUDED_)
