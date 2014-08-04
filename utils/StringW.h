//////////////////////////////////////////////////////////////////////
//
// Copyright 2001 SilentBox Software
// All rights reserved.
//
// Creation: 04.05.2002
// Author: Oskar Wieland
//
// Purpose: Unicode CString class
//
// Modifications:
//
// 04.05.2002 - File created
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// include guards
//////////////////////////////////////////////////////////////////////

#ifndef WTL_CSTRING_W_H__INCLUDED
#define WTL_CSTRING_W_H__INCLUDED

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//////////////////////////////////////////////////////////////////////
// additional defines
//////////////////////////////////////////////////////////////////////

struct CStringDataW
{
    long nRefs;     // reference count
    int nDataLength;
    int nAllocLength;
    // WCHAR data[nAllocLength]

    WCHAR* data()
        { return (WCHAR*)(this + 1); }
};


//////////////////////////////////////////////////////////////////////
// global data
//////////////////////////////////////////////////////////////////////

// For an empty string, m_pchData will point here
// (note: avoids special case of checking for NULL m_pchData)
// empty string data (and locked)
_declspec(selectany) int rgInitDataW[] = { -1, 0, 0, 0 };
_declspec(selectany) CStringDataW* _atltmpDataNilW = (CStringDataW*)&rgInitDataW;
_declspec(selectany) LPCWSTR _atltmpPchNilW = (LPCWSTR)(((BYTE*)&rgInitDataW) + sizeof(CStringDataW));


//////////////////////////////////////////////////////////////////////
// CStringW class
//////////////////////////////////////////////////////////////////////

class CStringW
{
public:
// Constructors
    CStringW() 
        { Init(); }

    CStringW(const CStringW& stringSrc)
    {
        ATLASSERT(stringSrc.GetData()->nRefs != 0);
        if (stringSrc.GetData()->nRefs >= 0)
        {
            ATLASSERT(stringSrc.GetData() != _atltmpDataNilW);
            m_pchData = stringSrc.m_pchData;
            InterlockedIncrement(&GetData()->nRefs);
        }
        else
        {
            Init();
            *this = stringSrc.m_pchData;
        }
    }

    CStringW(WCHAR ch, int nRepeat = 1)
    {
        Init();
        if (nRepeat >= 1)
        {
            if(AllocBuffer(nRepeat))
            {
                for (int i = 0; i < nRepeat; i++)
                    m_pchData[i] = ch;
            }
        }
    }

    CStringW(LPCSTR lpsz)
    {
        Init();
        if (lpsz != NULL && HIWORD(lpsz) == NULL)
        {
            UINT nID = LOWORD((DWORD_PTR)lpsz);
            if (!LoadString(nID))
                ATLTRACE2(atlTraceUI, 0, _T("Warning: implicit LoadString(%u) in CString failed\n"), nID);
        }
        else
        {
            int nSrcLen = (lpsz != NULL) ? lstrlenA(lpsz) : 0;
            if (nSrcLen != 0)
            {
                if(AllocBuffer(nSrcLen))
                {
                    _mbstowcsz(m_pchData, lpsz, nSrcLen + 1);
                    ReleaseBuffer();
                }
            }
        }
    }

    CStringW(LPCSTR ch, int nLength)
    {
        Init();
        if (nLength != 0)
        {
            if(AllocBuffer(nLength))
            {
                _mbstowcsz(m_pchData, ch, nLength);
                ReleaseBuffer();
            }
        }
    }

    CStringW(LPCWSTR lpsz)
    {
        Init();
        int nSrcLen = (lpsz != NULL) ? lstrlenW(lpsz) : 0;
        if (nSrcLen != 0)
        {
            if(AllocBuffer(nSrcLen))
                memcpy(m_pchData, lpsz, nSrcLen * sizeof(WCHAR));
        }
    }

    CStringW(LPCWSTR lpch, int nLength)
    {
        Init();
        if (nLength != 0)
        {
            if(AllocBuffer(nLength))
                memcpy(m_pchData, lpch, nLength * sizeof(WCHAR));
        }
    }

    CStringW(const unsigned char* psz) 
        { Init(); *this = (LPCSTR)psz; }

// Attributes & Operations
    // as an array of characters
    int GetLength() const 
        { return GetData()->nDataLength; }

    BOOL IsEmpty() const
        { return GetData()->nDataLength == 0; }

    void Empty()                       // free up the data
    {
        if (GetData()->nDataLength == 0)
            return;

        if (GetData()->nRefs >= 0)
            Release();
        else
            *this = L"";

        ATLASSERT(GetData()->nDataLength == 0);
        ATLASSERT(GetData()->nRefs < 0 || GetData()->nAllocLength == 0);
    }

    WCHAR GetAt(int nIndex) const       // 0 based
    {
        ATLASSERT(nIndex >= 0);
        ATLASSERT(nIndex < GetData()->nDataLength);
        return m_pchData[nIndex];
    }

    WCHAR operator[](int nIndex) const  // same as GetAt
    {
        // same as GetAt
        ATLASSERT(nIndex >= 0);
        ATLASSERT(nIndex < GetData()->nDataLength);
        return m_pchData[nIndex];
    }

    void SetAt(int nIndex, WCHAR ch)
    {
        ATLASSERT(nIndex >= 0);
        ATLASSERT(nIndex < GetData()->nDataLength);

        CopyBeforeWrite();
        m_pchData[nIndex] = ch;
    }
    
    operator LPCWSTR() const           // as a C string
        { return m_pchData; }

    // overloaded assignment
    const CStringW& operator=(const CStringW& stringSrc)
    {
        if (m_pchData != stringSrc.m_pchData)
        {
            if ((GetData()->nRefs < 0 && GetData() != _atltmpDataNilW) || stringSrc.GetData()->nRefs < 0)
            {
                // actual copy necessary since one of the strings is locked
                AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
            }
            else
            {
                // can just copy references around
                Release();
                ATLASSERT(stringSrc.GetData() != _atltmpDataNilW);
                m_pchData = stringSrc.m_pchData;
                InterlockedIncrement(&GetData()->nRefs);
            }
        }
        return *this;
    }

    const CStringW& operator=(WCHAR ch)
    {
        AssignCopy(1, &ch);
        return *this;
    }

    const CStringW& operator=(LPCSTR lpsz)
    {
        int nSrcLen = (lpsz != NULL) ? lstrlenA(lpsz) : 0;
        if(AllocBeforeWrite(nSrcLen))
        {
            _mbstowcsz(m_pchData, lpsz, nSrcLen + 1);
            ReleaseBuffer();
        }
        return *this;
    }

    const CStringW& operator=(LPCWSTR lpsz)
    {
        ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
        AssignCopy(SafeStrlen(lpsz), lpsz);
        return *this;
    }
    
    const CStringW& operator=(const unsigned char* psz)
        { *this = (LPCSTR)psz; return *this; }

    // string concatenation
    const CStringW& CStringW::operator+=(LPCWSTR lpsz)
    {
        ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
        ConcatInPlace(SafeStrlen(lpsz), lpsz);
        return *this;
    }

