; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlgAbout
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "AriCharset.h"

ClassCount=7
Class1=CAriCharsetApp
Class2=CAriCharsetDlg

ResourceCount=7
Resource2=IDD_DLG_DETECT
Resource1=IDR_MAINFRAME
Class3=CDlgLookup
Resource3=IDD_DLG_HEXCAT
Class4=CDlgFileConv
Resource4=IDD_DLG_LOOKUP
Class5=CDlgDetect
Resource5=IDD_DLG_FILECONV
Class6=CDlgHexcat
Resource6=IDD_ARICHARSET_DIALOG
Class7=CDlgAbout
Resource7=IDD_DLG_ABOUT

[CLS:CAriCharsetApp]
Type=0
HeaderFile=AriCharset.h
ImplementationFile=AriCharset.cpp
Filter=N

[CLS:CAriCharsetDlg]
Type=0
HeaderFile=AriCharsetDlg.h
ImplementationFile=AriCharsetDlg.cpp
Filter=D
LastObject=CAriCharsetDlg



[DLG:IDD_ARICHARSET_DIALOG]
Type=1
Class=CAriCharsetDlg
ControlCount=0

[DLG:IDD_DLG_LOOKUP]
Type=1
Class=CDlgLookup
ControlCount=26
Control1=IDC_REDT_INPUT,RICHEDIT,1353781380
Control2=IDC_RDO_CS_GBK,button,1342308361
Control3=IDC_RDO_CS_GB2312,button,1342177289
Control4=IDC_RDO_CS_GB18030,button,1342177289
Control5=IDC_RDO_CS_BIG5,button,1342177289
Control6=IDC_RDO_CS_QUWEI,button,1342177289
Control7=IDC_RDO_CS_WUBI,button,1342177289
Control8=IDC_RDO_CS_UTF7,button,1342177289
Control9=IDC_RDO_CS_UTF8,button,1342177289
Control10=IDC_RDO_CS_UTF16LE,button,1342177289
Control11=IDC_RDO_CS_UTF16BE,button,1342177289
Control12=IDC_RDO_CS_UTF32LE,button,1342177289
Control13=IDC_RDO_CS_UTF32BE,button,1342177289
Control14=IDC_RDO_CS_OTHER,button,1342177289
Control15=IDC_ST_TIPS,static,1342308352
Control16=IDC_BT_HIGHLIGHT,button,1342242816
Control17=IDC_BT_SAVE,button,1342242816
Control18=IDC_RDO_CAT_HEX,button,1342308361
Control19=IDC_RDO_CAT_DEC,button,1342177289
Control20=IDC_RDO_CAT_BIN,button,1342177289
Control21=IDC_STATIC,button,1342177543
Control22=IDOK,button,1342242816
Control23=IDC_BT_CLEAR,button,1342242816
Control24=IDC_CB_OTHER_CHARSETS,combobox,1344340227
Control25=IDC_CHK_HEX_UPPER,button,1342242819
Control26=IDC_CHK_DEC_SIGNED,button,1342242819

[CLS:CDlgLookup]
Type=0
HeaderFile=DlgLookup.h
ImplementationFile=DlgLookup.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgLookup
VirtualFilter=dWC

[DLG:IDD_DLG_FILECONV]
Type=1
Class=CDlgFileConv
ControlCount=25
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,button,1342177287
Control3=IDC_RDO_SRC_FILE,button,1342177289
Control4=IDC_EDT_SRC_FILE,edit,1350631552
Control5=IDC_RDO_SRC_DIR,button,1342177289
Control6=IDC_EDT_SRC_DIR,edit,1350631552
Control7=IDC_CHK_RECUR_DIR,button,1342242819
Control8=IDC_BT_SCAN_FILE,button,1342242816
Control9=IDC_BT_SCAN_SRC_DIR,button,1342242816
Control10=IDC_EDT_DST_DIR,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_BT_SCAN_DST_DIR,button,1342242816
Control13=IDC_CHK_SAME_DIR,button,1342242819
Control14=IDC_STATIC,static,1342308352
Control15=IDC_CB_SRC_CHARSET,combobox,1344340227
Control16=IDC_STATIC,static,1342308352
Control17=IDC_CB_DST_CHARSET,combobox,1344340227
Control18=IDC_RDO_FILTER_NONE,button,1342177289
Control19=IDC_RDO_FILTER_TYPE,button,1342177289
Control20=IDC_RDO_FILTER_WILDCARD,button,1342177289
Control21=IDC_PROGRESS1,msctls_progress32,1350565888
Control22=IDOK,button,1342251009
Control23=IDC_CHK_WRITE_BOM,button,1342242819
Control24=IDC_STATIC,button,1342177287
Control25=IDC_ST_PROGRESS,static,1342308353

[CLS:CDlgFileConv]
Type=0
HeaderFile=DlgFileConv.h
ImplementationFile=DlgFileConv.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgFileConv
VirtualFilter=dWC

[DLG:IDD_DLG_DETECT]
Type=1
Class=CDlgDetect
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDT_DET_FILE,edit,1350631552
Control6=IDC_BT_DET_SCAN,button,1342242816
Control7=IDC_STATIC,button,1342177287
Control8=IDC_EDT_DET_INPUT,edit,1352732868
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352

[CLS:CDlgDetect]
Type=0
HeaderFile=DlgDetect.h
ImplementationFile=DlgDetect.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgDetect
VirtualFilter=dWC

[DLG:IDD_DLG_HEXCAT]
Type=1
Class=CDlgHexcat
ControlCount=5
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDT_HEXCAT_FILE,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDC_EDT_HEXCAT_CONTENT,edit,1352734916
Control5=IDC_STATIC,static,1342308352

[CLS:CDlgHexcat]
Type=0
HeaderFile=DlgHexcat.h
ImplementationFile=DlgHexcat.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgHexcat
VirtualFilter=dWC

[DLG:IDD_DLG_ABOUT]
Type=1
Class=CDlgAbout
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[CLS:CDlgAbout]
Type=0
HeaderFile=DlgAbout.h
ImplementationFile=DlgAbout.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgAbout

