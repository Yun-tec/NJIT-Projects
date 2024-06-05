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

//应用程序声明
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")
#include <vector>
using namespace std;

#define		PAPER_BUFFER_SIZE	(1024*10)		//试题缓冲区1024*10
#define		ANSWER_BUFFER_SIZE	(100)			//答案缓冲区
#define		HEADELEN			(sizeof(HDR))	//包头长度
#define		BUFFER_SIZE			(128)			//接收数据缓冲区64
#define		SERVERPORT			5561			//服务器端口
#define		PAPERHEADHEIGHT		90				//试卷头高度	
#define		ITEMLEFT			40				//试题左边距
#define		ITEMHEIGHT			50				//试题高度

	
//包类型
#define		PULSE				100				//心跳包	|客户端
#define		STATE				101				//学生状态	|
#define		STNAME				102				//学生姓名	|服务器
#define		PAPER				103				//试卷		|


//学号长度
#define		SNLEN				4				//学号长度
//包头
typedef struct _header
{
	u_short		ustype;	//包类型
	u_short		usLen;	//数据长度
}HDR, *PHDER;

#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__EB3EE12C_11E5_469C_B2E6_D7CD0A687337__INCLUDED_)
