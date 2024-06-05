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


/*应用程序定义*/
#include <afxtempl.h> 
#include <Afxmt.h>
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF", "adoEOF") 
#include "vector"

using namespace std;

#define		PAPER_BUFFER_SIZE	(1024*10)					//试题缓冲区1024*10
#define		ANSWER_BUFFER_SIZE	(50)						//答案缓冲区
#define		HEADELEN			(sizeof(HDR))				//包头长度
#define		OVERLAPPEDPLUSLEN	(sizeof(IO_OPERATION_DATA))	//扩展重叠结构长度
#define		SERVERPORT			5561						//服务器端口
#define		BUFFER_SIZE			(128)						//接收数据缓冲区64


//最大子线程数量
#define		MAX_SUBTHREAD_SIZE	(5)							//最多子线程数量 

//学号长度
#define		SNLEN				4							//学号长度

//I/O操作类型
#define		IOReadHead			10							//接收包头
#define		IOReadBody			11							//接收包体
#define		IOWriteUnLogin		12							//验证失败
#define		IOWriteSTNAME		13							//发送姓名
#define		IOWritePAPER		14							//发送试卷
#define		IOEXIT				15							//客户端退出

	

//包类型
#define		PULSE				100							//心跳包	|客户端
#define		STATE				101							//学生状态	|
#define		STNAME				102							//学生姓名	|服务器
#define		PAPER				103							//试卷		|
//包头
typedef struct _header
{
	u_short		ustype;	//包类型
	u_short		usLen;	//数据长度
}HDR, *PHDER;

//考生信息
typedef struct _studentinfo
{
	CString	strSN;		//学号S:student N:Number
	CString	strName;	//姓名
	CString	strState;	//状态
	u_short usGrade;	//分数
}STUDENTINFO, *PSTUDENTINFO;

// I/O 操作数据结构
typedef	struct _io_operation_data 
{
	OVERLAPPED	overlapped;					//重叠结构
	char		recvBuf[BUFFER_SIZE];		//接收数据缓冲区
	HDR			hdr;						//包头
	byte		type;						//操作类型
}IO_OPERATION_DATA, *PIO_OPERATION_DATA;

#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6744DD28_AEEE_434F_B0DE_7C9355A5CE43__INCLUDED_)
