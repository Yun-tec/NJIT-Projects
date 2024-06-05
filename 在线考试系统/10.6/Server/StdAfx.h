// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__6744DD28_AEEE_434F_B0DE_7C9355A5CE43__INCLUDED_)
#define AFX_STDAFX_H__6744DD28_AEEE_434F_B0DE_7C9355A5CE43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls


/*Ӧ�ó�����*/
#include <afxtempl.h> 
#include <Afxmt.h>
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF", "adoEOF") 
#include "vector"

using namespace std;

#define		PAPER_BUFFER_SIZE	(1024*10)					//���⻺����1024*10
#define		ANSWER_BUFFER_SIZE	(50)						//�𰸻�����
#define		HEADELEN			(sizeof(HDR))				//��ͷ����
#define		OVERLAPPEDPLUSLEN	(sizeof(IO_OPERATION_DATA))	//��չ�ص��ṹ����
#define		SERVERPORT			5561						//�������˿�
#define		BUFFER_SIZE			(128)						//�������ݻ�����64


//������߳�����
#define		MAX_SUBTHREAD_SIZE	(5)							//������߳����� 

//ѧ�ų���
#define		SNLEN				4							//ѧ�ų���

//I/O��������
#define		IOReadHead			10							//���հ�ͷ
#define		IOReadBody			11							//���հ���
#define		IOWriteUnLogin		12							//��֤ʧ��
#define		IOWriteSTNAME		13							//��������
#define		IOWritePAPER		14							//�����Ծ�
#define		IOEXIT				15							//�ͻ����˳�

	

//������
#define		PULSE				100							//������	|�ͻ���
#define		STATE				101							//ѧ��״̬	|
#define		STNAME				102							//ѧ������	|������
#define		PAPER				103							//�Ծ�		|
//��ͷ
typedef struct _header
{
	u_short		ustype;	//������
	u_short		usLen;	//���ݳ���
}HDR, *PHDER;

//������Ϣ
typedef struct _studentinfo
{
	CString	strSN;		//ѧ��S:student N:Number
	CString	strName;	//����
	CString	strState;	//״̬
	u_short usGrade;	//����
}STUDENTINFO, *PSTUDENTINFO;

// I/O �������ݽṹ
typedef	struct _io_operation_data 
{
	OVERLAPPED	overlapped;					//�ص��ṹ
	char		recvBuf[BUFFER_SIZE];		//�������ݻ�����
	HDR			hdr;						//��ͷ
	byte		type;						//��������
}IO_OPERATION_DATA, *PIO_OPERATION_DATA;

#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6744DD28_AEEE_434F_B0DE_7C9355A5CE43__INCLUDED_)