    const CStringW& operator+=(WCHAR ch)
    {
        ConcatInPlace(1, &ch);
        return *this;
    }

    const CStringW& operator+=(LPCSTR lpsz)
    {
        ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
        int nSrcLen = (lpsz != NULL) ? lstrlenA(lpsz) : 0;
        ConcatInPlace(nSrcLen, CStringW(lpsz));
        return *this;
    }

    // Concatenation
    // NOTE: "operator+" is done as friend functions for simplicity

    friend CStringW __stdcall operator+(const CStringW& string1, const CStringW& string2);
    friend CStringW __stdcall operator+(const CStringW& string, char ch);
    friend CStringW __stdcall operator+(const CStringW& string, WCHAR ch);
    friend CStringW __stdcall operator+(WCHAR ch, const CStringW& string);
    friend CStringW __stdcall operator+(char ch, const CStringW& string);

    // string comparison
    int Compare(LPCWSTR lpsz) const
        { return _cstrcmp(m_pchData, lpsz); }    // MBCS/Unicode aware

    int CompareNoCase(LPCWSTR lpsz) const      // ignore case
        { return _cstrcmpi(m_pchData, lpsz); }  // MBCS/Unicode aware

    int Collate(LPCWSTR lpsz) const            // NLS aware
        { return _cstrcoll(m_pchData, lpsz); }   // locale sensitive

    int CollateNoCase(LPCWSTR lpsz) const      // ignore case
        { return _cstrcolli(m_pchData, lpsz); }   // locale sensitive

    // simple sub-string extraction
    CStringW Mid(int nFirst, int nCount) const
    {
        // out-of-bounds requests return sensible things
        if (nFirst < 0)
            nFirst = 0;
        if (nCount < 0)
            nCount = 0;

        if (nFirst + nCount > GetData()->nDataLength)
            nCount = GetData()->nDataLength - nFirst;
        if (nFirst > GetData()->nDataLength)
            nCount = 0;

        CStringW dest;
        AllocCopy(dest, nCount, nFirst, 0);
        return dest;
    }


    CStringW Mid(int nFirst) const
    {
        return Mid(nFirst, GetData()->nDataLength - nFirst);
    }

    CStringW Left(int nCount) const
    {
        if (nCount < 0)
            nCount = 0;
        else if (nCount > GetData()->nDataLength)
            nCount = GetData()->nDataLength;

        CStringW dest;
        AllocCopy(dest, nCount, 0, 0);
        return dest;
    }

    CStringW Right(int nCount) const
    {
        if (nCount < 0)
            nCount = 0;
        else if (nCount > GetData()->nDataLength)
            nCount = GetData()->nDataLength;

        CStringW dest;
        AllocCopy(dest, nCount, GetData()->nDataLength-nCount, 0);
        return dest;
    }

    CStringW SpanIncluding(LPCWSTR lpszCharSet) const
    {
        // strspn equivalent
        ATLASSERT(_IsValidString(lpszCharSet));
        return Left(_cstrspn(m_pchData, lpszCharSet));
    
    }
    
    CStringW SpanExcluding(LPCWSTR lpszCharSet) const
    {
        // strcspn equivalent
        ATLASSERT(_IsValidString(lpszCharSet));
        return Left(_cstrcspn(m_pchData, lpszCharSet));
    }

    // upper/lower/reverse conversion
    void MakeUpper()
    {
        CopyBeforeWrite();
        CharUpperW(m_pchData);
    }

    void MakeLower()
    {
        CopyBeforeWrite();
        CharLowerW(m_pchData);
    }

    void MakeReverse()
    {
        CopyBeforeWrite();
        _cstrrev(m_pchData);
    }

    // trimming whitespace (either side)
    void TrimRight()
    {
        CopyBeforeWrite();

        // find beginning of trailing spaces by starting at beginning (DBCS aware)
        LPWSTR lpsz = m_pchData;
        LPWSTR lpszLast = NULL;
        while (*lpsz != '\0')
        {
            if (_cstrisspace(*lpsz))
            {
                if (lpszLast == NULL)
                    lpszLast = lpsz;
            }
            else
            {
                lpszLast = NULL;
            }
            lpsz = ::CharNextW(lpsz);
        }

        if (lpszLast != NULL)
        {
            // truncate at trailing space start
            *lpszLast = '\0';
            GetData()->nDataLength = (int)(DWORD_PTR)(lpszLast - m_pchData);
        }
    }

