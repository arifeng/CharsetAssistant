/**
*
* @file: MfcUtil.h
* @desc: MFC相关实用函数
* 
* @auth: liwei (www.leewei.org)
* @mail: ari.feng@qq.com
* @date: 2012/04/10
* @mdfy: 2012/10/24
* @Ver: 3.8
*
*/

/* 主要包括：
 *
 * 1、字符串、编码转换
 * 2、图形图像绘制(GDI+,OpenCV..)
 * 3、其他
 */

#ifndef __MFCUTIL_H
#define __MFCUTIL_H

#include <string>

#if _MSC_VER <= 1200  //VC6
#include <ATLBASE.H>
#define atlTraceUI 0
typedef ULONG ULONG_PTR;
typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;
#include "StringA.h"
#include "StringW.h"
#endif

//GDI+
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

//CxImage
//#include "../CxImage/ximage.h"
//#pragma comment(lib, "../CxImage/cximageu.lib")

namespace mfcutil{

CString			GetModulePath();												//获取可执行文件所在目录
void			GetModulePath(CString &rString);

CString			SelectFile(CString desc, CString filter);						//选择一个文件
CString			SelectDirectory(CString title);									//选择一个目录
void			ShellExec(const CString &strFile, const CString &strParam,		//执行系统命令或打开文件
							DWORD dwWait = 0, int nShow = SW_SHOW);

BOOL			Shutdown();														//关机

//////////////////////////////////////////////////////////////////////////
//字符串、编码转换

namespace mfcharset{

//CStringW <==> CStringA
CStringW		CSA2CSW(const CStringA &strA);
CStringA		CSW2CSA(const CStringW &strW);

//CStringW <==> wstring
CStringW		wstringToCStringW(const std::wstring &ws);
std::wstring	CStringWTowstring(const CStringW &strW);

//CStringA <==> string
CStringA		stringToCStringA(const std::string &s);
std::string		CStringATostring(const CStringA &strA);	

#ifdef UNICODE
  #define CStringTowstring(x)	CStringWTowstring((CStringW)x)
  #define wstringToCString(x)	wstringToCStringW(x)
  #define CStringTostring(x)	CStringATostring(CSW2CSA((CStringW)x))
  #define stringToCString(x)	CSA2CSW(stringToCStringA(x))
#else //MBCS
  #define CStringTostring(x)	CStringATostring((CStringA)x)
  #define stringToCString(x)	stringToCStringA(x)
  #define CStringTowstring(x)   CStringWTowstring(CSA2CSW((CStringA)x))
  #define wstringToCString(x)   CSW2CSA(wstringToCStringW(x))
#endif

//CStringW <==> UTF-8
CStringA		CSW2UTF8(const CStringW& strCsw);
CStringW		UTF82CSW(const CStringA& strUtf8);

//CStringA <==> UTF-8
#define CSA2UTF8(x) CSW2UTF8(CSA2CSW(x))
#define UTF82CSA(x) CSW2CSA(UTF82CSW(x))

//CString <==> UTF-8 string
#ifdef UNICODE
  #define CStringToUTF8string(x) CStringATostring(CSW2UTF8((CStringW)x))
  #define UTF8stringToCString(x) UTF82CSW(stringToCStringA(x))
#else
  #define CStringToUTF8string(x) CStringATostring(CSA2UTF8((CStringA)x))
  #define UTF8stringToCString(x) UTF82CSA(stringToCStringA(x))
#endif

}; //namespace charset

//////////////////////////////////////////////////////////////////////////
//图形图像处理、GDI绘制、OpenCV、CxImage...

//UImgLib
namespace mfgraph{

 int		ColorCmp(Color c1, Color c2);													//比较两个颜色是否相同
 BOOL		PathIsPic(CString path);														//指定路径是否为图片文件

 void		RotateByPoints (PointF *point, int iAngle, int &dst_w, int &dst_h);				//矩阵旋转
 int		GetEncoderClsid (BSTR format, CLSID* pClsid);

