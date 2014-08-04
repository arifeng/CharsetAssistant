// ColorDef.h: interface and implementation of the CColorDef class.
//
/////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORDEF_H__2A4E63F6_A106_4295_BCBA_06D03CD67AE7__INCLUDED_)
#define AFX_COLORDEF_H__2A4E63F6_A106_4295_BCBA_06D03CD67AE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////////////////////////////////////////
// helper struct. equates to COLORREF

#pragma pack(push)
#pragma pack(1)

struct HLSX
{
	HLSX() { fHue = fLuminosity = fSaturation = 0.0f; }

	float fHue; // (0-360)
	float fLuminosity; // (0-1)
	float fSaturation; // (0-1)
};

class RGBX
{
public:
	RGBX() { btRed = btBlue = btGreen = btUnused = 0; }
	RGBX(BYTE red, BYTE green, BYTE blue) { btRed = red; btBlue = blue; btGreen = green; btUnused = 0; }
	RGBX(const RGBX& color) { *this = color; }
	RGBX(const COLORREF& color) { btRed = GetRValue(color); btBlue = GetBValue(color); btGreen = GetGValue(color); btUnused = 0; }

	inline const RGBX& operator=(const RGBX& rgb) { btRed = rgb.btRed; btBlue = rgb.btBlue; btGreen = rgb.btGreen; return *this; }
	inline BOOL operator==(const RGBX& rgb) const { return (btRed == rgb.btRed && btGreen == rgb.btGreen && btBlue == rgb.btBlue); }
	inline BOOL IsGray() const { return (btRed == btGreen && btGreen == btBlue); }
	inline operator COLORREF() const { return RGB(btRed, btGreen, btBlue); }

	inline int Luminance() const { return (((int)btBlue + (int)btGreen * 6 + (int)btRed * 3) / 10); }
	inline void MakeGray() { btRed = btGreen = btBlue = Luminance(); }
	inline void MakeGray(const RGBX& rgbSrc) { btRed = btGreen = btBlue = rgbSrc.Luminance(); }

	void MakeGray(const RGBX& rgbSrc, double dRedFactor, double dGreenFactor, double dBlueFactor) 
	{ 
		*this = rgbSrc;
		MakeGray(dRedFactor, dGreenFactor, dBlueFactor);
	}

	void MakeGray(double dRedFactor, double dGreenFactor, double dBlueFactor) 
	{ 
		btRed = btGreen = btBlue = (int)(btBlue * dBlueFactor) +
									(int)(btGreen * dGreenFactor) + 
									(int)(btRed * dRedFactor); 
	}

	void AdjustLighting(int nIncrement)
	{
		btRed = (BYTE)max(0, min(255, btRed + nIncrement));
		btGreen = (BYTE)max(0, min(255, btGreen + nIncrement));
		btBlue = (BYTE)max(0, min(255, btBlue + nIncrement));
	}

	void AdjustLighting(const RGBX& rgbSrc, int nIncrement)
	{
		btRed = (BYTE)max(0, min(255, rgbSrc.btRed + nIncrement));
		btGreen = (BYTE)max(0, min(255, rgbSrc.btGreen + nIncrement));
		btBlue = (BYTE)max(0, min(255, rgbSrc.btBlue + nIncrement));
	}

	static void HLS2RGB(const HLSX& hls, RGBX& rgb)
	{
		if (!hls.fSaturation)
			rgb.btRed = rgb.btBlue = rgb.btGreen = (BYTE)(min(255.0f, hls.fLuminosity * 255));

		else
		{
			float fT1, fT2;

			if (hls.fLuminosity < 0.5f)
				fT2 = hls.fLuminosity * (1.0f + hls.fSaturation);
			else
				fT2= (hls.fLuminosity + hls.fSaturation) - (hls.fLuminosity * hls.fSaturation);

			fT1 = 2 * hls.fLuminosity - fT2;

			rgb.btRed = Hue2Triplet(fT1, fT2, hls.fHue + 120.0f);
			rgb.btGreen = Hue2Triplet(fT1, fT2, hls.fHue);
			rgb.btBlue = Hue2Triplet(fT1, fT2, hls.fHue - 120.0f);
		}
	}

	static void RGB2HLS(const RGBX& rgb, HLSX& hls)
	{
		int nMax = max(rgb.btRed, max(rgb.btGreen, rgb.btBlue));
		int nMin = min(rgb.btRed, min(rgb.btGreen, rgb.btBlue));

		float fDiff = (float)(nMax - nMin);
		float fSum = (float)(nMax + nMin);
			  
		// calculate luminosity
		hls.fLuminosity = fSum / 510.0f;

		if (nMax == nMin) // gray
		{
			hls.fHue = hls.fSaturation = 0.0f;
		}
		else
		{
			// calculate Saturation
			if (hls.fLuminosity < 0.5f)
				hls.fSaturation = fDiff / fSum;
			else
				hls.fSaturation = fDiff / (510.0f - fSum);
			
			// calculate Hue
			float fRNorm = (nMax - rgb.btRed) / fDiff;	  
			float fGNorm = (nMax - rgb.btGreen) / fDiff;
			float fBNorm = (nMax - rgb.btBlue) / fDiff;   
			
			if (rgb.btRed == nMax)
				hls.fHue = 60.0f * (6.0f + fBNorm - fGNorm);
			
			else if (rgb.btGreen == nMax)
				hls.fHue = 60.0f * (2.0f + fRNorm - fBNorm);
			
			else
				hls.fHue = 60.0f * (4.0f + fGNorm - fRNorm);
			
			if (hls.fHue > 360.0f)
				hls.fHue -= 360.0f;
		}
	}

public:
	BYTE btBlue;
	BYTE btGreen;
	BYTE btRed;

protected:
	BYTE btUnused;

protected:
	static BYTE Hue2Triplet(float fT1, float fT2, float fHue) // private helper for RGBX::HLS2RGB
	{
		if (fHue < 0)
			fHue += 360.0f;

		else if (fHue > 360.0f)
			fHue -= 360.0f;

		if (fHue < 60.0f) 
			fT1 = fT1 + (fT2 - fT1) * fHue / 60.0f; 
		
		else if (fHue < 180.0f) 
			fT1 = fT2;

		else if (fHue < 240.0f) 
			fT1 = fT1 + (fT2 - fT1) * (240.0f - fHue) / 60.0f;  
		
		return (BYTE)min(fT1 * 255.0f, 255.0f);
	}
};

#pragma pack(pop)

#endif // !defined(AFX_COLORDEF_H__2A4E63F6_A106_4295_BCBA_06D03CD67AE7__INCLUDED_)

///////////////////////////////////////////////////////////////////////

