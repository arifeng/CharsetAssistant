#include "Stdafx.h"
#include "MfcUtil.h"
#include <math.h>

#pragma warning(disable: 4244)

namespace mfcutil{

CString GetModulePath()
{
	CString rString;
	GetModulePath(rString);

	return rString;
}

void GetModulePath(CString &rString)
{
	TCHAR szValue[MAX_PATH];
	memset(szValue,0,sizeof(szValue));

	GetModuleFileName(NULL,szValue,sizeof(szValue));
	TCHAR *slash_end = wcsrchr(szValue,'\\');
	if(slash_end != NULL) 
		slash_end[1]='\x00';

	rString = szValue;
}

CString SelectFile(CString desc, CString filter)
{
	CString strPath, strFilter;

	strFilter.Format(_T("%s (%s)|%s|"), desc, filter, filter);
	CFileDialog dlg(TRUE, _T(""), NULL, 0, strFilter);
	if (dlg.DoModal() != IDOK)
		return _T("");

	return dlg.GetPathName();
}

CString SelectDirectory(CString title)
{
	BROWSEINFO bi;					
	TCHAR Buffer[512]= _T(" "); 
	TCHAR FullPath[512]= _T(" "); 
	bi.hwndOwner = NULL;	
	bi.pidlRoot = NULL; 
	bi.pszDisplayName = Buffer;			
	bi.lpszTitle = title;			
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;                           
	bi.lParam = 0; 
	bi.iImage = 0; 

#if _MSC_VER > 1200
	bi.ulFlags |= BIF_NEWDIALOGSTYLE;
#endif

	ITEMIDLIST* pidl = ::SHBrowseForFolder(&bi);   
	if(::SHGetPathFromIDList(pidl,FullPath)) 
		return FullPath;

	return _T("");
}

void ShellExec(const CString &strFile, const CString &strParam, 
				DWORD dwWait, int nShow)
{
	SHELLEXECUTEINFO ShExecInfo = {0}; 
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); 
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL; 
	ShExecInfo.lpFile = strFile; 
	ShExecInfo.lpParameters = strParam; 
	ShExecInfo.lpDirectory = NULL; 
	ShExecInfo.nShow = nShow; 
	ShExecInfo.hInstApp = NULL; 

	ShellExecuteEx(&ShExecInfo); 

	if (dwWait)
		WaitForSingleObject(ShExecInfo.hProcess, dwWait);

	CloseHandle(ShExecInfo.hProcess);
}

BOOL Shutdown()
{
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 

	// Get a token for this process. 
	if (!OpenProcessToken(GetCurrentProcess(), 
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		return( FALSE ); 
	// Get the LUID for the shutdown privilege. 
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
		&tkp.Privileges[0].Luid); 
	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	// Get the shutdown privilege for this process. 
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
		(PTOKEN_PRIVILEGES)NULL, 0); 
	if (GetLastError() != ERROR_SUCCESS) 
		return FALSE; 

	// Shut down the system and force all applications to close.
	if (!ExitWindowsEx(EWX_POWEROFF, 0
		//,SHTDN_REASON_MAJOR_SOFTWARE | SHTDN_REASON_FLAG_PLANNED
		)) 
		return FALSE; 

	//shutdown was successful
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//字符串、编码转换

namespace mfcharset{

//CString宽字符串转换为多字节
CStringA CSW2CSA(const CStringW &strW)
{
	BOOL bUsed;

	if (strW.IsEmpty())
		return "";

	const wchar_t *wp = (LPCWSTR)strW; 
	int outlen = WideCharToMultiByte(CP_ACP, NULL,  wp, -1, NULL, 0, NULL, &bUsed);

	char *buf = new char[outlen];
	memset(buf, NULL, outlen);
	WideCharToMultiByte(CP_ACP, NULL, wp, strW.GetLength(), buf, outlen, NULL, &bUsed);

	CStringA strA = buf;
	delete [] buf;

	return strA;
}

//CString多字节转换为宽字符串
CStringW CSA2CSW(const CStringA &strA)
{
	if (strA.IsEmpty())
		return L"";

	const char *p = (LPCSTR)strA;
	int outlen = MultiByteToWideChar(CP_ACP, NULL, p, -1, NULL, 0);

	wchar_t *buf = new wchar_t[outlen];
	wmemset(buf, NULL, outlen);

	MultiByteToWideChar(CP_ACP, NULL, p, strA.GetLength(), buf, outlen);
	CStringW strW = buf;
	delete [] buf;

	return strW;
}

CStringW wstringToCStringW(const std::wstring &ws)
{
	CStringW str;

	str = ws.c_str();

	return str;
}

std::wstring CStringWTowstring(const CStringW &strW)
{
	return (LPCWSTR)strW;
}

CStringA stringToCStringA(const std::string &s)
{
	CStringA str;

	str = s.c_str();

	return str;
}

std::string CStringATostring(const CStringA &strA)
{
	return (LPCSTR) strA;
}

CStringA CSW2UTF8(const CStringW& strCsw)
{
	const wchar_t *wbuf = (LPCWSTR) strCsw;

	//UNICODE->UTF8
	int len = WideCharToMultiByte (CP_UTF8, 0, wbuf, -1, NULL, 0, NULL, NULL); 
	char *szUtf8 = new char[len + 1]; 
	memset(szUtf8, 0, len + 1); 
	WideCharToMultiByte (CP_UTF8, 0, wbuf, -1, szUtf8, len, NULL, NULL);   

	CStringA strUTF8 = szUtf8; 
	delete[] szUtf8; 

	return strUTF8;
}

CStringW UTF82CSW(const CStringA& strUtf8)
{
	//UTF8->UNICODE
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, NULL, 0); 
	wchar_t* wsz = new wchar_t[len+1]; 
	memset(wsz, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, wsz, len); 

	CStringW strUni = wsz;
	delete[] wsz;

	return strUni;
}

}; //namespace mfcharsets

