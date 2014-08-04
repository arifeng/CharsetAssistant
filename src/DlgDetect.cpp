// DlgDetect.cpp : implementation file
//

#include "stdafx.h"
#include "CharsetAssistant.h"
#include "DlgDetect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDetect dialog


CDlgDetect::CDlgDetect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDetect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDetect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgDetect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDetect)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDetect, CDialog)
	//{{AFX_MSG_MAP(CDlgDetect)
	ON_BN_CLICKED(IDC_BT_DET_SCAN, OnBtDetScan)
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDetect message handlers

BOOL CDlgDetect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::DragAcceptFiles();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDetect::OnBtDetScan() 
{
	CString strPath = SelectFile(_T("所有文件"), _T("*.*"));
	if (strPath.IsEmpty())
		return;
	
	SetDlgItemText(IDC_EDT_DET_FILE, strPath);
	DetectFile(strPath);
}

void CDlgDetect::DetectFile(CString strFile)
{
	double probability;
	std::string cs = utils::GetFileCharset(CStringToUTF8string(strFile), probability);
	
	if (cs.empty())
		AfxMessageBox(_T("文件编码类型未知！"));
	else
	{
		CString strMsg;
		strMsg.Format(_T("文件编码类型是：%s，可能性：%.2f%%"), UTF8stringToCString(cs), probability*100);
		AfxMessageBox(strMsg);
	}
}

//文件拖拽
void CDlgDetect::OnDropFiles(HDROP hDropInfo)
{
	TCHAR cFileName[MAX_PATH]; 
	UINT uFileCount, i = 0; 
	CString strPath;
	
	uFileCount = ::DragQueryFile(hDropInfo, -1, cFileName, sizeof(cFileName));   //拖拉的文件个数 
	::DragQueryFile(hDropInfo, i, cFileName, sizeof(cFileName));				//取得每个文件的文件名 
	
	strPath = cFileName;
	if (!PathIsDirectory(strPath))
	{
		SetDlgItemText(IDC_EDT_DET_FILE, strPath);
		DetectFile(strPath);
	}

	::DragFinish(hDropInfo);
	
	CDialog::OnDropFiles(hDropInfo);
}

void CDlgDetect::OnOK()
{
	CString strInput;
	GetDlgItemText(IDC_EDT_DET_INPUT, strInput);
	
	if (strInput.IsEmpty())
	{
		AfxMessageBox(_T("输入不能为空！"));
		return;
	}

	std::string sValid;
	for (int i = 0; i < strInput.GetLength(); i++)
	{
		TCHAR ch = strInput.GetAt(i);
		if (ISXDIGIT(ch))
			sValid += ch;
	}

	if (sValid.empty())
	{
		AfxMessageBox(_T("无有效十六进制字符序列！"));
		return;
	}

	int len = (sValid.length() + 1) / 2;
	char *data = (char*)xcalloc(len + 1, 1);
	if (!data)
	{
		AfxMessageBox(_T("内存分配失败！"));
		return;
	}

	for (i = 0; i < len; i++)
	{
		if (i < sValid.length() - 1)
			data[i] = X2DIGITS_TO_NUM(sValid.at(i*2), sValid.at(i*2+1));
		else
			break;
	}
	
	//检测编码类型
	CString strMsg;

	if (is_ascii(data, i))
		strMsg.Format(_T("字符串编码类型是 ASCII\n\n原文是：%s"), stringToCString(data));
	else if (is_utf8(data, i))
		strMsg.Format(_T("字符串编码类型是 UTF-8\n\n原文是：%s"), UTF8stringToCString(data));
	else if (is_gb2312(data, i))
		strMsg.Format(_T("字符串编码类型是 GB2312\n\n原文是：%s"), stringToCString(data));
	else if (is_gbk(data, i))
		strMsg.Format(_T("字符串编码类型是 GBK\n\n原文是：%s"), stringToCString(data));
	else
		strMsg = _T("编码类型未知！");
	
	xfree(data);
	AfxMessageBox(strMsg);
}

void CDlgDetect::OnCancel()
{
	SetDlgItemText(IDC_EDT_DET_INPUT, _T(""));
}
