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
	void	InitControl(void);	//初始化控件
public:
	CStatic		m_static;		//显示问题静态控件
	CButton		m_bt[4];		//显示答案单选按钮
	CString		m_strItem;		//问题
	int			m_nIndex;		//试题序号
	CFont		m_font;			//字体 
	int			m_nSelected;	//被选择序号
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
	afx_msg void OnSelected(UINT nCmdID);//选择答案
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEM2_H__6D41B003_82B3_432D_85D6_5853FBFE4111__INCLUDED_)