//////////////////////////////////////////////////////////////////////////
////图形图像处理、GDI绘制、OpenCV、CxImage...

#define DF_PI	3.1415926
#define DF_ARC	0.01745

namespace mfgraph{

int ColorCmp(Color c1, Color c2)
{
	int rx = abs(c1.GetRed() - c2.GetRed());
	int gx = abs(c1.GetGreen() - c2.GetGreen());
	int bx = abs(c1.GetBlue() - c2.GetBlue());

	if (rx == 0 && gx == 0 && bx == 0)
		return 0;
	else
		return (rx + gx + bx) / 3;
}

BOOL PathIsPic(CString path)
{
	static TCHAR* PicFilter[]={_T(".jpg"),_T(".JPG"),
		_T(".jpeg"),_T(".JPEG"),
		_T(".gif"),_T(".GIF"),
		_T(".bmp"),_T(".BMP"),
		_T(".png"), _T(".PNG"),
		_T(".tif"), _T(".TIF"),
	};

	for (int j = 0; j < sizeof(PicFilter)/sizeof(PicFilter[0]); j++)
		if (path.Find(PicFilter[j]) > 0)
			return TRUE;

	return FALSE;
}

void GetColorBitmap(int w, int h, COLORREF color, CBitmap &bmp)
{
	BYTE r,g,b;
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	BYTE *pDIB = new BYTE[w*h*4];
	for (int i = 0; i < w * h; i++)
	{
		pDIB[i*4] = b;
		pDIB[i*4+1] = g;
		pDIB[i*4+2] = r;
	}

	bmp.CreateBitmap(w,h,1,32,pDIB);
	delete[] pDIB;
}

int GetEncoderClsid(BSTR format, CLSID* pClsid)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		UINT num  = 0;   // number of image encoders 
	UINT size = 0;       // size of the image encoder array in bytes 
	ImageCodecInfo* pImageCodecInfo = NULL; 
	GetImageEncodersSize(&num, &size); 
	if(size == 0)
	{
		return S_FALSE;   // Failure 
	}
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size)); 
	if(pImageCodecInfo == NULL) 
	{
		return S_FALSE;   // Failure 
	}
	GetImageEncoders(num, size, pImageCodecInfo); 
	for(UINT j = 0; j < num; ++j) 
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 ) 
		{ 
			*pClsid = pImageCodecInfo[j].Clsid; 
			free(pImageCodecInfo); 

			return S_OK;   // Success 
		}
	}
	free(pImageCodecInfo);
	return 1;
}

void RotateByPoints(PointF *point, int iAngle, int &dst_w, int &dst_h)
{
	double COS=fabs(cos(iAngle*DF_ARC)), SIN=fabs(sin(iAngle*DF_ARC));
	int src_w=int(point[1].X-point[0].X)   , src_h=int(point[2].Y-point[0].Y);
	dst_w=int(src_w*COS+src_h*SIN);
	dst_h=int(src_w*SIN+src_h*COS);
	Matrix m;
	m.Translate(float(dst_w/2), float(dst_h/2));
	m.Rotate(float(iAngle));
	m.Translate(-float(src_w/2), -float(src_h/2));
	m.TransformPoints(point,3);
}

#ifdef _GDIPLUS_H