    void TrimLeft()
    {
        CopyBeforeWrite();

        // find first non-space character
        LPCWSTR lpsz = m_pchData;
        while (_cstrisspace(*lpsz))
            lpsz = ::CharNextW(lpsz);

        // fix up data and length
        int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_pchData);
        memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(WCHAR));
        GetData()->nDataLength = nDataLength;
    }

    // remove continuous occurrences of chTarget starting from right
    void TrimRight(WCHAR chTarget)
    {
        // find beginning of trailing matches
        // by starting at beginning (DBCS aware)

        CopyBeforeWrite();
        LPWSTR lpsz = m_pchData;
        LPWSTR lpszLast = NULL;

        while (*lpsz != '\0')
        {
            if (*lpsz == chTarget)
            {
                if (lpszLast == NULL)
                    lpszLast = lpsz;
            }
            else
                lpszLast = NULL;
            lpsz = ::CharNextW(lpsz);
        }

        if (lpszLast != NULL)
        {
            // truncate at left-most matching character
            *lpszLast = '\0';
            GetData()->nDataLength = (int)(DWORD_PTR)(lpszLast - m_pchData);
        }
    }

    // remove continuous occcurrences of characters in passed string,
    // starting from right
    void TrimRight(LPCWSTR lpszTargets)
    {
        // find beginning of trailing matches
        // by starting at beginning (DBCS aware)

        CopyBeforeWrite();
        LPWSTR lpsz = m_pchData;
        LPWSTR lpszLast = NULL;

        while (*lpsz != '\0')
        {
            if (_cstrchr(lpszTargets, *lpsz) != NULL)
            {
                if (lpszLast == NULL)
                    lpszLast = lpsz;
            }
            else
                lpszLast = NULL;
            lpsz = ::CharNextW(lpsz);
        }

        if (lpszLast != NULL)
        {
            // truncate at left-most matching character
            *lpszLast = '\0';
            GetData()->nDataLength = (int)(DWORD_PTR)(lpszLast - m_pchData);
        }
    }


    // remove continuous occurrences of chTarget starting from left
    void TrimLeft(WCHAR chTarget)
    {
        // find first non-matching character

        CopyBeforeWrite();
        LPCWSTR lpsz = m_pchData;

        while (chTarget == *lpsz)
            lpsz = ::CharNextW(lpsz);

        if (lpsz != m_pchData)
        {
            // fix up data and length
            int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_pchData);
            memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(WCHAR));
            GetData()->nDataLength = nDataLength;
        }
    }

    // remove continuous occcurrences of characters in
    // passed string, starting from left
    void TrimLeft(LPCWSTR lpszTargets)
    {
        // if we're not trimming anything, we're not doing any work
        if (SafeStrlen(lpszTargets) == 0)
            return;

        CopyBeforeWrite();
        LPCWSTR lpsz = m_pchData;

        while (*lpsz != '\0')
        {
            if (_cstrchr(lpszTargets, *lpsz) == NULL)
                break;
            lpsz = ::CharNextW(lpsz);
        }

        if (lpsz != m_pchData)
        {
            // fix up data and length
            int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_pchData);
            memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(WCHAR));
            GetData()->nDataLength = nDataLength;
        }
    }

    // advanced manipulation
    // replace occurrences of chOld with chNew
    int Replace(WCHAR chOld, WCHAR chNew)
    {
        int nCount = 0;

        // short-circuit the nop case
        if (chOld != chNew)
        {
            // otherwise modify each character that matches in the string
            CopyBeforeWrite();
            LPWSTR psz = m_pchData;
            LPWSTR pszEnd = psz + GetData()->nDataLength;
            while (psz < pszEnd)
            {
                // replace instances of the specified character only
                if (*psz == chOld)
                {
                    *psz = chNew;
                    nCount++;
                }
                psz = ::CharNextW(psz);
            }
        }
        return nCount;
    }

    // replace occurrences of substring lpszOld with lpszNew;
    // empty lpszNew removes instances of lpszOld
    int Replace(LPCWSTR lpszOld, LPCWSTR lpszNew)
    {
        // can't have empty or NULL lpszOld

        int nSourceLen = SafeStrlen(lpszOld);
        if (nSourceLen == 0)
            return 0;
        int nReplacementLen = SafeStrlen(lpszNew);

        // loop once to figure out the size of the result string
        int nCount = 0;
        LPWSTR lpszStart = m_pchData;
        LPWSTR lpszEnd = m_pchData + GetData()->nDataLength;
        LPWSTR lpszTarget;
        while (lpszStart < lpszEnd)
        {
            while ((lpszTarget = _cstrstr(lpszStart, lpszOld)) != NULL)
            {
                nCount++;
                lpszStart = lpszTarget + nSourceLen;
            }
            lpszStart += lstrlenW(lpszStart) + 1;
        }

        // if any changes were made, make them
        if (nCount > 0)
        {
            CopyBeforeWrite();

            // if the buffer is too small, just
            //   allocate a new buffer (slow but sure)
            int nOldLength = GetData()->nDataLength;
            int nNewLength =  nOldLength + (nReplacementLen - nSourceLen) * nCount;
            if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1)
            {
                CStringDataW* pOldData = GetData();
                LPWSTR pstr = m_pchData;
                if(!AllocBuffer(nNewLength))
                    return -1;
                memcpy(m_pchData, pstr, pOldData->nDataLength * sizeof(WCHAR));
                CStringW::Release(pOldData);
            }
            // else, we just do it in-place
            lpszStart = m_pchData;
            lpszEnd = m_pchData + GetData()->nDataLength;

            // loop again to actually do the work
            while (lpszStart < lpszEnd)
            {
                while ( (lpszTarget = _cstrstr(lpszStart, lpszOld)) != NULL)
                {
                    int nBalance = nOldLength - ((int)(DWORD_PTR)(lpszTarget - m_pchData) + nSourceLen);
                    memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen, nBalance * sizeof(WCHAR));
                    memcpy(lpszTarget, lpszNew, nReplacementLen * sizeof(WCHAR));
                    lpszStart = lpszTarget + nReplacementLen;
                    lpszStart[nBalance] = '\0';
                    nOldLength += (nReplacementLen - nSourceLen);
                }
                lpszStart += lstrlenW(lpszStart) + 1;
            }
            ATLASSERT(m_pchData[nNewLength] == '\0');
            GetData()->nDataLength = nNewLength;
        }

        return nCount;
    }

    // remove occurrences of chRemove
    int Remove(WCHAR chRemove)
    {
        CopyBeforeWrite();

        LPWSTR pstrSource = m_pchData;
        LPWSTR pstrDest = m_pchData;
        LPWSTR pstrEnd = m_pchData + GetData()->nDataLength;

        while (pstrSource < pstrEnd)
        {
            if (*pstrSource != chRemove)
            {
                *pstrDest = *pstrSource;
                pstrDest = ::CharNextW(pstrDest);
            }
            pstrSource = ::CharNextW(pstrSource);
        }
        *pstrDest = '\0';
        int nCount = (int)(DWORD_PTR)(pstrSource - pstrDest);
        GetData()->nDataLength -= nCount;

        return nCount;
    }

    // insert character at zero-based index; concatenates
    // if index is past end of string
    int Insert(int nIndex, WCHAR ch)
    {
        CopyBeforeWrite();

        if (nIndex < 0)
            nIndex = 0;

        int nNewLength = GetData()->nDataLength;
        if (nIndex > nNewLength)
            nIndex = nNewLength;
        nNewLength++;

        if (GetData()->nAllocLength < nNewLength)
        {
            CStringDataW* pOldData = GetData();
            LPWSTR pstr = m_pchData;
            if(!AllocBuffer(nNewLength))
                return -1;
            memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(WCHAR));
            CStringW::Release(pOldData);
        }

        // move existing bytes down
        memmove(m_pchData + nIndex + 1, m_pchData + nIndex, (nNewLength - nIndex) * sizeof(WCHAR));
        m_pchData[nIndex] = ch;
        GetData()->nDataLength = nNewLength;

        return nNewLength;
    }

    // insert substring at zero-based index; concatenates
    // if index is past end of string
    int Insert(int nIndex, LPCWSTR pstr)
    {
        if (nIndex < 0)
            nIndex = 0;

        int nInsertLength = SafeStrlen(pstr);
        int nNewLength = GetData()->nDataLength;
        if (nInsertLength > 0)
        {
            CopyBeforeWrite();
            if (nIndex > nNewLength)
                nIndex = nNewLength;
            nNewLength += nInsertLength;

            if (GetData()->nAllocLength < nNewLength)
            {
                CStringDataW* pOldData = GetData();
                LPWSTR pstr = m_pchData;
                if(!AllocBuffer(nNewLength))
                    return -1;
                memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(WCHAR));
                CStringW::Release(pOldData);
            }

            // move existing bytes down
            memmove(m_pchData + nIndex + nInsertLength, m_pchData + nIndex, (nNewLength - nIndex - nInsertLength + 1) * sizeof(WCHAR));
            memcpy(m_pchData + nIndex, pstr, nInsertLength * sizeof(WCHAR));
            GetData()->nDataLength = nNewLength;
        }

        return nNewLength;
    }

    // delete nCount characters starting at zero-based index
    int Delete(int nIndex, int nCount = 1)
    {
        if (nIndex < 0)
            nIndex = 0;
        int nNewLength = GetData()->nDataLength;
        if (nCount > 0 && nIndex < nNewLength)
        {
            CopyBeforeWrite();
            int nBytesToCopy = nNewLength - (nIndex + nCount) + 1;

            memmove(m_pchData + nIndex, m_pchData + nIndex + nCount, nBytesToCopy * sizeof(WCHAR));
            GetData()->nDataLength = nNewLength - nCount;
        }

        return nNewLength;
    }

    // searching (return starting index, or -1 if not found)
    // look for a single character match
    int Find(WCHAR ch) const                      // like "C" strchr
    {
        return Find(ch, 0);
    }

    int ReverseFind(WCHAR ch) const
    {
        // find last single character
        LPWSTR lpsz = _cstrrchr(m_pchData, ch);

        // return -1 if not found, distance from beginning otherwise
        return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
    }

    int Find(WCHAR ch, int nStart) const          // starting at index
    {
        int nLength = GetData()->nDataLength;
        if (nStart >= nLength)
            return -1;

        // find first single character
        LPWSTR lpsz = _cstrchr(m_pchData + nStart, ch);

        // return -1 if not found and index otherwise
        return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
    }

    int FindOneOf(LPCWSTR lpszCharSet) const
    {
        ATLASSERT(_IsValidString(lpszCharSet));
        LPWSTR lpsz = _cstrpbrk(m_pchData, lpszCharSet);
        return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
    }

    // look for a specific sub-string
    int Find(LPCWSTR lpszSub) const        // like "C" strstr
        { return Find(lpszSub, 0); }

    int Find(LPCWSTR lpszSub, int nStart) const  // starting at index
    {
        ATLASSERT(_IsValidString(lpszSub));

        int nLength = GetData()->nDataLength;
        if (nStart > nLength)
            return -1;

        // find first matching substring
        LPWSTR lpsz = _cstrstr(m_pchData + nStart, lpszSub);

        // return -1 for not found, distance from beginning otherwise
        return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
    }

    // Concatentation for non strings
    const CStringW& Append(int n)
    {
        WCHAR szBuffer[10];
        wsprintfW( szBuffer, L"%d", n );
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }

    // simple formatting
    BOOL __cdecl Format(LPCWSTR lpszFormat, ...)
    {
	    // format message into temporary buffer lpszTemp
	    va_list argList;
	    va_start(argList, lpszFormat);
	    LPWSTR lpszTemp;
	    BOOL bRet = TRUE;

	    if (::FormatMessageW(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			    lpszFormat, 0, 0, (LPWSTR)&lpszTemp, 0, &argList) == 0 || lpszTemp == NULL)
		    bRet = FALSE;

	    // assign lpszTemp into the resulting string and free the temporary
	    *this = lpszTemp;
	    LocalFree(lpszTemp);
	    va_end(argList);
	    return bRet;
    }

    BOOL __cdecl Format(UINT nFormatID, ...)
    {
	    CStringW strFormat;
	    BOOL bRet = strFormat.LoadString(nFormatID);
	    ATLASSERT(bRet != 0);

	    va_list argList;
	    va_start(argList, nFormatID);
	    bRet = FormatV(strFormat, argList);
	    va_end(argList);
	    return bRet;
    }

    BOOL FormatV(LPCWSTR lpszFormat, va_list argList)
    {
	    ATLASSERT(_IsValidString(lpszFormat));

	    enum _FormatModifiers
	    {
		    FORCE_ANSI =	0x10000,
		    FORCE_UNICODE =	0x20000,
		    FORCE_INT64 =	0x40000
	    };

	    va_list argListSave = argList;

	    // make a guess at the maximum length of the resulting string
	    int nMaxLen = 0;
	    for (LPCWSTR lpsz = lpszFormat; *lpsz != '\0'; lpsz = ::CharNextW(lpsz))
	    {
		    // handle '%' character, but watch out for '%%'
		    if (*lpsz != '%' || *(lpsz = ::CharNextW(lpsz)) == '%')
		    {
			    // this is instead of _tclen()
			    nMaxLen++;
			    continue;
		    }

		    int nItemLen = 0;

		    // handle '%' character with format
		    int nWidth = 0;
		    for (; *lpsz != '\0'; lpsz = ::CharNextW(lpsz))
		    {
			    // check for valid flags
			    if (*lpsz == '#')
				    nMaxLen += 2;   // for '0x'
			    else if (*lpsz == '*')
				    nWidth = va_arg(argList, int);
			    else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' || *lpsz == ' ')
				    ;
			    else // hit non-flag character
				    break;
		    }
		    // get width and skip it
		    if (nWidth == 0)
		    {
			    // width indicated by
			    nWidth = _cstrtoi(lpsz);
			    for (; *lpsz != '\0' && _cstrisdigit(*lpsz); lpsz = ::CharNextW(lpsz))
				    ;
		    }
		    ATLASSERT(nWidth >= 0);

		    int nPrecision = 0;
		    if (*lpsz == '.')
		    {
			    // skip past '.' separator (width.precision)
			    lpsz = ::CharNextW(lpsz);

			    // get precision and skip it
			    if (*lpsz == '*')
			    {
				    nPrecision = va_arg(argList, int);
				    lpsz = ::CharNextW(lpsz);
			    }
			    else
			    {
				    nPrecision = _cstrtoi(lpsz);
				    for (; *lpsz != '\0' && _cstrisdigit(*lpsz); lpsz = ::CharNextW(lpsz))
					    ;
			    }
			    ATLASSERT(nPrecision >= 0);
		    }

		    // should be on type modifier or specifier
		    int nModifier = 0;
		    if(lpsz[0] == _T('I') && lpsz[1] == _T('6') && lpsz[2] == _T('4'))
		    {
			    lpsz += 3;
			    nModifier = FORCE_INT64;
		    }
		    else
		    {
			    switch (*lpsz)
			    {
			    // modifiers that affect size
			    case 'h':
				    nModifier = FORCE_ANSI;
				    lpsz = ::CharNextW(lpsz);
				    break;
			    case 'l':
				    nModifier = FORCE_UNICODE;
				    lpsz = ::CharNextW(lpsz);
				    break;

			    // modifiers that do not affect size
			    case 'F':
			    case 'N':
			    case 'L':
				    lpsz = ::CharNextW(lpsz);
				    break;
			    }
		    }

		    // now should be on specifier
		    switch (*lpsz | nModifier)
		    {
		    // single characters
		    case 'c':
		    case 'C':
			    nItemLen = 2;
			    va_arg(argList, TCHAR);
			    break;
		    case 'c' | FORCE_ANSI:
		    case 'C' | FORCE_ANSI:
			    nItemLen = 2;
			    va_arg(argList, char);
			    break;
		    case 'c' | FORCE_UNICODE:
		    case 'C' | FORCE_UNICODE:
			    nItemLen = 2;
			    va_arg(argList, WCHAR);
			    break;

		    // strings
		    case 's':
		    {
			    LPCWSTR pstrNextArg = va_arg(argList, LPCWSTR);
			    if (pstrNextArg == NULL)
			    {
				    nItemLen = 6;  // "(null)"
			    }
			    else
			    {
				    nItemLen = lstrlenW(pstrNextArg);
				    nItemLen = max(1, nItemLen);
			    }
			    break;
		    }

		    case 'S':
		    {
#ifndef _UNICODE
			    LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
			    if (pstrNextArg == NULL)
			    {
				    nItemLen = 6;  // "(null)"
			    }
			    else
			    {
				    nItemLen = (int)wcslen(pstrNextArg);
				    nItemLen = max(1, nItemLen);
			    }
#else
			    LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
			    if (pstrNextArg == NULL)
			    {
				    nItemLen = 6; // "(null)"
			    }
			    else
			    {
				    nItemLen = lstrlenA(pstrNextArg);
				    nItemLen = max(1, nItemLen);
			    }
#endif
			    break;
		    }

		    case 's' | FORCE_ANSI:
		    case 'S' | FORCE_ANSI:
		    {
			    LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
			    if (pstrNextArg == NULL)
			    {
				    nItemLen = 6; // "(null)"
			    }
			    else
			    {
				    nItemLen = lstrlenA(pstrNextArg);
				    nItemLen = max(1, nItemLen);
			    }
			    break;
		    }

		    case 's' | FORCE_UNICODE:
		    case 'S' | FORCE_UNICODE:
		    {
			    LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
			    if (pstrNextArg == NULL)
			    {
				    nItemLen = 6; // "(null)"
			    }
			    else
			    {
				    nItemLen = (int)wcslen(pstrNextArg);
				    nItemLen = max(1, nItemLen);
			    }
			    break;
		    }
		    }

		    // adjust nItemLen for strings
		    if (nItemLen != 0)
		    {
			    nItemLen = max(nItemLen, nWidth);
			    if (nPrecision != 0)
				    nItemLen = min(nItemLen, nPrecision);
		    }
		    else
		    {
			    switch (*lpsz)
			    {
			    // integers
			    case 'd':
			    case 'i':
			    case 'u':
			    case 'x':
			    case 'X':
			    case 'o':
				    if (nModifier & FORCE_INT64)
					    va_arg(argList, __int64);
				    else
					    va_arg(argList, int);
				    nItemLen = 32;
				    nItemLen = max(nItemLen, nWidth + nPrecision);
				    break;

#ifndef _ATL_USE_CSTRING_FLOAT
			    case 'e':
			    case 'f':
			    case 'g':
			    case 'G':
				    ATLASSERT(!"Floating point (%%e, %%f, %%g, and %%G) is not supported by the WTL::CString class.");
#ifndef _DEBUG
				    ::OutputDebugString(_T("Floating point (%%e, %%f, %%g, and %%G) is not supported by the WTL::CString class."));
				    ::DebugBreak();
#endif //!_DEBUG
				    break;
#else //_ATL_USE_CSTRING_FLOAT
			    case 'e':
			    case 'g':
			    case 'G':
				    va_arg(argList, double);
				    nItemLen = 128;
				    nItemLen = max(nItemLen, nWidth + nPrecision);
				    break;
			    case 'f':
				    {
					    double f;
					    LPTSTR pszTemp;

					    // 312 == strlen("-1+(309 zeroes).")
					    // 309 zeroes == max precision of a double
					    // 6 == adjustment in case precision is not specified,
					    //   which means that the precision defaults to 6
					    pszTemp = (LPTSTR)_alloca(max(nWidth, 312 + nPrecision + 6));

					    f = va_arg(argList, double);
					    _stprintf(pszTemp, _T( "%*.*f" ), nWidth, nPrecision + 6, f);
					    nItemLen = _tcslen(pszTemp);
				    }
				    break;
#endif //_ATL_USE_CSTRING_FLOAT

			    case 'p':
				    va_arg(argList, void*);
				    nItemLen = 32;
				    nItemLen = max(nItemLen, nWidth + nPrecision);
				    break;

			    // no output
			    case 'n':
				    va_arg(argList, int*);
				    break;

			    default:
				    ATLASSERT(FALSE);  // unknown formatting option
			    }
		    }

		    // adjust nMaxLen for output nItemLen
		    nMaxLen += nItemLen;
	    }

	    if(GetBuffer(nMaxLen) == NULL)
		    return FALSE;
#ifndef _ATL_USE_CSTRING_FLOAT
	    int nRet = wvsprintfW(m_pchData, lpszFormat, argListSave);
#else //_ATL_USE_CSTRING_FLOAT
	    int nRet = vswprintf(m_pchData, lpszFormat, argListSave);
#endif //_ATL_USE_CSTRING_FLOAT
	    nRet;	// ref
	    ATLASSERT(nRet <= GetAllocLength());
	    ReleaseBuffer();

	    va_end(argListSave);
	    return TRUE;
    }

    // formatting for localization (uses FormatMessage API)
    BOOL __cdecl FormatMessage(LPCWSTR lpszFormat, ...)
    {
	    ATLASSERT(_IsValidString(lpszFormat));

	    va_list argList;
	    va_start(argList, lpszFormat);
	    BOOL bRet = FormatV(lpszFormat, argList);
	    va_end(argList);
	    return bRet;
    }

    BOOL __cdecl FormatMessage(UINT nFormatID, ...)
    {
	    CStringW strFormat;
	    BOOL bRet = strFormat.LoadString(nFormatID);
	    ATLASSERT(bRet != 0);

	    va_list argList;
	    va_start(argList, nFormatID);
	    bRet = FormatV(strFormat, argList);
	    va_end(argList);
	    return bRet;
    }

    // Windows support
    BOOL LoadString(UINT nID)          // load from string resource (255 chars max)
    {
        // try fixed buffer first (to avoid wasting space in the heap)
        WCHAR szTemp[256];
        int nCount =  sizeof(szTemp) / sizeof(szTemp[0]);
        int nLen = _LoadString(nID, szTemp, nCount);
        if (nCount - nLen > 2)
        {
            *this = szTemp;
            return (nLen > 0);
        }

        // try buffer size of 512, then larger size until entire string is retrieved
        int nSize = 256;
        do
        {
            nSize += 256;
            LPWSTR lpstr = GetBuffer(nSize - 1);
            if(lpstr == NULL)
            {
                nLen = 0;
                break;
            }
            nLen = _LoadString(nID, lpstr, nSize);
        } while (nSize - nLen <= 2);
        ReleaseBuffer();

        return (nLen > 0);
    }

