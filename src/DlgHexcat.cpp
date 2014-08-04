// DlgHexcat.cpp : implementation file
//

#include "stdafx.h"
#include "CharsetAssistant.h"
#include "DlgHexcat.h"
#include <fstream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHexcat dialog


CDlgHexcat::CDlgHexcat(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHexcat::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHexcat)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgHexcat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHexcat)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHexcat, CDialog)
	//{{AFX_MSG_MAP(CDlgHexcat)
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHexcat message handlers


BOOL CDlgHexcat::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::DragAcceptFiles();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgHexcat::OnOK() 
{
	CString strFile = SelectFile(_T("选择要查看的文件"), _T("*.*"));
	if (strFile.IsEmpty())
		return;
	
	SetDlgItemText(IDC_EDT_HEXCAT_FILE, strFile);
	HexCatFile(strFile);
}

//查看区域共分为3列
//第一列是16进制表示得字符
//第二列是原文的16进制表示
//第三列是原文的ASCII码表示

#define LINE_COUNT	10		//每行表示得字符数
#define SPACE12		4		//第一、二列之间的空格数
#define SPACE23		6		//第二、三列之间的空格数

static _inline BYTE ascii_char_cat(BYTE c)
{
	if (c <= 32 || c >= 127)	//32是空格
		return '.';
	else
		return c;
}

void CDlgHexcat::HexCatFile(CString strFile)
{
	ifstream fin;
	BYTE buf[LINE_COUNT];
	CString strContent, strNum, strHex, strASCII, strChar;
	CString strSpace12(' ', SPACE12);
	CString strSpace23(' ', SPACE23);
	int count = 0;
	
	fin.open(CStringTostring(strFile).c_str(), ios::in|ios::binary);
	if (fin.bad())
	{
		AfxMessageBox(_T("无法读取文件！"));
		return;
	}
	
	//检测文件中的每一行
	while(!fin.eof()){
		
		memset(buf, 0, LINE_COUNT);
		fin.read((char*)buf, LINE_COUNT);
		
		//字符序号
		strNum.Format(_T("%.4x"), count * LINE_COUNT);
		count++;
		strContent += strNum;
		strContent += strSpace12;
		
		//获取使用十六进制编码字符串，如 “e4 b8 ad”
		strHex = _T("");
		for (int j = 0; j < LINE_COUNT; j++)
		{
			strChar.Format(_T("%.2X"), (unsigned char)buf[j]);
			
			strHex += strChar;
			if (j != LINE_COUNT - 1)
				strHex += ' ';
		}
		strContent += strHex;
		strContent += strSpace23;
		
		//获取ASCII码表示字符串
		strASCII = _T("");
		for (j = 0; j < LINE_COUNT; j++)
		{
			strASCII += (char)ascii_char_cat(buf[j]);
			if (j != LINE_COUNT - 1)
				strASCII += ' ';
		}
		strContent += strASCII;
		
		if (!fin.eof())
			strContent += "\r\n";
	};
	
	SetDlgItemText(IDC_EDT_HEXCAT_CONTENT, strContent);
}

//文件拖拽
void CDlgHexcat::OnDropFiles(HDROP hDropInfo)
{
	TCHAR cFileName[MAX_PATH]; 
	UINT uFileCount, i = 0; 
	CString strPath;
	
	uFileCount = ::DragQueryFile(hDropInfo, -1, cFileName, sizeof(cFileName));   //拖拉的文件个数 
	::DragQueryFile(hDropInfo, i, cFileName, sizeof(cFileName));				//取得每个文件的文件名 
	
	strPath = cFileName;
	if (!PathIsDirectory(strPath))
	{
		SetDlgItemText(IDC_EDT_HEXCAT_FILE, strPath);
		HexCatFile(strPath);
	}
	
	::DragFinish(hDropInfo);
	
	CDialog::OnDropFiles(hDropInfo);
}
