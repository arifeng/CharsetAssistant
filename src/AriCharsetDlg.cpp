// AriCharsetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CharsetAssistant.h"
#include "AriCharsetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAriCharsetDlg dialog

CAriCharsetDlg::CAriCharsetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAriCharsetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAriCharsetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAriCharsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAriCharsetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAriCharsetDlg, CDialog)
	//{{AFX_MSG_MAP(CAriCharsetDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAriCharsetDlg message handlers

int CAriCharsetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(m_TabCtrl.Create(this, WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),3001)==false) 
		return -1;
	
	if(m_dlgLookup.Create(IDD_DLG_LOOKUP, &m_TabCtrl)==0) 
		return -1;
	m_dlgLookup.SetDlgCtrlID(3010);

	if(m_dlgFileConv.Create(IDD_DLG_FILECONV, &m_TabCtrl)==0) 
		return -1;
	m_dlgFileConv.SetDlgCtrlID(3011);

	if(m_dlgDetect.Create(IDD_DLG_DETECT, &m_TabCtrl)==0) 
		return -1;
	m_dlgDetect.SetDlgCtrlID(3012);

	if(m_dlgHexcat.Create(IDD_DLG_HEXCAT, &m_TabCtrl)==0)
		return -1;
	m_dlgHexcat.SetDlgCtrlID(3013);

	if(m_dlgAbout.Create(IDD_DLG_ABOUT, &m_TabCtrl)==0)
		return -1;
	m_dlgAbout.SetDlgCtrlID(3014);

	return 0;
}

BOOL CAriCharsetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO:  在此添加您专用的创建代码

	CFont font;
	font.CreatePointFont(85,_T("Tahoma"));
	m_TabCtrl.SetFont(&font);
	m_TabCtrl.SetFontSelect(&font);
	
	//m_TabCtrl.SetCursor(IDC_CURSOR1);
	m_TabCtrl.RemoveTabEnable(true);
	// 
	//m_TabCtrl.SetNotifyManager(this);
	//
	m_TabCtrl.Add(m_dlgLookup, _T(" 字符编码查询 "), -1);
	m_TabCtrl.Add(m_dlgFileConv, _T(" 文件编码转换 "), -1);
	m_TabCtrl.Add(m_dlgDetect, _T(" 未知编码检测  "), -1);
	m_TabCtrl.Add(m_dlgHexcat, _T(" 十六进制查看  "), -1);
	m_TabCtrl.Add(m_dlgAbout, _T(" 帮助 && 关于  "), -1);

	m_TabCtrl.InstallStyle(&m_TabCtrlStyle);
	m_TabCtrl.Update();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAriCharsetDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAriCharsetDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAriCharsetDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	if (m_TabCtrl.GetSafeHwnd())
	{
		CRect rc;
		
		GetClientRect(rc);
		m_TabCtrl.MoveWindow(&rc);
	}
}


void CAriCharsetDlg::OnOK()
{
	return;
}