void GetBitmapInfoHeader(Bitmap *pbm, BITMAPINFOHEADER &bmih, BitmapData &bmd)
{
	int biBits;

	PixelFormat format = pbm->GetPixelFormat();
	switch(format) {
	case PixelFormat1bppIndexed:
		biBits = 1;
		break;
	case PixelFormat4bppIndexed:
		biBits = 4;
		break;
	case PixelFormat8bppIndexed:
		biBits = 8;
		break;
	case PixelFormat24bppRGB:
		biBits = 24;
		break;
	case PixelFormat32bppRGB:
	case PixelFormat32bppARGB:
		biBits = 32;
		break;
	default:
		biBits = 24;
		break;;
	}

	bmih.biSize               = sizeof(BITMAPINFOHEADER);
	bmih.biWidth              = pbm->GetWidth();
	bmih.biHeight             = pbm->GetHeight();
	bmih.biPlanes             = 1;
	bmih.biBitCount           = biBits;
	bmih.biCompression        = 0;
	bmih.biSizeImage          = 0;
	bmih.biXPelsPerMeter      = 0;
	bmih.biYPelsPerMeter      = 0;
	bmih.biClrUsed            = 0;
	bmih.biClrImportant       = 0;

	//Get data size
	pbm->LockBits(new Rect(0, 0, pbm->GetWidth(), pbm->GetHeight()), 
		ImageLockModeRead, format, &bmd);

	bmih.biSizeImage = bmd.Height * bmd.Stride;
}

//////////////////////////////////////////////////////////////////////////
// 从磁盘选择并加载一副图像
Bitmap *SelectLoadImage()
{
	TCHAR szFilter[] = _T("Image files (*.bmp;*.jpeg;*.jpg;*.png;*.gif;*.tif)|*.bmp;*.jpeg;*.jpg;*.png;*.gif;*.tif|");

	CFileDialog dlg(TRUE, _T(""), NULL, 0, szFilter);
	if (dlg.DoModal() != IDOK) 
		NULL;

#ifdef __CXIMAGE_H
	return LoadBitmapX(dlg.GetPathName());
#else
	return LoadBitmap(dlg.GetPathName());
#endif
}

//////////////////////////////////////////////////////////////////////////
// 加载图像
// @strPath: 待加载图像的路径
//
Bitmap* LoadBitmap(CString strPath)
{
	Bitmap *pbm;

	// 使用GDI+加载图像
#ifdef _UNICODE
	pbm = Bitmap::FromFile(strPath, TRUE);
#else
	pbm = Bitmap::FromFile(CSA2CSW(strPath), TRUE);
#endif

	return pbm;
}

//////////////////////////////////////////////////////////////////////////
// 保存图像
// @pbm: 待保存图像的指针
// @path: 保存到的路径（含扩展名, 支持bmp/jpg/png）

BOOL SaveBitmap(Bitmap *pbm, CStringW path)
{	
	path.MakeLower();

	//使用GDI+保存
	CLSID clsid;
	if (PathMatchSpec(path, _T("*.bmp")))
		GetEncoderClsid(L"image/bmp", &clsid);
	else if (PathMatchSpec(path, _T("*.jpeg")) || PathMatchSpec(path, _T("*.jpg")))
		GetEncoderClsid(L"image/jpeg", &clsid);
	else if (PathMatchSpec(path, _T("*.png")))
		GetEncoderClsid(L"image/png", &clsid);
	else if (PathMatchSpec(path, _T("*.gif")))
		GetEncoderClsid(L"image/gif", &clsid);
	else if (PathMatchSpec(path, _T("*.tiff")))
		GetEncoderClsid(L"image/tiff", &clsid);
	else
		return false;

	return pbm->Save(path, &clsid, NULL) == Ok;
}

BOOL HasAlpha(Bitmap* pbm)
{
	return pbm->GetPixelFormat() == PixelFormat32bppARGB;
}

//////////////////////////////////////////////////////////////////////////
//等比缩放图像以适应指定尺寸
Bitmap* ScaleImageFitRange(Bitmap *pbm, int *w, int *h)
{
	if (!pbm)
		return NULL;

	Bitmap* pbmTemp = pbm->Clone(0,0,pbm->GetWidth(),pbm->GetHeight(),pbm->GetPixelFormat());
	if (!pbmTemp)
		return NULL;

	int iw = pbmTemp->GetWidth();
	int ih = pbmTemp->GetHeight();

	//图像的实际宽高均小于指定区域
	if (iw <= *w && ih <= *h)
	{
		return pbmTemp;
	}

	//约束宽高比例缩放
	double dScaleX = (double)*w/iw;
	double dScaleY = (double)*h/ih;

	double dScale = dScaleX > dScaleY ? dScaleY : dScaleX;

	int iScaleW,iScaleH;
	iScaleW = (int)(iw * dScale);
	iScaleH = (int)(ih * dScale);

	Bitmap *pBmScale = new Bitmap(iScaleW,iScaleH);
	if (!pBmScale)
	{
		delete pbmTemp;
		return NULL;
	}

	Graphics *g = Graphics::FromImage(pBmScale);
	g->DrawImage(pbmTemp,0,0,iScaleW,iScaleH);
	delete g;
	delete pbmTemp;

	*w = iScaleW;
	*h = iScaleH;
	return pBmScale;
}


