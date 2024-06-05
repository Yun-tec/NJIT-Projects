// Item2.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "Item.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define		FONTHEIGHT	20 //字体高度
/////////////////////////////////////////////////////////////////////////////
// CItem dialog
CItem::CItem(CWnd* pParent /*=NULL*/)
	: CDialog(CItem::IDD, pParent)
,m_strItem(_T("")),
m_nIndex(0),
m_nSelected(-1)
{
}


void CItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItem)
// 	DDX_Text(pDX, IDC_STATIC_QUESTION, m_question);
// 	DDV_MaxChars(pDX, m_question, 100);
	//}}AFX_DATA_MAP
}


/*
 * 初始化控件
 */
void CItem::InitControl( void )
{	
	int nIndex = -1;
	nIndex = m_strItem.Find('|');//查找第一个‘|’
	if(-1 != nIndex)
	{
		//客户区域
		CRect rect;
		GetClientRect(&rect);
		int ctlWidth = (float)rect.Width()/4;//平均分成4份

		//1.显示问题
		//试题序号
		CString strIndex = m_strItem.Left(nIndex + 1);	
		m_nIndex = atoi(strIndex);
		
		CString strRight = m_strItem.Mid(nIndex+1);//剩余的字符串
		
		//问题字符串
		nIndex = strRight.Find('|');				
		CString strTemp;
		strTemp.Format("%d", m_nIndex);
		strTemp += ".  ";					//添加“ .”
		strTemp += strRight.Left(nIndex);	

		//控件区域
		CRect rectctl;
		rectctl.left = rect.left;
		rectctl.top = rect.top;
		rectctl.right = rect.right;
		rectctl.bottom = rect.bottom /2;
		//创建对象
		m_static.Create(strTemp, WS_CHILD|WS_VISIBLE, rectctl,this, 100);
		
		//2.显示答案A
		//答案A字符串;
		strTemp = strRight.Mid(nIndex + 1);
		nIndex = strTemp.Find('|');
		CString strLeft = strTemp.Left(nIndex);
		CString strAnswer("A: ");
		strAnswer += strLeft;
		
		//控件区域
		rectctl.left = rect.left;
		rectctl.top = rect.top + FONTHEIGHT;
		rectctl.right = rectctl.left + ctlWidth * 1;
		rectctl.bottom = rectctl.top + FONTHEIGHT;	
		//创建对象
		m_bt[0].Create(strAnswer, WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,rectctl, this, 1);
		
		//3.显示答案B
		//答案B字符串
		strRight = strTemp.Mid(nIndex +1);
		nIndex = strRight.Find('|');
		strLeft = strRight.Left(nIndex);		
		strAnswer = "B: ";
		strAnswer += strLeft;
		
		//控件区域
		rectctl.left = rect.left + ctlWidth * 1;
		rectctl.top = rect.top + FONTHEIGHT;
		rectctl.right = rectctl.left + ctlWidth;
		rectctl.bottom = rectctl.top + FONTHEIGHT;
		//创建对象
		m_bt[1].Create(strAnswer, WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,rectctl, this, 2);
		
		//4.显示答案C
		//答案C字符串
		strTemp = strRight.Mid(nIndex + 1);
		nIndex = strTemp.Find('|');
		strLeft = strTemp.Left(nIndex);		
		strAnswer = "C: ";
		strAnswer += strLeft;

		//控件区域
		rectctl.left = rect.left + ctlWidth * 2;
		rectctl.top = rect.top + FONTHEIGHT;
		rectctl.right = rectctl.left + ctlWidth;
		rectctl.bottom = rectctl.top + FONTHEIGHT;
		//创建对象
		m_bt[2].Create(strAnswer, WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,rectctl, this, 3);		
		

		//5.显示答案D
		//答案D字符串
		strRight = strTemp.Mid(nIndex +1);		
		strAnswer = "D: ";
		strAnswer += strRight;
		
		//控件区域
		rectctl.left = rect.left + ctlWidth * 3;
		rectctl.top = rect.top + FONTHEIGHT;
		rectctl.right = rectctl.left + ctlWidth ;
		rectctl.bottom = rectctl.top + FONTHEIGHT;	
		//创建对象
		m_bt[3].Create(strAnswer, WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,rectctl, this, 4);
		
		//创建字体
		m_font.CreateFont(12,
							0,         
							0,                         
							0,                         
							FW_NORMAL,                 
							FALSE,                     
							FALSE,                     
							0,                         
							ANSI_CHARSET,              
							OUT_DEFAULT_PRECIS,        
							CLIP_DEFAULT_PRECIS,       
							DEFAULT_QUALITY,           
							DEFAULT_PITCH | FF_SWISS, 
							"宋体"); 	
		//为每个控件设置字体
		CStatic *pStatic = (CStatic*)GetDlgItem(100);
		pStatic->SetFont(&m_font);
		CButton *pBt = (CButton*)GetDlgItem(1);
		pBt->SetFont(&m_font);
		pBt = (CButton*)GetDlgItem(2);
		pBt->SetFont(&m_font);
		pBt = (CButton*)GetDlgItem(3);
		pBt->SetFont(&m_font);
		pBt = (CButton*)GetDlgItem(4);
		pBt->SetFont(&m_font);		
	}
}


BEGIN_MESSAGE_MAP(CItem, CDialog)
	//{{AFX_MSG_MAP(CItem)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, 1, 4, OnSelected)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItem message handlers

BOOL CItem::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitControl();
	return TRUE;  
}


/*
 * 选择答案
 */
afx_msg void CItem::OnSelected( UINT nCmdID )
{
	switch(nCmdID)
	{
	case 1://控件1
		m_bt[0].SetCheck(1);//设置选中状态
		m_bt[1].SetCheck(0);//设置未选中状态
		m_bt[2].SetCheck(0);//设置未选中状态
		m_bt[3].SetCheck(0);//设置未选中状态
		m_nSelected = 0;	//当前被选择答案的序号
		break;
	case 2://控件2
		m_bt[0].SetCheck(0);
		m_bt[1].SetCheck(1);
		m_bt[2].SetCheck(0);
		m_bt[3].SetCheck(0);
		m_nSelected = 1;
		break;	
	case 3://控件3
		m_bt[0].SetCheck(0);
		m_bt[1].SetCheck(0);
		m_bt[2].SetCheck(1);
		m_bt[3].SetCheck(0);
		m_nSelected = 2;
		break;	
	case 4://控件4
		m_bt[0].SetCheck(0);
		m_bt[1].SetCheck(0);
		m_bt[2].SetCheck(0);
		m_bt[3].SetCheck(1);
		m_nSelected = 3;
		break;	
	default:break;		
	} 
}