#ifndef _ATL_NO_COM
    // OLE BSTR support (use for OLE automation)
    BSTR AllocSysString() const
    {
       return ::SysAllocStringLen(m_pchData, GetData()->nDataLength);
    }

    BSTR SetSysString(BSTR* pbstr) const
    {
        ::SysReAllocStringLen(pbstr, m_pchData, GetData()->nDataLength);
    	ATLASSERT(*pbstr != NULL);
        return *pbstr;
    }
#endif //!_ATL_NO_COM

    // Access to string implementation buffer as "C" character array
    LPWSTR GetBuffer(int nMinBufLength)
    {
        ATLASSERT(nMinBufLength >= 0);

        if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength)
        {
            // we have to grow the buffer
            CStringDataW* pOldData = GetData();
            int nOldLen = GetData()->nDataLength;   // AllocBuffer will tromp it
            if (nMinBufLength < nOldLen)
                nMinBufLength = nOldLen;

            if(!AllocBuffer(nMinBufLength))
                return NULL;

            memcpy(m_pchData, pOldData->data(), (nOldLen + 1) * sizeof(WCHAR));
            GetData()->nDataLength = nOldLen;
            CStringW::Release(pOldData);
        }
        ATLASSERT(GetData()->nRefs <= 1);

        // return a pointer to the character storage for this string
        ATLASSERT(m_pchData != NULL);
        return m_pchData;
    }

    void ReleaseBuffer(int nNewLength = -1)
    {
        CopyBeforeWrite();  // just in case GetBuffer was not called

        if (nNewLength == -1)
            nNewLength = lstrlenW(m_pchData); // zero terminated

        ATLASSERT(nNewLength <= GetData()->nAllocLength);
        GetData()->nDataLength = nNewLength;
        m_pchData[nNewLength] = '\0';
    }

    LPWSTR GetBufferSetLength(int nNewLength)
    {
        ATLASSERT(nNewLength >= 0);

        if(GetBuffer(nNewLength) == NULL)
            return NULL;

        GetData()->nDataLength = nNewLength;
        m_pchData[nNewLength] = '\0';
        return m_pchData;
    }

    void FreeExtra()
    {
        ATLASSERT(GetData()->nDataLength <= GetData()->nAllocLength);
        if (GetData()->nDataLength != GetData()->nAllocLength)
        {
            CStringDataW* pOldData = GetData();
            if(AllocBuffer(GetData()->nDataLength))
            {
                memcpy(m_pchData, pOldData->data(), pOldData->nDataLength * sizeof(WCHAR));
                ATLASSERT(m_pchData[GetData()->nDataLength] == '\0');
                CStringW::Release(pOldData);
            }
        }
        ATLASSERT(GetData() != NULL);
    }

    // Use LockBuffer/UnlockBuffer to turn refcounting off
    LPWSTR LockBuffer()
    {
        LPWSTR lpsz = GetBuffer(0);
        if(lpsz != NULL)
            GetData()->nRefs = -1;
        return lpsz;
    }

    void UnlockBuffer()
    {
        ATLASSERT(GetData()->nRefs == -1);
        if (GetData() != _atltmpDataNilW)
            GetData()->nRefs = 1;
    }