//////////////////////////////////////////////////////////////////////////
//旋转图像

//返回值：旋转后的图像或NULL(pbmSrc为NULL)
//w, h： 旋转前后图像的宽高
//point: 旋转后原图顶点坐标

Bitmap* RotateBitmap(Bitmap *pbmSrc, int angle, int &w, int &h, PointF *pt)
{
	PointF point[3];
	point[0] = PointF(0, 0);
	point[1] = PointF((float)w, 0);
	point[2] = PointF(0, (float)h);

	int dst_w, dst_h;
	RotateByPoints(point, angle, dst_w, dst_h);

	w = dst_w;
	h = dst_h;

	//返回顶点坐标
	if (pt != NULL)
	{
		pt[0].X = point[0].X; 
		pt[0].Y = point[0].Y;
		pt[1].X = point[1].X;
		pt[1].Y = point[1].Y;
		pt[2].X = point[1].X + point[2].X - point[0].X;
		pt[2].Y = point[2].Y - point[0].Y + point[1].Y;
		pt[3].X = point[2].X;
		pt[3].Y = point[2].Y;
	}

	if (pbmSrc)
	{
		Bitmap* pbm = new Bitmap(dst_w, dst_h);
		Graphics *g = Graphics::FromImage(pbm);
		g->DrawImage(pbmSrc, point, 3);
		return pbm;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//生成一幅任意大小、和原图模式相同的缩略图像，边界用border填充
Bitmap* GetThumb(Bitmap *pbm, int w, int h, Color *border)
{
	if (!pbm) 
		return NULL;

	int w0 = pbm->GetWidth();
	int h0 = pbm->GetHeight();

	Bitmap *pbmThumb = new Bitmap(w, h, pbm->GetPixelFormat());
	Graphics *g = Graphics::FromImage(pbmThumb);

	if (w0 <= w && h0 <= h)
	{
		if (!HasAlpha(pbm))
			g->Clear(Color::White);
		g->DrawImage(pbm, (w-w0)/2, (h-h0)/2, w0, h0);
	}
	else
	{
		//约束宽高比例缩放
		double dScaleX = (double)w/(double)w0;
		double dScaleY = (double)h/(double)h0;
		double dScale = min(dScaleX, dScaleY);

		double ws = (double)w0 * dScale;
		double hs = (double)h0 * dScale;

		Bitmap *pbmScale = (Bitmap*)pbm->GetThumbnailImage((int)ws, (int)hs);
		if (!pbmScale) 
		{
			delete g;
			return NULL;
		}

		//填充边的间隙
		if (border)
		{
			SolidBrush brush(*border);

			if (dScaleX < dScaleY)	//上下的空白
			{
				g->FillRectangle(&brush, 0, 0, w, (h-hs)/2);
				g->FillRectangle(&brush, 0, h/2+hs/2, w, (h-hs)/2+1);
			}
			else					//左右的空白
			{
				g->FillRectangle(&brush, 0,0,(w-ws)/2, h);
				g->FillRectangle(&brush, w/2+ws/2, 0, (w-ws)/2+1, h);
			}
		}

		g->DrawImage(pbmScale, (int)(w-ws)/2-1, (int)(h-hs)/2-1, (int)ws+2, (int)hs+2);
		delete pbmScale;
	}

	delete g;
	return pbmThumb;
}


Bitmap* GetThumbnail(Bitmap *pbm, int w, int h, Color &bg)
{
	Bitmap *pbmThumb = GetThumb(pbm, w, h, NULL);
	SetTransColor(pbmThumb, bg);

	return pbmThumb;
}

//////////////////////////////////////////////////////////////////////////
//同上，除原图透明处将填充为指定颜色
//
Bitmap* GetThumbAlphaColor(Bitmap *pbm, int w, int h, Color alpha_color, Color bg)
{
	if (!HasAlpha(pbm))
		return GetThumbnail(pbm, w, h, bg);

	Bitmap *pbmTemp = CloneBitmap(pbm);
	if (!pbmTemp)
		return NULL;

	SetTransColor(pbmTemp, alpha_color);

	Bitmap *pbmThumb = GetThumbnail(pbmTemp, w, h, bg);

	delete pbmTemp;
	return pbmThumb;
}


//////////////////////////////////////////////////////////////////////////
//为图像增加透明层
//
void RGB2ARGB(Bitmap **pbm)
{
	if (HasAlpha(*pbm))
		return;

	Bitmap *pbmA = new Bitmap((*pbm)->GetWidth(), (*pbm)->GetHeight(), PixelFormat32bppARGB);
	if (!pbmA) return;

	Graphics *g = Graphics::FromImage(pbmA);
	g->DrawImage(*pbm, 0,0,(*pbm)->GetWidth(), (*pbm)->GetHeight());
	delete g;

	delete *pbm;
	*pbm = pbmA;
}

void ARGB2RGB(Bitmap **pbm)
{
	if (!HasAlpha(*pbm))
		return;

	Bitmap *pbmA = new Bitmap((*pbm)->GetWidth(), (*pbm)->GetHeight(), PixelFormat24bppRGB);
	if (!pbmA) return;

	Graphics *g = Graphics::FromImage(pbmA);
	g->DrawImage(*pbm, 0,0,(*pbm)->GetWidth(), (*pbm)->GetHeight());
	delete g;

	delete *pbm;
	*pbm = pbmA;
}

//////////////////////////////////////////////////////////////////////////
//复制图像
//
Bitmap* CloneBitmap(Bitmap *pbm)
{
	if (!pbm)
		return NULL;

	Bitmap *pbm1 = new Bitmap(pbm->GetWidth(), pbm->GetHeight(), pbm->GetPixelFormat());
	if (!pbm1)
		return NULL;

	Graphics *g = Graphics::FromImage(pbm1);
	g->DrawImage(pbm, 0, 0, pbm->GetWidth(), pbm->GetHeight());
	delete g;

	return pbm1;
}

Bitmap* CloneBitmapRGB(Bitmap *pbm)
{
	if (!pbm)
		return NULL;

	Bitmap *pbm1 = new Bitmap(pbm->GetWidth(), pbm->GetHeight(), PixelFormat24bppRGB);
	if (!pbm1)
		return NULL;

	Graphics *g = Graphics::FromImage(pbm1);
	g->DrawImage(pbm, 0, 0, pbm->GetWidth(), pbm->GetHeight());
	delete g;

	return pbm1;
}

Bitmap* CloneBitmapARGB(Bitmap *pbm)
{
	if (!pbm)
		return NULL;

	Bitmap *pbm1 = new Bitmap(pbm->GetWidth(), pbm->GetHeight(), PixelFormat32bppARGB);
	if (!pbm1)
		return NULL;

	Graphics *g = Graphics::FromImage(pbm1);
	g->DrawImage(pbm, 0, 0, pbm->GetWidth(), pbm->GetHeight());
	delete g;

	return pbm1;
}

void CloneAlpha(Bitmap **pbmDst, Bitmap *pbmSrc)
{
	if ((*pbmDst)->GetWidth() != pbmSrc->GetWidth() ||
		(*pbmDst)->GetHeight() != pbmSrc->GetHeight() ||
		!HasAlpha(pbmSrc))
		return;

	RGB2ARGB(pbmDst);

	int w = (*pbmDst)->GetWidth();
	int h = (*pbmDst)->GetHeight();

	BitmapData bmpData1;
	pbmSrc->LockBits(&Rect(0,0,w,h), ImageLockModeRead, PixelFormat32bppARGB, &bmpData1);
	BYTE *pByteS = (BYTE*)bmpData1.Scan0;
	int iOffset = bmpData1.Stride - w * 4;	//0

	BitmapData bmpData;
	(*pbmDst)->LockBits(&Rect(0,0,w,h), ImageLockModeWrite, PixelFormat32bppARGB, &bmpData);
	BYTE *pByteD = (BYTE*)bmpData.Scan0;

	//转换每一行像素值
	int x,y;
	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			pByteD[3] = pByteS[3];

			pByteS += 4;
			pByteD += 4;
		}

		pByteS += iOffset;
		pByteD += iOffset;
	}

	(*pbmDst)->UnlockBits(&bmpData);
	pbmSrc->UnlockBits(&bmpData1);
}

