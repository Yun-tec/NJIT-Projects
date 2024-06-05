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
#define		FONTHEIGHT	20 //����߶�
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
 * ��ʼ���ؼ�
 */
void CItem::InitControl( void )
{	
	int nIndex = -1;
	nIndex = m_strItem.Find('|');//���ҵ�һ����|��
	if(-1 != nIndex)
	{
		//�ͻ�����
		CRect rect;
		GetClientRect(&rect);
		int ctlWidth = (float)rect.Width()/4;//ƽ���ֳ�4��

		//1.��ʾ����
		//�������
		CString strIndex = m_strItem.Left(nIndex + 1);	
		m_nIndex = atoi(strIndex);
		
		CString strRight = m_strItem.Mid(nIndex+1);//ʣ����ַ���
		
		//�����ַ���
		nIndex = strRight.Find('|');				
		CString strTemp;
		strTemp.Format("%d", m_nIndex);
		strTemp += ".  ";					//��ӡ� .��
		strTemp += strRight.Left(nIndex);	

		//�ؼ�����
		CRect rectctl;
		rectctl.left = rect.left;
		rectctl.top = rect.top;
		rectctl.right = rect.right;
		rectctl.bottom = rect.bottom /2;
		//��������
		m_static.Create(strTemp, WS_CHILD|WS_VISIBLE, rectctl,this, 100);
		
		//2.��ʾ��A
		//��A�ַ���;
		strTemp = strRight.Mid(nIndex + 1);
		nIndex = strTemp.Find('|');
		CString strLeft = strTemp.Left(nIndex);
		CString strAnswer("A: ");
		strAnswer += strLeft;
		
		//�ؼ�����
		rectctl.left = rect.left;
		rectctl.top = rect.top + FONTHEIGHT;
		rectctl.right = rectctl.left + ctlWidth * 1;
		rectctl.bottom = rectctl.top + FONTHEIGHT;	
		//��������
		m_bt[0].Create(strAnswer, WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,rectctl, this, 1);
		
		//3.��ʾ��B
		//��B�ַ���
		strRight = strTemp.Mid(nIndex +1);
		nIndex = strRight.Find('|');
		strLeft = strRight.Left(nIndex);		
		strAnswer = "B: ";
		strAnswer += strLeft;
		
		//�ؼ�����
		rectctl.left = rect.left + ctlWidth * 1;
		rectctl.top = rect.top + FONTHEIGHT;
		rectctl.right = rectctl.left + ctlWidth;
		rectctl.bottom = rectctl.top + FONTHEIGHT;
		//��������
		m_bt[1].Create(strAnswer, WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,rectctl, this, 2);
		
		//4.��ʾ��C
		//��C�ַ���
		strTemp = strRight.Mid(nIndex + 1);
		nIndex = strTemp.Find('|');
		strLeft = strTemp.Left(nIndex);		
		strAnswer = "C: ";
		strAnswer += strLeft;

		//�ؼ�����
		rectctl.left = rect.left + ctlWidth * 2;
		rectctl.top = rect.top + FONTHEIGHT;
		rectctl.right = rectctl.left + ctlWidth;
		rectctl.bottom = rectctl.top + FONTHEIGHT;
		//��������
		m_bt[2].Create(strAnswer, WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,rectctl, this, 3);		
		

		//5.��ʾ��D
		//��D�ַ���
		strRight = strTemp.Mid(nIndex +1);		
		strAnswer = "D: ";
		strAnswer += strRight;
		
		//�ؼ�����
		rectctl.left = rect.left + ctlWidth * 3;
		rectctl.top = rect.top + FONTHEIGHT;
		rectctl.right = rectctl.left + ctlWidth ;
		rectctl.bottom = rectctl.top + FONTHEIGHT;	
		//��������
		m_bt[3].Create(strAnswer, WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,rectctl, this, 4);
		
		//��������
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
							"����"); 	
		//Ϊÿ���ؼ���������
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
 * ѡ���
 */
afx_msg void CItem::OnSelected( UINT nCmdID )
{
	switch(nCmdID)
	{
	case 1://�ؼ�1
		m_bt[0].SetCheck(1);//����ѡ��״̬
		m_bt[1].SetCheck(0);//����δѡ��״̬
		m_bt[2].SetCheck(0);//����δѡ��״̬
		m_bt[3].SetCheck(0);//����δѡ��״̬
		m_nSelected = 0;	//��ǰ��ѡ��𰸵����
		break;
	case 2://�ؼ�2
		m_bt[0].SetCheck(0);
		m_bt[1].SetCheck(1);
		m_bt[2].SetCheck(0);
		m_bt[3].SetCheck(0);
		m_nSelected = 1;
		break;	
	case 3://�ؼ�3
		m_bt[0].SetCheck(0);
		m_bt[1].SetCheck(0);
		m_bt[2].SetCheck(1);
		m_bt[3].SetCheck(0);
		m_nSelected = 2;
		break;	
	case 4://�ؼ�4
		m_bt[0].SetCheck(0);
		m_bt[1].SetCheck(0);
		m_bt[2].SetCheck(0);
		m_bt[3].SetCheck(1);
		m_nSelected = 3;
		break;	
	default:break;		
	} 
}
