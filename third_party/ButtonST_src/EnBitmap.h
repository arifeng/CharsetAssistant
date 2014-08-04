// EnBitmap.h: interface for the CEnBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENBITMAP_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_)
#define AFX_ENBITMAP_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "colordef.h"
#include <afxtempl.h>
#include <afxdisp.h> 

/////////////////////////////////////////////////////////////////////////////////////
// base class for image processing

class C32BitImageProcessor
{
public:
	C32BitImageProcessor(BOOL bEnableWeighting = FALSE);
	virtual ~C32BitImageProcessor();

	virtual CSize CalcDestSize(CSize sizeSrc);
	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);

	inline void CalcWeightedColor(RGBX* pPixels, CSize size, double dX, double dY, RGBX& rgbResult);

protected:
	BOOL m_bWeightingEnabled;
};

typedef CArray<C32BitImageProcessor*, C32BitImageProcessor*> C32BIPArray;

//////////////////////////////////////////////////////////////////////////////////////////

void DrawTransparent(CDC *pDC ,CBitmap *pSrc, LPRECT lpRect, COLORREF crMask );

class CEnBitmap : public CBitmap  
{
public:
	CEnBitmap(COLORREF crBkgnd = RGB(255, 255, 255));
	virtual ~CEnBitmap();

	BOOL LoadImage(LPCTSTR szImagePath, COLORREF crBack = -1);
	BOOL LoadImage(UINT uIDRes, LPCTSTR szResourceType, HMODULE hInst = NULL, COLORREF crBack = -1); 
	BOOL CopyImage(HBITMAP hBitmap);
	BOOL CopyImage(CBitmap* pBitmap);

	BOOL ProcessImage(C32BitImageProcessor* pProcessor, COLORREF crMask = -1);
	BOOL ProcessImage(C32BIPArray& aProcessors, COLORREF crMask = -1); // ordered list of processors

	// helpers
	static HBITMAP LoadImageFile(LPCTSTR szImagePath, COLORREF crBack = -1);
	static HBITMAP LoadImageResource(UINT uIDRes, LPCTSTR szResourceType, HMODULE hInst = NULL, COLORREF crBack = 0); 
	static BOOL GetResource(LPCTSTR lpName, LPCTSTR lpType, HMODULE hInst, void* pResource, int& nBufSize);
	static IPicture* LoadFromBuffer(BYTE* pBuff, int nSize);

protected:
	COLORREF m_crBkgnd;

protected:
	RGBX* GetDIBits32();
	BOOL PrepareBitmapInfo32(BITMAPINFO& bi, HBITMAP hBitmap = NULL);

	static HBITMAP ExtractBitmap(IPicture* pPicture, COLORREF crBack);
	static int GetFileType(LPCTSTR szImagePath);
	static BOOL Fill(RGBX* pPixels, CSize size, COLORREF color);
};

// inline weighting function
inline void C32BitImageProcessor::CalcWeightedColor(RGBX* pPixels, CSize size, double dX, double dY, RGBX& rgbResult)
{
	ASSERT (m_bWeightingEnabled);

	// interpolate between the current pixel and its adjacent pixels to the right and down
	int nX = (int)dX;
	int nY = (int)dY;

	if (dX < 0 || dY < 0)
	{
		rgbResult = pPixels[max(0, nY) * size.cx + max(0, nX)]; // closest
		return;
	}

	double dXFraction = dX - nX;
	double dX1MinusFraction = 1 - dXFraction;
	
	double dYFraction = dY - nY;
	double dY1MinusFraction = 1 - dYFraction;

	int nXP1 = min(nX + 1, size.cx - 1);
	int nYP1 = min(nY + 1, size.cy - 1);
	
	RGBX* pRGB = &pPixels[nY * size.cx + nX]; // x, y
	RGBX* pRGBXP = &pPixels[nY * size.cx + nXP1]; // x + 1, y
	RGBX* pRGBYP = &pPixels[nYP1 * size.cx + nX]; // x, y + 1
	RGBX* pRGBXYP = &pPixels[nYP1 * size.cx + nXP1]; // x + 1, y + 1
	
	int nRed = (int)(dX1MinusFraction * dY1MinusFraction * pRGB->btRed +
					dXFraction * dY1MinusFraction * pRGBXP->btRed +
					dX1MinusFraction * dYFraction * pRGBYP->btRed +
					dXFraction * dYFraction * pRGBXYP->btRed);
	
	int nGreen = (int)(dX1MinusFraction * dY1MinusFraction * pRGB->btGreen +
					dXFraction * dY1MinusFraction * pRGBXP->btGreen +
					dX1MinusFraction * dYFraction * pRGBYP->btGreen +
					dXFraction * dYFraction * pRGBXYP->btGreen);
	
	int nBlue = (int)(dX1MinusFraction * dY1MinusFraction * pRGB->btBlue +
					dXFraction * dY1MinusFraction * pRGBXP->btBlue +
					dX1MinusFraction * dYFraction * pRGBYP->btBlue +
					dXFraction * dYFraction * pRGBXYP->btBlue);

	rgbResult.btRed = max(0, min(255, nRed));
	rgbResult.btGreen = max(0, min(255, nGreen));
	rgbResult.btBlue = max(0, min(255, nBlue));
}

#endif // !defined(AFX_ENBITMAP_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_)
