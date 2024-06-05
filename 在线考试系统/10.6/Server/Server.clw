; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Server.h"
LastPage=0

ClassCount=5
Class1=CServerApp
Class2=CServerDoc
Class3=CServerView
Class4=CMainFrame

ResourceCount=3
Resource1=IDD_SERVER_ADDRESS_DLG (English (U.S.))
Resource2=IDD_ABOUTBOX
Class5=CAboutDlg
Resource3=IDR_MAINFRAME

[CLS:CServerApp]
Type=0
HeaderFile=Server.h
ImplementationFile=Server.cpp
Filter=N

[CLS:CServerDoc]
Type=0
HeaderFile=ServerDoc.h
ImplementationFile=ServerDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC

[CLS:CServerView]
Type=0
HeaderFile=ServerView.h
ImplementationFile=ServerView.cpp
Filter=C
BaseClass=CListView
VirtualFilter=VWC
LastObject=CServerView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_SERVER_EXIT




[CLS:CAboutDlg]
Type=0
HeaderFile=Server.cpp
ImplementationFile=Server.cpp
Filter=D
LastObject=IDOK

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_SERVER_START
Command2=ID_SERVER_EXIT
Command3=ID_VIEW_TOOLBAR
Command4=ID_VIEW_STATUS_BAR
Command5=ID_APP_ABOUT
CommandCount=5

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_SERVER_START
Command2=ID_SERVER_EXIT
Command3=ID_APP_ABOUT
CommandCount=3

[DLG:IDD_SERVER_ADDRESS_DLG (English (U.S.))]
Type=1
Class=?
ControlCount=7
Control1=IDC_SERVER_IPADDRESS,SysIPAddress32,1342242816
Control2=IDC_SERVER_PORT_EDIT,edit,1350639744
Control3=IDOK,button,1342242816
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342308352