//////////////////////////////////////////////////////////////////////////
//将透明处像素设置为颜色color
//
void SetTransColor(Bitmap *pBitmap, Color color)
{
	if (!pBitmap || !HasAlpha(pBitmap))
		return;

	int w = pBitmap->GetWidth();
	int h = pBitmap->GetHeight();

	BitmapData bmpData;
	Rect rect(0, 0, w, h);
	pBitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppARGB, &bmpData);
	BYTE *pByte = (BYTE*)bmpData.Scan0;
	int iOffset = bmpData.Stride - w * 4;

	//遍历每个像素值
	int x,y;
	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			if (pByte[3] == 0)
			{
				pByte[2] = color.GetRed();
				pByte[1] = color.GetGreen();
				pByte[0] = color.GetBlue();
				pByte[3] = 255;
			}

			pByte += 4;
		}
		pByte += iOffset;
	}

	pBitmap->UnlockBits(&bmpData);
}


//////////////////////////////////////////////////////////////////////////
//设置图像的某一颜色所在像素的透明度
//
void SetColorTrans(Bitmap *pBitmap, Color color, unsigned char level)
{
	if (!pBitmap || !HasAlpha(pBitmap))
		return;

	int w = pBitmap->GetWidth();
	int h = pBitmap->GetHeight();

	BitmapData bmpData;
	Rect rect(0, 0, w, h);
	pBitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppARGB, &bmpData);
	BYTE *pByte = (BYTE*)bmpData.Scan0;
	int iOffset = bmpData.Stride - w * 4;

	//遍历每个像素值
	int x,y;
	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			if (pByte[2] == color.GetRed() && 
				pByte[1] == color.GetGreen() &&
				pByte[0] == color.GetBlue())
			{
				pByte[3] = 0;
			}

			pByte += 4;
		}
		pByte += iOffset;
	}

	pBitmap->UnlockBits(&bmpData);
}

