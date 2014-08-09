// DlgLookup.cpp : implementation file
//

#include "stdafx.h"
#include "CharsetAssistant.h"
#include "DlgLookup.h"

#include "WubiCode.h"
#include "../utils/src/cutil.h"

using namespace std;

struct text_range{
	int		start;
	int		end;
};


/////////////////////////////////////////////////////////////////////////////
// CDlgLookup dialog


CDlgLookup::CDlgLookup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLookup::IDD, pParent)
{
	m_clrHighLight = RGB(255,0,0);
}


void CDlgLookup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_HIGHLIGHT, m_btHighlight);
	DDX_Control(pDX, IDC_ST_TIPS, m_lbTips);
	DDX_Control(pDX, IDC_CB_OTHER_CHARSETS, m_cbCharsets);
	DDX_Control(pDX, IDC_REDT_INPUT, m_edtText);
	DDX_Control(pDX, IDC_RDO_CAT_HEX, m_rdHexCat);
	DDX_Control(pDX, IDC_RDO_CAT_DEC, m_rdOctCat);
	DDX_Control(pDX, IDC_RDO_CAT_BIN, m_rdBinCat);
	DDX_Control(pDX, IDC_CHK_HEX_UPPER, m_ckCapital);
	DDX_Control(pDX, IDC_CHK_DEC_SIGNED, m_ckSigned);
	DDX_Control(pDX, IDOK, m_btLookup);
	DDX_Control(pDX, IDC_BT_CLEAR, m_btClear);
	DDX_Control(pDX, IDC_BT_SAVE, m_btSave);

	//{{AFX_DATA_MAP(CDlgLookup)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLookup, CDialog)
	//{{AFX_MSG_MAP(CDlgLookup)
	ON_COMMAND_RANGE(IDC_RDO_CS_GBK, IDC_RDO_CS_OTHER, OnSelectCharset)
	ON_COMMAND_RANGE(IDC_RDO_CAT_HEX, IDC_RDO_CAT_BIN, OnSelectScale)
	ON_CBN_SELCHANGE(IDC_CB_OTHER_CHARSETS, OnSelOtherCharsets)
	ON_COMMAND(IDC_BT_CLEAR, OnClear)
	ON_COMMAND(IDC_CHK_HEX_UPPER, ReLookup)
	ON_COMMAND(IDC_CHK_DEC_SIGNED, ReLookup)
	ON_COMMAND(IDC_BT_HIGHLIGHT, OnBnClickedBtColor)
	ON_COMMAND(IDC_BT_SAVE, OnBnClickedBtSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLookup message handlers

BOOL CDlgLookup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_btHighlight.SetFlat(FALSE);
	m_btHighlight.SetColor(CButtonST::BTNST_COLOR_FG_IN, m_clrHighLight);
	m_btHighlight.SetColor(CButtonST::BTNST_COLOR_FG_OUT, m_clrHighLight);
	m_btHighlight.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, m_clrHighLight);    
	
	m_btSave.EnableWindow(FALSE);

	m_lbTips.SetTextColor(RGB(255,128,0));
	m_lbTips.FlashText(TRUE);

	//所有支持的编码
	char *p, *charsets;
	charsets = strdupa(supported_charsets);
	while (p = strsep(&charsets, " "))
		m_cbCharsets.AddString(stringToCString(p));
	m_cbCharsets.SelectString(0, _T("ASCII"));

	//默认查询GBK编码
	((CButton*)GetDlgItem(IDC_RDO_CS_GBK))->SetCheck(TRUE);
	m_strCharset = _T("GBK");

	//默认以十六进制大写表示
	m_rdHexCat.SetCheck(TRUE);
	m_ckCapital.SetCheck(TRUE);
	m_ckSigned.EnableWindow(FALSE);

	//Tooltip
	m_tooltip.Create(this);
	m_tooltip.SetSize(CPPToolTip::PPTTSZ_MARGIN_CX, 8);
	m_tooltip.SetSize(CPPToolTip::PPTTSZ_MARGIN_CY, 8);
	m_tooltip.AddTool(&m_btHighlight, _T("更改编码高亮颜色"));
	m_tooltip.AddTool(&m_btSave, _T("保存查询结果到rtf文件"));
	
	m_tooltip.AddTool(GetDlgItem(IDC_RDO_CS_GB2312), _T("GB2312编码用2个字节表示一个汉字\n编码范围是0xA1A1~0xF7FE，\n共收录6763个汉字和682个符号。"));
	m_tooltip.AddTool(GetDlgItem(IDC_RDO_CS_GBK), _T("GBK编码是GB2312的超集，用2个字节表示一个汉字，\n编码范围是0x8140~0xFEFE（不包括0x??7F一条线）\n共收入21886个汉字和883个图形符号。\nWindows对应代码页：CP936。"));
	m_tooltip.AddTool(GetDlgItem(IDC_RDO_CS_GB18030), _T("GB18030编码是GBK/GB2312的超集，\n用2或4个字节表示一个汉字，\nGB18030-2005标准共定义了76556个字符。"));
	m_tooltip.AddTool(GetDlgItem(IDC_RDO_CS_WUBI), _T("五笔字型码"));
	m_tooltip.AddTool(GetDlgItem(IDC_RDO_CS_QUWEI), _T("汉字区位码"));

	m_tooltip.AddTool(GetDlgItem(IDC_RDO_CS_UTF8), _T("UTF-8是UNICODE的一种变长字符编码，又称万国码，\n用1~6个字节表示一个字符（汉字占3个字节）。\nWindows对应代码页：CP65001。"));
	m_tooltip.AddTool(GetDlgItem(IDC_RDO_CS_UTF16LE), _T("在windows下被称作“Unicode编码”，每个字符占2个字节。\nUTF-16可看成是UCS-2的父集，在没有辅助平面前UTF-16就等于UCS-2"));
	m_tooltip.AddTool(GetDlgItem(IDC_RDO_CS_UTF16BE), _T("UTF-16的大端表示法"));
	m_tooltip.AddTool(GetDlgItem(IDC_RDO_CS_UTF32LE), _T("UTF-32 LE仅使用了unicode范围(0到0x10FFFF)的32位编码, 因占空间大(每字符4个字节)较少被使用"));
	m_tooltip.AddTool(GetDlgItem(IDC_RDO_CS_UTF32BE), _T("UTF-32的大端表示法，类似UTF-32LE"));
	m_tooltip.AddTool(GetDlgItem(IDC_RDO_CS_BIG5), _T("BIG-5码是通行于台湾、香港地区的一个繁体字编码方案，\n俗称“大五码”。共收录13,060个中文字\n"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#define OUTBUF	10

void CDlgLookup::OnOK()
{
	CStringW strFrom, strTo;
	CStringW strOutput, strText;
	std::string from_cs, to_cs;
	const char *inbuf;
	char outbuf[OUTBUF];
	size_t inlen, outlen;
	bool bWarned = false;

	if (m_strCharset.IsEmpty())
	{
		AfxMessageBox(_T("请选择一种字符编码！"));
		return;
	}

	strFrom = _T("UTF-16LE");
	strTo = m_strCharset;
	from_cs = CStringTostring(strFrom);			//原字符集
	to_cs = CStringTostring(strTo);				//目标字符集

	if (strTo == "SECTPOS")
		to_cs = "GB2312";						//区码码对应汉字内码

	//strInput中存放着Unicode (UTF-16LE)格式编码的字符串
	GetDlgItemText(IDC_REDT_INPUT, m_strInput);

	//对每个Unicode字符进行转换
	int pos = 0;
	list<text_range> scope_list;
	list<text_range>::iterator iter;

	for (int i = 0; i < m_strInput.GetLength(); i++)
	{
		wchar_t wch;
		CString strChar, strCode;
		text_range scope;

		//待转换字符
		wch = m_strInput.GetAt(i);

		//初始化输入缓冲区、长度等
		inbuf = (const char*)&wch;
		inlen = sizeof(wch);
		outlen = OUTBUF;

		//回车换行符
		if (wch == '\r' || wch == '\n')
		{
			strOutput += wch;
			pos += 1;
			continue;
		}

		//////////////////////////////////////////////////////////////////////////

		//五笔字型编码查询
		if (strTo == _T("WUBI"))
		{
			std::string res = CWubiCode::Find(std::wstring(1, wch));
			if (!res.empty() && res.length() <= 4)
			{
				memset(outbuf, 0, OUTBUF);
				strncpy(outbuf, res.c_str(), res.length());
				outlen = res.length();
				strCode = outbuf;
			}
			else
			{
				strOutput += wch;
				pos += wch > CHAR_MAX ? 2 : 1;
				continue;
			}
		}
		//汉字区位码
		else if (strTo == _T("SECTPOS"))
		{
			//区位码是基于GBK编码的，需先转换为GBK编码
			char*p = outbuf;
			if (wch > 127 && convert_to_charset(from_cs.c_str(), "GBK", inbuf, inlen, &p, &outlen, 1))
			{
				outbuf[0] &= ~(1<<7);
				outbuf[0] -= 32;
				outbuf[1] &= ~(1<<7);
				outbuf[1] -= 32;
				strCode.Format(_T("%.2d%.2d"), outbuf[0], outbuf[1]);
			} else {
				strOutput += wch;
				pos += wch > CHAR_MAX ? 2 : 1;
				continue;
			}
		}
		//其他编码使用ICONV进行转换，并将目标字符串长度赋值给outlen
		else
		{
			char *p = outbuf;
			if (!convert_to_charset(from_cs.c_str(), to_cs.c_str(), inbuf, inlen, &p, &outlen, 1)) {
				if (!bWarned)
				{
					if (errno == E2BIG)
						AfxMessageBox(_T("目标缓冲区太小无法保存转换后的字符"));
					else if (errno == EILSEQ)
						AfxMessageBox(_T("待转换内容包含目标编码无法识别的字符"));
					if (errno == EINVAL)
						AfxMessageBox(_T("不完全的待转换内容序列"));
					
					bWarned = true;
				}
				strOutput += wch;
				pos += wch > CHAR_MAX ? 2 : 1;
			    continue;
			} else {
				for (int j = 0; j < outlen; j++)
				{
					//十六进制显示方式
					if (m_rdHexCat.GetCheck())
					{
						if (m_ckCapital.GetCheck())
							strChar.Format(_T("%.2X"), (unsigned char)outbuf[j]);
						else
							strChar.Format(_T("%.2x"), (unsigned char)outbuf[j]);
					}
					//十进制显示方式
					else if (m_rdOctCat.GetCheck())
					{
						if (!m_ckSigned.GetCheck())
							strChar.Format(_T("%.2d"), (unsigned char)outbuf[j]);
						else
							strChar.Format(_T("%.2d"), outbuf[j]);
					}
					else
						//二进制查看方式
					{
						strChar = _T("");
						for (int i = 1; i <= 128; i<<=1)
						{
							if (outbuf[j] & i)
								strChar.Insert(0, _T("1"));
							else
								strChar.Insert(0, _T("0"));
						}
					}
					
					strCode += strChar;
					if (j != outlen - 1)
						strCode += _T(" ");
				}
			}
		}

		//输出一个字符及其编码表示，如“中(e4 b8 ad) ”，"李(3287) "，"呵(ksk)"
		strOutput += wch;
		strOutput += _T("(");
		strOutput += strCode;
		strOutput += _T(") ");

		//此算此编码的高亮范围
		pos += wch > CHAR_MAX ? 2 : 1;
		scope.start = pos + 1 /* '(' */;
		scope.end = scope.start + strCode.GetLength();
		scope_list.push_back(scope);

		pos = scope.end + 2 /* ") " */;

	} //foreach character

	//设置输出内容
	SetDlgItemText(IDC_REDT_INPUT, strOutput);

	//编码高亮显示
	for (iter = scope_list.begin(); iter != scope_list.end(); iter++)
	{
		m_edtText.SetSel(iter->start, iter->end);
		m_edtText.SetColour(m_clrHighLight);
	}
	
	m_edtText.SetSel(0, strOutput.GetLength());
	m_edtText.SetFontSize(13);
	m_edtText.SetReadOnly(TRUE);

	m_btLookup.EnableWindow(FALSE);
	m_btClear.EnableWindow(TRUE);
	m_btSave.EnableWindow(TRUE);	
}

void CDlgLookup::ReLookup()
{
	//用户在查询了一种编码后想要快速查询其他编码的结果
	if (!m_strInput.IsEmpty() && !m_btLookup.IsWindowEnabled())
	{
		m_edtText.SetWindowText(m_strInput);
		OnOK();
	}
}

void CDlgLookup::OnClear()
{
	m_strInput = _T("");
	m_edtText.SetWindowText(_T(""));
	
	m_edtText.SetScrollRange(SB_VERT, 0,0);
	m_edtText.SetScrollRange(SB_HORZ, 0,0);

	m_edtText.SetReadOnly(FALSE);
	m_btClear.EnableWindow(FALSE);
	m_btSave.EnableWindow(FALSE);
	m_btLookup.EnableWindow(TRUE);
}

#define CASE_CHARSET(idc, s)	\
		case idc:				\
		m_strCharset = _T(s);	\
		bValid = true;		\
		break

void CDlgLookup::OnSelectCharset(UINT nID)
{
	bool bValid = false;
	
	switch(nID)
	{
	CASE_CHARSET(IDC_RDO_CS_GBK, "GBK");
	CASE_CHARSET(IDC_RDO_CS_GB2312, "GB2312");
	CASE_CHARSET(IDC_RDO_CS_GB18030, "GB18030");
	CASE_CHARSET(IDC_RDO_CS_BIG5, "BIG5");
	CASE_CHARSET(IDC_RDO_CS_QUWEI, "SECTPOS");
	CASE_CHARSET(IDC_RDO_CS_WUBI, "WUBI");
	CASE_CHARSET(IDC_RDO_CS_UTF7, "UTF-7");
	CASE_CHARSET(IDC_RDO_CS_UTF8, "UTF-8");
	CASE_CHARSET(IDC_RDO_CS_UTF16LE, "UTF-16LE");
	CASE_CHARSET(IDC_RDO_CS_UTF16BE, "UTF-16BE");
	CASE_CHARSET(IDC_RDO_CS_UTF32LE, "UTF-32LE");
	CASE_CHARSET(IDC_RDO_CS_UTF32BE, "UTF-32BE");
	case IDC_RDO_CS_OTHER:
	m_cbCharsets.GetWindowText(m_strCharset);
	bValid = true;
	break;
	default:
	NOT_REACHED();
	m_strCharset = _T("");
	}

	if (bValid)
		ReLookup();
}

void CDlgLookup::OnSelectScale(UINT nID)
{
	switch(nID)
	{
	case IDC_RDO_CAT_HEX:
		m_ckCapital.EnableWindow(TRUE);
		m_ckSigned.EnableWindow(FALSE);
		break;
	case IDC_RDO_CAT_DEC:
		m_ckCapital.EnableWindow(FALSE);
		m_ckSigned.EnableWindow(TRUE);
		break;
	case IDC_RDO_CAT_BIN:
		m_ckCapital.EnableWindow(FALSE);
		m_ckSigned.EnableWindow(FALSE);
		break;
	default:
		NOT_REACHED();
		break;
	}

	ReLookup();
}

//更改高亮颜色
void CDlgLookup::OnBnClickedBtColor()
{
	CColorDialog FarbDlg;
	if( FarbDlg.DoModal() == IDOK )
	{
		m_clrHighLight = FarbDlg.GetColor();
		m_btHighlight.SetColor(CButtonST::BTNST_COLOR_FG_IN, m_clrHighLight);
		m_btHighlight.SetColor(CButtonST::BTNST_COLOR_FG_OUT, m_clrHighLight);
		m_btHighlight.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, m_clrHighLight);
		
		ReLookup();
	}
}

void CDlgLookup::OnSelOtherCharsets()
{
	m_cbCharsets.GetLBText(m_cbCharsets.GetCurSel(), m_strCharset);
	ReLookup();
}

void CDlgLookup::OnBnClickedBtSave()
{
	static CString strInitDir = _T("");
	static CString strInitExt = _T("rtf");
	static CString strInitFil = _T("编码转换结果.rtf");
	static CString strInitNamen = _T("RTF-Files (*.rtf)|All files (*.*)|*.*||");
	
	CString strDateiname;
	if (strInitDir.IsEmpty())	 
	{
		LPTSTR pstr = strInitDir.GetBuffer(1032);
		GetCurrentDirectory(1024,pstr);
		strInitDir.ReleaseBuffer();
	}
	
	UpdateData(true);
	
	CFileDialog DateiDlg (false, strInitExt ,strInitFil,0,strInitNamen);
	DateiDlg.m_ofn.Flags |= OFN_PATHMUSTEXIST;
	DateiDlg.m_ofn.lpstrInitialDir = strInitDir;
	
	if(IDOK == DateiDlg.DoModal())
	{ strDateiname = DateiDlg.GetPathName();
	int iPos = strDateiname.ReverseFind(TCHAR('\\'));
	if( -1 != iPos )
	{ strInitDir = strDateiname.Left(iPos);
	}
	CFileStatus fileStatus;
	BOOL bReturnStatus = FALSE;
	bReturnStatus = CFile::GetStatus(strDateiname,fileStatus);
	if( bReturnStatus )
	{ int iReturn = AfxMessageBox(_T("文件已经存在，要覆盖它吗？"),MB_YESNOCANCEL);
	if( IDCANCEL == iReturn ) return;
	if( IDNO == iReturn )
	{ return;
	}
	}
	m_edtText.WriteRTF(strDateiname);
	AfxMessageBox(_T("查询结果保存成功！"));
	} 
}

BOOL CDlgLookup::PreTranslateMessage(MSG* pMsg)
{
	m_tooltip.RelayEvent(pMsg);
	
	return CDialog::PreTranslateMessage(pMsg);
}