// Implementation
public:
    ~CStringW()
    {
        // free any attached data
        if (GetData() != _atltmpDataNilW)
        {
            if (InterlockedDecrement(&GetData()->nRefs) <= 0)
                delete[] (BYTE*)GetData();
        }
    }

    int GetAllocLength() const
        { return GetData()->nAllocLength; }

    static BOOL __stdcall _IsValidString(LPCWSTR lpsz, int nLength = -1)
    {
        if(lpsz == NULL)
            return FALSE;
        return !::IsBadStringPtrW(lpsz, nLength);
    }

    static BOOL __stdcall _IsValidString(LPCSTR lpsz, int nLength = -1)
    {
        if(lpsz == NULL)
            return FALSE;
        return !::IsBadStringPtrA(lpsz, nLength);
    }

protected:
    LPWSTR m_pchData;   // pointer to ref counted string data

    // implementation helpers
    CStringDataW* GetData() const
        { ATLASSERT(m_pchData != NULL); return ((CStringDataW*)m_pchData) - 1; }

    void Init()
        { m_pchData = _GetEmptyString().m_pchData; }

    void AllocCopy(CStringW& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const
    {
        // will clone the data attached to this string
        // allocating 'nExtraLen' characters
        // Places results in uninitialized string 'dest'
        // Will copy the part or all of original data to start of new string

        int nNewLen = nCopyLen + nExtraLen;
        if (nNewLen == 0)
        {
            dest.Init();
        }
        else
        {
            if(dest.AllocBuffer(nNewLen))
                memcpy(dest.m_pchData, m_pchData + nCopyIndex, nCopyLen * sizeof(WCHAR));
        }
    }


    BOOL AllocBuffer(int nLen)
    {
        // always allocate one extra character for '\0' termination
        // assumes [optimistically] that data length will equal allocation length

        ATLASSERT(nLen >= 0);
        ATLASSERT(nLen <= INT_MAX - 1);    // max size (enough room for 1 extra)

        if (nLen == 0)
        {
            Init();
        }
        else
        {
            CStringDataW* pData = NULL;
            ATLTRY(pData = (CStringDataW*)new BYTE[sizeof(CStringDataW) + (nLen + 1) * sizeof(WCHAR)]);
            if(pData == NULL)
                return FALSE;

            pData->nRefs = 1;
            pData->data()[nLen] = '\0';
            pData->nDataLength = nLen;
            pData->nAllocLength = nLen;
            m_pchData = pData->data();
        }

        return TRUE;
    }

    void AssignCopy(int nSrcLen, LPCWSTR lpszSrcData)
    {
        if(AllocBeforeWrite(nSrcLen))
        {
            memcpy(m_pchData, lpszSrcData, nSrcLen * sizeof(WCHAR));
            GetData()->nDataLength = nSrcLen;
            m_pchData[nSrcLen] = '\0';
        }
    }

    
    BOOL ConcatCopy(int nSrc1Len, LPCWSTR lpszSrc1Data, int nSrc2Len, LPCWSTR lpszSrc2Data)
    {
      // -- master concatenation routine
      // Concatenate two sources
      // -- assume that 'this' is a new CString object

        BOOL bRet = TRUE;
        int nNewLen = nSrc1Len + nSrc2Len;
        if (nNewLen != 0)
        {
            bRet = AllocBuffer(nNewLen);
            if (bRet)
            {
                memcpy(m_pchData, lpszSrc1Data, nSrc1Len * sizeof(WCHAR));
                memcpy(m_pchData + nSrc1Len, lpszSrc2Data, nSrc2Len * sizeof(WCHAR));
            }
        }
        return bRet;
    }

    void ConcatInPlace(int nSrcLen, LPCWSTR lpszSrcData)
    {
        //  -- the main routine for += operators

        // concatenating an empty string is a no-op!
        if (nSrcLen == 0)
            return;

        // if the buffer is too small, or we have a width mis-match, just
        //   allocate a new buffer (slow but sure)
        if (GetData()->nRefs > 1 || GetData()->nDataLength + nSrcLen > GetData()->nAllocLength)
        {
            // we have to grow the buffer, use the ConcatCopy routine
            CStringDataW* pOldData = GetData();
            if (ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData))
            {
                ATLASSERT(pOldData != NULL);
                CStringW::Release(pOldData);
            }
        }
        else
        {
            // fast concatenation when buffer big enough
            memcpy(m_pchData + GetData()->nDataLength, lpszSrcData, nSrcLen * sizeof(WCHAR));
            GetData()->nDataLength += nSrcLen;
            ATLASSERT(GetData()->nDataLength <= GetData()->nAllocLength);
            m_pchData[GetData()->nDataLength] = '\0';
        }
    }

    void CopyBeforeWrite()
    {
        if (GetData()->nRefs > 1)
        {
            CStringDataW* pData = GetData();
            Release();
            if(AllocBuffer(pData->nDataLength))
                memcpy(m_pchData, pData->data(), (pData->nDataLength + 1) * sizeof(WCHAR));
        }
        ATLASSERT(GetData()->nRefs <= 1);
    }

    BOOL AllocBeforeWrite(int nLen)
    {
        BOOL bRet = TRUE;
        if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength)
        {
            Release();
            bRet = AllocBuffer(nLen);
        }
        ATLASSERT(GetData()->nRefs <= 1);
        return bRet;
    }

    void Release()
    {
        if (GetData() != _atltmpDataNilW)
        {
            ATLASSERT(GetData()->nRefs != 0);
            if (InterlockedDecrement(&GetData()->nRefs) <= 0)
                delete[] (BYTE*)GetData();
            Init();
        }
    }

    static void PASCAL Release(CStringDataW* pData)
    {
        if (pData != _atltmpDataNilW)
        {
            ATLASSERT(pData->nRefs != 0);
            if (InterlockedDecrement(&pData->nRefs) <= 0)
                delete[] (BYTE*)pData;
        }
    }
    
    static int PASCAL SafeStrlen(LPCWSTR lpsz)
        { return (lpsz == NULL) ? 0 : lstrlenW(lpsz); }

    static int __stdcall _LoadString(UINT nID, LPWSTR lpszBuf, UINT nMaxBuf)
    {
#ifdef _DEBUG
        // LoadString without annoying warning from the Debug kernel if the
        //  segment containing the string is not present
        if (::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE((nID>>4) + 1), RT_STRING) == NULL)
        {
            lpszBuf[0] = '\0';
            return 0; // not found
        }