//////////////////////////////////////////////////////////////////////////
//将图像中的颜色from替换为颜色to
//
void SetColorColor(Bitmap *pBitmap, Color from, Color to)
{
	int w = pBitmap->GetWidth();
	int h = pBitmap->GetHeight();

	BitmapData bmpData;
	Rect rect(0, 0, w, h);
	pBitmap->LockBits(&rect, ImageLockModeRead|ImageLockModeWrite, PixelFormat24bppRGB, &bmpData);
	BYTE *pByte = (BYTE*)bmpData.Scan0;
	int iOffset = bmpData.Stride - w * 3;

	int x,y;
	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			if (pByte[2] == from.GetRed() && 
				pByte[1] == from.GetGreen() &&
				pByte[0] == from.GetBlue())
			{
				pByte[2] = to.GetRed();
				pByte[1] = to.GetGreen();
				pByte[0] = to.GetGreen();
			}

			pByte += 3;
		}
		pByte += iOffset;
	}

	pBitmap->UnlockBits(&bmpData);
}

void DrawNumber(Graphics *g, PointF &point, int num, int sz, bool bHori)
{
	CString str;
	str.Format(_T("%d"), num);
	DrawString(g, point, str, sz, bHori);
}

void DrawString(Graphics *g, PointF &point, CString text, int sz, bool bHori)
{
	Gdiplus::FontFamily fontFamily(_T("Arial"));
	Gdiplus::Font font(&fontFamily, sz, FontStyleRegular, UnitPoint);
	SolidBrush brush(Color(50,50,50));

	if (bHori)
	{
		g->DrawString(text, -1, &font, point, &brush);
	}
	else
	{
		StringFormat format;
		format.SetFormatFlags(StringFormatFlagsDirectionVertical);
		g->DrawString(text, -1, &font, point, &format, &brush);
	}
}

#if (defined __CXIMAGE_H) && (defined _GDIPLUS_H)
//////////////////////////////////////////////////////////////////////////
//GDI+ Bitmap 与 CxImage 图像格式转换

BOOL Bitmap2Ximage(Bitmap *pBitmap, CxImage &xImage)
{
	HBITMAP hBitmap;
	pBitmap->GetHBITMAP(NULL, &hBitmap);
	if (!hBitmap) return FALSE;

	xImage.CreateFromHBITMAP(hBitmap);
	if (!xImage.IsValid()) return FALSE;

	//设置ALPHA通道
	if (HasAlpha(pBitmap))
	{
		if (!xImage.AlphaCreate())
			return TRUE;

		BitmapData bmpData;
		Rect rect(0,0,pBitmap->GetWidth(),pBitmap->GetHeight());
		pBitmap->LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);
		int w = pBitmap->GetWidth();
		int h = pBitmap->GetHeight();
		BYTE* dst = (BYTE*)xImage.AlphaGetPointer();
		BYTE* src = (BYTE*)bmpData.Scan0;
		for(long y=0; y<h; y++){
			for(long x=0; x<w; x++){
				dst[(h-y-1)*w+x] = src[4*(y*w+x)+3];
			}
		}
		pBitmap->UnlockBits(&bmpData);
	}

	return TRUE;
}

Bitmap* Ximage2Bitmap(CxImage *pXimage)
{
	if (!pXimage)
		return NULL;

	BYTE *pDIB = (BYTE*)pXimage->GetDIB();
	BITMAPINFOHEADER *pBmih = (BITMAPINFOHEADER*)pDIB;	
	int w = pBmih->biWidth;
	int h = pBmih->biHeight;

	Bitmap *pBitmap = new Bitmap(w, h, PixelFormat24bppRGB);
	if (!pBitmap)
		return NULL;

	//复制RGB像素数据
	BitmapData bmpData;
	pBitmap->LockBits(&Rect(0,0,w,h), ImageLockModeWrite, PixelFormat24bppRGB, &bmpData);

	if (pBmih->biSizeImage == bmpData.Stride * bmpData.Height)	//likely
		memcpy(bmpData.Scan0, pDIB + sizeof(BITMAPINFOHEADER), pBmih->biSizeImage);
	pBitmap->UnlockBits(&bmpData);

	//上下颠倒问题
	pBitmap->RotateFlip(RotateNoneFlipY);

	//设置ALPHA通道
	BYTE* src = (BYTE*)pXimage->AlphaGetPointer();
	if (src)
	{
		RGB2ARGB(&pBitmap);

		BitmapData bmpDataA;
		pBitmap->LockBits(&Rect(0,0,w,h), ImageLockModeWrite, PixelFormat32bppARGB, &bmpDataA);

		BYTE* dst = (BYTE*)bmpDataA.Scan0;
		for(long y=0; y<h; y++){
			for(long x=0; x<w; x++){
				dst[4*(y*w+x)+3] = src[(h-y-1)*w+x];
			}
		}

		pBitmap->UnlockBits(&bmpDataA);
	}

	return pBitmap;
}

