// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__EB3EE12C_11E5_469C_B2E6_D7CD0A687337__INCLUDED_)
#define AFX_STDAFX_H__EB3EE12C_11E5_469C_B2E6_D7CD0A687337__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls

//Ӧ�ó�������
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")
#include <vector>
using namespace std;

#define		PAPER_BUFFER_SIZE	(1024*10)		//���⻺����1024*10
#define		ANSWER_BUFFER_SIZE	(100)			//�𰸻�����
#define		HEADELEN			(sizeof(HDR))	//��ͷ����
#define		BUFFER_SIZE			(128)			//�������ݻ�����64
#define		SERVERPORT			5561			//�������˿�
#define		PAPERHEADHEIGHT		90				//�Ծ�ͷ�߶�	
#define		ITEMLEFT			40				//������߾�
#define		ITEMHEIGHT			50				//����߶�

	
//������
#define		PULSE				100				//������	|�ͻ���
#define		STATE				101				//ѧ��״̬	|
#define		STNAME				102				//ѧ������	|������
#define		PAPER				103				//�Ծ�		|


//ѧ�ų���
#define		SNLEN				4				//ѧ�ų���
//��ͷ
typedef struct _header
{
	u_short		ustype;	//������
	u_short		usLen;	//���ݳ���
}HDR, *PHDER;

#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__EB3EE12C_11E5_469C_B2E6_D7CD0A687337__INCLUDED_)