 void		GetColorBitmap(int w, int h, COLORREF color, CBitmap &bmp);

#ifdef _GDIPLUS_H
 void		GetBitmapInfoHeader(Bitmap *pbm, BITMAPINFOHEADER &bmih, BitmapData &bmd);

 Bitmap*	SelectLoadImage();																//从磁盘选择并加载一副图像

 BOOL		SaveBitmap(Bitmap *pbm, CString path);											//保存位图为指定格式文件
 Bitmap*	LoadBitmap(CString path);														//由路径加载图像

 Bitmap*	CloneBitmap(Bitmap *pbm);														//完整复制一副图像
 Bitmap*	CloneBitmapRGB(Bitmap *pbm);													//复制一副图片(不包含透明层的)
 Bitmap*	CloneBitmapARGB(Bitmap *pbm);													//复制一副图片(包含透明层的)

 void		CloneAlpha(Bitmap **pbmDst, Bitmap *pbmSrc);									//复制Src的透明设置到Dst

 void		SetColorTrans(Bitmap *pbm, Color color, unsigned char level);					//为指定颜色的像素设置透明度
 void		SetTransColor(Bitmap *pbm, Color color);										//将透明处像素设置为颜色color
 void		SetColorColor(Bitmap *pbm, Color from, Color to);								//将图像中的颜色from替换为颜色to
 BOOL		HasAlpha(Bitmap* pbm);															//图像是否有透明层

 void		RGB2ARGB(Bitmap **pbm);															//由RGB模式转换为ARGB模式
 void		ARGB2RGB(Bitmap **pbm);

 Bitmap*	GetThumb(Bitmap *pbm, int w, int h, Color *border = NULL);						//获取等比缩放后的缩略图像(图像保持透明)
 Bitmap*	GetThumbnail(Bitmap *pbm, int w, int h, Color &bg = Color(255,255,255));		//获取等比缩放后的缩略图像(透明填充)
 Bitmap*	GetThumbAlphaColor(Bitmap *pbm, int w, int h, Color alpha_color, Color bg);		//同上，但原图透明处将填充为指定颜色

 Bitmap*	RotateBitmap(Bitmap *pbm, int angle, int &w, int &h, PointF *ptf = NULL);		//旋转图像
 Bitmap*	ScaleImageFitRange(Bitmap *pbm, int *w, int *h);								//等比缩放图像以适应指定尺寸

 void		DrawNumber(Graphics *g, PointF &point, int num, int sz, bool bHori = true);
 void		DrawString(Graphics *g, PointF &point, CString text, int sz, bool bHori = true);

#if (defined __CXIMAGE_H) && (defined _GDIPLUS_H)
 BOOL		SaveBitmapX(Bitmap *pbm, CString path);											//可保存bmp透明图像
 Bitmap* 	LoadBitmapX(CString path);														//由CxImage加载图像

 Bitmap*	Ximage2Bitmap (CxImage *pXimage);												//CxImage格式转换为Bitmap格式
 BOOL		Bitmap2Ximage (Bitmap *pBitmap, CxImage &xImage);								//Bitmap格式转换为CxImage格式
#endif

#ifdef OPENCV
 Bitmap*	FloodFill (Bitmap* pBitmap, unsigned x, unsigned y, Color newclr);				//漫水填充算法—从指定点开始扩散
 Bitmap*	FloodFillCorner (Bitmap* pBitmap, Color clrnew);								//漫水填充算法—从白色的顶点扩散
 Bitmap*	GetBitmapBorderTrans(Bitmap *pbm);												//将图像周边的白色置为透明

 IplImage*   Bitmap2IplImage (Bitmap* pBitmap);
 Bitmap*	 IplImage2Bitmap (IplImage* pIplImg);
#endif //ifdef OPENCV

#endif //ifdef GDIPLUS

#ifdef OPENCV
 void		cvGetPixel(IplImage* pIplImg, int i, int j, Color &color);						//获取某个像素的颜色值
 void		cvSetPixel(IplImage* pIplImg, int i, int j, Color color);						//设置某个像素的颜色值
#endif //ifdef OPENCV

}; //namespace mfgraph

}; //namespace mfcutil;

#endif