#endif //_DEBUG
        int nLen = ::LoadStringW(AfxGetResourceHandle(), nID, lpszBuf, nMaxBuf);
        if (nLen == 0)
            lpszBuf[0] = '\0';
        return nLen;
    }

    static const CStringW& __stdcall _GetEmptyString()
        { return *(CStringW*)&_atltmpPchNilW; }

// CStringW conversion helpers
    static int __cdecl _wcstombsz(char* mbstr, const wchar_t* wcstr, size_t count)
    {
        if (count == 0 && mbstr != NULL)
            return 0;

        int result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, mbstr, (int)count, NULL, NULL);
        ATLASSERT(mbstr == NULL || result <= (int)count);
        if (result > 0)
            mbstr[result - 1] = 0;
        return result;
    }

    static int __cdecl _mbstowcsz(wchar_t* wcstr, const char* mbstr, size_t count)
    {
        if (count == 0 && wcstr != NULL)
            return 0;

        int result = ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1, wcstr, (int)count);
        ATLASSERT(wcstr == NULL || result <= (int)count);
        if (result > 0)
            wcstr[result - 1] = 0;
        return result;
    }

// Helpers to avoid CRT startup code
#ifdef _ATL_MIN_CRT
    static WCHAR* _cstrchr(const WCHAR* p, WCHAR ch)
    {
        //strchr for '\0' should succeed
        while (*p != 0)
        {
            if (*p == ch)
                break;
            p = ::CharNextW(p);
        }
        return (WCHAR*)((*p == ch) ? p : NULL);
    }
    
    static WCHAR* _cstrchr_db(const WCHAR* p, WCHAR ch1, WCHAR ch2)
    {
        const WCHAR* lpsz = NULL;
        while (*p != 0)
        {
            if (*p == ch1 && *(p + 1) == ch2)
            {
                lpsz = p;
                break;
            }
            p = ::CharNextW(p);
        }
        return (WCHAR*)lpsz;
    }
    
    static WCHAR* _cstrrchr(const WCHAR* p, WCHAR ch)
    {
        const WCHAR* lpsz = NULL;
        while (*p != 0)
        {
            if (*p == ch)
                lpsz = p;
            p = ::CharNextW(p);
        }
        return (WCHAR*)lpsz;
    }
    
    static WCHAR* _cstrrev(WCHAR* pStr)
    {
        // Optimize NULL, zero-length, and single-char case.
        if ((pStr == NULL) || (pStr[0] == '\0') || (pStr[1] == '\0'))
            return pStr;

        WCHAR* p = pStr;

        while (p[1] != 0) 
        {
            WCHAR* pNext = ::CharNextW(p);
            if(pNext > p + 1)
            {
                char p1 = *(char*)p;
                *(char*)p = *(char*)(p + 1);
                *(char*)(p + 1) = p1;
            }
            p = pNext;
        }

        WCHAR* q = pStr;

        while (q < p)
        {
            WCHAR t = *q;
            *q = *p;
            *p = t;
            q++;
            p--;
        }
        return (WCHAR*)pStr;
    }
    
    static WCHAR* _cstrstr(const WCHAR* pStr, const WCHAR* pCharSet)
    {
        int nLen = lstrlenW(pCharSet);
        if (nLen == 0)
            return (WCHAR*)pStr;

        const WCHAR* pRet = NULL;
        const WCHAR* pCur = pStr;
        while((pStr = _cstrchr(pCur, *pCharSet)) != NULL)
        {
            if(memcmp(pCur, pCharSet, nLen * sizeof(WCHAR)) == 0)
            {
                pRet = pCur;
                break;
            }
            pCur = ::CharNextW(pCur);
        }
        return (WCHAR*) pRet;
    }
    
    static int _cstrspn(const WCHAR* pStr, const WCHAR* pCharSet)
    {
        int nRet = 0;
        WCHAR* p = (WCHAR*)pStr;
        while (*p != 0)
        {
            WCHAR* pNext = ::CharNextW(p);
            if(pNext > p + 1)
            {
                if(_cstrchr_db(pCharSet, *p, *(p + 1)) == NULL)
                    break;
                nRet += 2;
            }
            else
            {
                if(_cstrchr(pCharSet, *p) == NULL)
                    break;
                nRet++;
            }
            p = pNext;
        }
        return nRet;
    }
    
    static int _cstrcspn(const WCHAR* pStr, const WCHAR* pCharSet)
    {
        int nRet = 0;
        WCHAR* p = (WCHAR*)pStr;
        while (*p != 0)
        {
            WCHAR* pNext = ::CharNextW(p);
            if(pNext > p + 1)
            {
                if(_cstrchr_db(pCharSet, *p, *(p + 1)) != NULL)
                    break;
                nRet += 2;
            }
            else
            {
                if(_cstrchr(pCharSet, *p) != NULL)
                    break;
                nRet++;
            }
            p = pNext;
        }
        return nRet;
    }
    
    static WCHAR* _cstrpbrk(const WCHAR* p, const WCHAR* lpszCharSet)
    {
        while (*p != 0)
        {
            if (_cstrchr(lpszCharSet, *p) != NULL)
            {
                return (WCHAR*)p;
                break;
            }
            p = ::CharNextW(p);
        }
        return NULL;
    }
    
    static int _cstrisdigit(WCHAR ch)
    {
        WORD type;
        GetStringTypeExW(GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
        return (type & C1_DIGIT) == C1_DIGIT;
    }
    
    static int _cstrisspace(WCHAR ch)
    {
        WORD type;
        GetStringTypeExW(GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
        return (type & C1_SPACE) == C1_SPACE;
    }
    
    static int _cstrcmp(const WCHAR* pstrOne, const WCHAR* pstrOther)
        { return lstrcmpW(pstrOne, pstrOther); }
    
    static int _cstrcmpi(const WCHAR* pstrOne, const WCHAR* pstrOther)
        { return lstrcmpiW(pstrOne, pstrOther); }
    
    static int _cstrcoll(const WCHAR* pstrOne, const WCHAR* pstrOther)
    {
        int nRet = CompareStringW(GetThreadLocale(), 0, pstrOne, -1, pstrOther, -1);
        ATLASSERT(nRet != 0);
        return nRet - 2;  // Convert to strcmp convention.  This really is documented.
    }
    
    static int _cstrcolli(const WCHAR* pstrOne, const WCHAR* pstrOther)
    {
        int nRet = CompareStringW(GetThreadLocale(), NORM_IGNORECASE, pstrOne, -1, pstrOther, -1);
        ATLASSERT(nRet != 0);
        return nRet - 2;  // Convert to strcmp convention.  This really is documented.
    }
    
    static int _cstrtoi(const WCHAR* nptr)
    {
        int c;              /* current char */
        int total;          /* current total */
        int sign;           /* if '-', then negative, otherwise positive */

        while ( _cstrisspace(*nptr) )
            ++nptr;

        c = (int)(_TUCHAR)*nptr++;
        sign = c;           /* save sign indication */
        if (c == _T('-') || c == _T('+'))
            c = (int)(_TUCHAR)*nptr++;    /* skip sign */

        total = 0;

        while (_cstrisdigit((WCHAR)c)) {
            total = 10 * total + (c - '0');     /* accumulate digit */
            c = (int)(_TUCHAR)*nptr++;    /* get next char */
        }

        if (sign == '-')
            return -total;
        else
            return total;   /* return result, negated if necessary */
    }

#else //!_ATL_MIN_CRT

    static WCHAR* _cstrchr(const WCHAR* p, WCHAR ch)
        { return wcschr(p, ch); }

    static WCHAR* _cstrrchr(const WCHAR* p, WCHAR ch)
        { return wcsrchr(p, ch); }

    static WCHAR* _cstrrev(WCHAR* pStr)
        { return _wcsrev(pStr); }

    static WCHAR* _cstrstr(const WCHAR* pStr, const WCHAR* pCharSet)
        { return wcsstr(pStr, pCharSet); }

    static int _cstrspn(const WCHAR* pStr, const WCHAR* pCharSet)
        { return (int)wcsspn(pStr, pCharSet); }

    static int _cstrcspn(const WCHAR* pStr, const WCHAR* pCharSet)
        { return (int)wcscspn(pStr, pCharSet); }

    static WCHAR* _cstrpbrk(const WCHAR* p, const WCHAR* lpszCharSet)
        { return wcspbrk(p, lpszCharSet); }

    static int _cstrisdigit(WCHAR ch)
        { return iswdigit(ch); }

    static int _cstrisspace(WCHAR ch)
        { return iswspace(ch); }

    static int _cstrcmp(const WCHAR* pstrOne, const WCHAR* pstrOther)
        { return wcscmp(pstrOne, pstrOther); }

    static int _cstrcmpi(const WCHAR* pstrOne, const WCHAR* pstrOther)
        { return _wcsicmp(pstrOne, pstrOther); }

    static int _cstrcoll(const WCHAR* pstrOne, const WCHAR* pstrOther)
        { return wcscoll(pstrOne, pstrOther); }

    static int _cstrcolli(const WCHAR* pstrOne, const WCHAR* pstrOther)
        { return _wcsicoll(pstrOne, pstrOther); }

    static int _cstrtoi(const WCHAR* nptr)
        { return _wtoi(nptr); }

#endif //!_ATL_MIN_CRT

};

// Compare helpers
inline bool __stdcall operator==(const CStringW& s1, const CStringW& s2)
    { return s1.Compare(s2) == 0; }
inline bool __stdcall operator==(const CStringW& s1, LPCWSTR s2)
    { return s1.Compare(s2) == 0; }
inline bool __stdcall operator==(LPCWSTR s1, const CStringW& s2)
    { return s2.Compare(s1) == 0; }
inline bool __stdcall operator!=(const CStringW& s1, const CStringW& s2)
    { return s1.Compare(s2) != 0; }
inline bool __stdcall operator!=(const CStringW& s1, LPCWSTR s2)
    { return s1.Compare(s2) != 0; }
inline bool __stdcall operator!=(LPCWSTR s1, const CStringW& s2)
    { return s2.Compare(s1) != 0; }
inline bool __stdcall operator<(const CStringW& s1, const CStringW& s2)
    { return s1.Compare(s2) < 0; }
inline bool __stdcall operator<(const CStringW& s1, LPCWSTR s2)
    { return s1.Compare(s2) < 0; }
inline bool __stdcall operator<(LPCWSTR s1, const CStringW& s2)
    { return s2.Compare(s1) > 0; }
inline bool __stdcall operator>(const CStringW& s1, const CStringW& s2)
    { return s1.Compare(s2) > 0; }
inline bool __stdcall operator>(const CStringW& s1, LPCWSTR s2)
    { return s1.Compare(s2) > 0; }
inline bool __stdcall operator>(LPCWSTR s1, const CStringW& s2)
    { return s2.Compare(s1) < 0; }
inline bool __stdcall operator<=(const CStringW& s1, const CStringW& s2)
    { return s1.Compare(s2) <= 0; }
inline bool __stdcall operator<=(const CStringW& s1, LPCWSTR s2)
    { return s1.Compare(s2) <= 0; }
inline bool __stdcall operator<=(LPCWSTR s1, const CStringW& s2)
    { return s2.Compare(s1) >= 0; }
inline bool __stdcall operator>=(const CStringW& s1, const CStringW& s2)
    { return s1.Compare(s2) >= 0; }
inline bool __stdcall operator>=(const CStringW& s1, LPCWSTR s2)
    { return s1.Compare(s2) >= 0; }
inline bool __stdcall operator>=(LPCWSTR s1, const CStringW& s2)
    { return s2.Compare(s1) <= 0; }

// friend functions
inline CStringW __stdcall operator+(const CStringW& string1, const CStringW& string2)
{
    CStringW s;
    s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData, string2.GetData()->nDataLength, string2.m_pchData);
    return s;
}

inline CStringW __stdcall operator+(const CStringW& string1, char ch)
{
    CStringW s;
    WCHAR wch(ch);
    s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData, 1, &wch);
    return s;
}

inline CStringW __stdcall operator+(const CStringW& string1, WCHAR ch)
{
    CStringW s;
    s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData, 1, &ch);
    return s;
}

inline CStringW __stdcall operator+(WCHAR ch, const CStringW& string1)
{
    CStringW s;
    s.ConcatCopy(1, &ch, string1.GetData()->nDataLength, string1.m_pchData);
    return s;
}

inline CStringW __stdcall operator+(char ch, const CStringW& string1)
{
    CStringW s;
    WCHAR wch(ch);
    s.ConcatCopy(1, &wch, string1.GetData()->nDataLength, string1.m_pchData);
    return s;
}


#endif  // WTL_CSTRING_W_H__INCLUDED