BOOL SaveBitmapX(Bitmap *pbm, CString path)
{
	CxImage xImage;
	Bitmap2Ximage(pbm, xImage);
	if (!xImage.IsValid())
		return false;

	path.MakeLower();
	if (PathMatchSpec(path, _T("*.bmp")))
		return xImage.Save(path, CXIMAGE_FORMAT_BMP);	
	else if (PathMatchSpec(path, _T("*.jpeg")) || PathMatchSpec(path, _T("*.jpg")))
		return xImage.Save(path, CXIMAGE_FORMAT_JPG);	
	else if (PathMatchSpec(path, _T("*.png")))
		return xImage.Save(path, CXIMAGE_FORMAT_PNG);
	else if (PathMatchSpec(path, _T("*.tiff")))
		return xImage.Save(path, CXIMAGE_FORMAT_TIF);	
	else if (PathMatchSpec(path, _T("*.gif")))
		return xImage.Save(path, CXIMAGE_FORMAT_GIF);

	return false;
}

Bitmap*	LoadBitmapX(CString path)
{
	// 由CxImage加载的图像可以保留透明层
	// 此图像必须也为CxImage所保存	
	Bitmap *pbm = NULL;
	CxImage xImage;

	xImage.Load(path);
	if (!xImage.IsValid())
		return NULL;

	pbm = Ximage2Bitmap(&xImage);

	xImage.Destroy();	//redundunce
	return pbm;
}

#endif	// (defined __CXIMAGE_H) && (defined _GDIPLUS_H)

#if (defined OPENCV) && (defined _GDIPLUS_H)
//////////////////////////////////////////////////////////////////////////
//OpenCV图像格式与GDI+ Bitmap格式转换

IplImage* Bitmap2IplImage(Bitmap* pBitmap)
{
	if (!pBitmap)
		return NULL;

	int w = pBitmap->GetWidth();
	int h = pBitmap->GetHeight();

	BitmapData bmpData;
	Rect rect(0,0,w,h);
	pBitmap->LockBits(&rect, ImageLockModeRead, PixelFormat24bppRGB, &bmpData);
	BYTE* temp = (bmpData.Stride>0) ? ((BYTE*)bmpData.Scan0) : ((BYTE*)bmpData.Scan0 + bmpData.Stride*(h-1));

	IplImage* pIplImg = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 3);
	if (!pIplImg)
	{
		pBitmap->UnlockBits(&bmpData);
		return NULL;
	}

	memcpy(pIplImg->imageData, temp, abs(bmpData.Stride)*bmpData.Height);
	pBitmap->UnlockBits(&bmpData);

	//判断Top-Down or Bottom-Up
	if (bmpData.Stride<0)       
		cvFlip(pIplImg, pIplImg);

	return pIplImg;
}

// pBitmap 同样需要外部释放
Bitmap* IplImage2Bitmap(IplImage* pIplImg)
{
	if(!pIplImg)
		return NULL;

	Bitmap *pBitmap = new Bitmap(pIplImg->width, pIplImg->height, PixelFormat24bppRGB);
	if (!pBitmap)
		return NULL;

	BitmapData bmpData;
	Rect rect(0, 0, pIplImg->width, pIplImg->height);
	pBitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat24bppRGB, &bmpData);
	BYTE *pByte = (BYTE*)bmpData.Scan0;

	if (pIplImg->widthStep == bmpData.Stride)	//likely
		memcpy(bmpData.Scan0, pIplImg->imageDataOrigin, pIplImg->imageSize);

	pBitmap->UnlockBits(&bmpData);
	return pBitmap;
}

//////////////////////////////////////////////////////////////////////////
//漫水填充算法——设置像素颜色
//
Bitmap* FloodFill(Bitmap*pBitmap, unsigned x, unsigned y, Color newclr)
{
	if (!pBitmap || 
		x > pBitmap->GetWidth()-1 ||
		y > pBitmap->GetHeight()-1
		)
		return NULL;

	IplImage* pIplImg = Bitmap2IplImage(pBitmap);
	if (!pIplImg)
		return NULL;

	CvPoint seed = cvPoint(x,y);
	CvScalar color = CV_RGB(newclr.GetRed(), newclr.GetGreen(), newclr.GetBlue());
	cvFloodFill(pIplImg, seed, color);

	Bitmap *pbmNew = IplImage2Bitmap(pIplImg);
	RGB2ARGB(&pbmNew);

	cvReleaseImage(&pIplImg);
	return pbmNew;
}

Bitmap* FloodFillCorner(Bitmap *pBitmap, Color newclr)
{
	if (!pBitmap)
		return NULL;

	IplImage* pIplImg = Bitmap2IplImage(pBitmap);
	if (!pIplImg)
		return NULL;

	int x = pBitmap->GetWidth();
	int y = pBitmap->GetHeight();

	Color rare(191,219,255);		//FIXME
	Color white(255,255,255);
	Color clrLT,clrLB,clrRT,clrRB, clrNow;

	CvScalar scalarRare = CV_RGB(rare.GetRed(), 
		rare.GetGreen(), rare.GetBlue());

	pBitmap->GetPixel(0, 0, &clrLT);
	pBitmap->GetPixel(0, y-1, &clrLB);
	pBitmap->GetPixel(x-1, 0, &clrRT);
	pBitmap->GetPixel(x-1, y-1, &clrRB);

	if (!ColorCmp(clrLT, white))
	{
		CvPoint seed = cvPoint(0,0);
		cvFloodFill(pIplImg, seed, scalarRare);
	}

	cvGetPixel(pIplImg, x-1, 0, clrNow);
	if (!ColorCmp(clrRT, white) && ColorCmp(clrNow, rare))	//在从左上角漫水时如果右上角漫到了，原来的白色就变成了rare色，否则保持为白色需要再次执行漫水算法
	{
		CvPoint seed = cvPoint(x-1,0);
		cvFloodFill(pIplImg, seed, scalarRare);
	}

	cvGetPixel(pIplImg, 0, y-1, clrNow);
	if (!ColorCmp(clrLB, white) && ColorCmp(clrNow, rare))
	{
		CvPoint seed = cvPoint(0,y-1);
		cvFloodFill(pIplImg, seed, scalarRare);
	}

	cvGetPixel(pIplImg, x-1, y-1, clrNow);
	if (!ColorCmp(clrRB, white) && ColorCmp(clrNow, rare))
	{
		CvPoint seed = cvPoint(x-1,y-1);
		cvFloodFill(pIplImg, seed, scalarRare);
	}

	Bitmap *pbmNew = IplImage2Bitmap(pIplImg);

	cvReleaseImage(&pIplImg);
	return pbmNew;
}

//将图像周边的白色置为透明
Bitmap* GetBitmapBorderTrans(Bitmap *pbm)
{
	if (!pbm)
		return NULL;

	int w = pbm->GetWidth();
	int h = pbm->GetHeight();

	//漫水填充周边为某不用颜色
	Color rare(191,219,255);
	Bitmap *pbmRet = FloodFillCorner(pbm, rare);
	if (!pbmRet)
		return NULL;

	//还原透明层
	CloneAlpha(&pbmRet, pbm);

	BitmapData bmpData;
	Rect rect(0, 0, w, h);
	pbmRet->LockBits(&rect, ImageLockModeRead|ImageLockModeWrite, PixelFormat32bppARGB, &bmpData);
	BYTE *pByte = (BYTE*)bmpData.Scan0;
	int iOffset = bmpData.Stride - w * 4;

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			if (pByte[2] == rare.GetRed() && pByte[1] == rare.GetGreen() && pByte[0] == rare.GetBlue())
			{
				pByte[3] = 0;
			}

			pByte += 4;
		}

		pByte += iOffset;
	}

	pbmRet->UnlockBits(&bmpData);
	return pbmRet;
}

#endif //#if (defined OPENCV) && (defined _GDIPLUS_H)

#endif // _GDIPLUS_H

#ifdef OPENCV
void cvGetPixel(IplImage* pIplImg, int i, int j, Color &color)
{
	if (i >= pIplImg->width || i < 0 || 
		j >= pIplImg->height || j < 0 )
		return;

	//BGR,BGR,BGR...
	uchar *p = ((uchar*)(pIplImg->imageData + pIplImg->widthStep * j + i * 3));
	color = Color(p[2], p[1], p[0]);
}

void cvSetPixel(IplImage* pIplImg, int i, int j, Color color)
{
	if (i >= pIplImg->width || i < 0 || 
		j >= pIplImg->height || j < 0 )
		return;

	//BGR,BGR,BGR...
	uchar *p = ((uchar*)(pIplImg->imageData + pIplImg->widthStep * j + i * 3));
	p[2] = color.GetRed();
	p[1] = color.GetGreen();
	p[0] = color.GetBlue();
}

#endif	//OPENCV

};//namespace mfgraph

}; // namespace mfcutil