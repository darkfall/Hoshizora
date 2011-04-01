/*
 *  SoraStringConv.cpp
 *  KuiIP
 *
 *  Created by griffin clare on 5/6/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoraStringConv.h"

#include <algorithm>


namespace sora {

	
std::string int_to_str(int32 n)
{
	char *str = NULL;                  
	int length = 1;                     
	int temp = 0;
	int sign = 1;                      
	
	if(n<0) {
		sign = -1;
		n = -n;
		++length;                         
	}
	
	temp = n;
	do {
		++length;
	}
	while((temp /= 10)>0);
	
	str = (char*)malloc(length);        
	
	if(sign<0)                      
		str[0] = '-';                    
	
	str[--length] = '\0';             
	
	do {
		str[--length] = '0'+n%10;
	}while((n /= 10) > 0);
	
	std::string buffer(str);
	delete str;
	return buffer;
}

std::string fp_to_str(float32 x)
{
	char *str = NULL;            
	char *integral = NULL;           
	char *fraction = NULL;          
	size_t length = 0;            
	
	std::string inte = int_to_str((int)x);
	integral = const_cast<char*>(inte.c_str());    
	
	if(x<0)
		x = -x;
	std::string frac = int_to_str((int)(100.0*(x+0.005-(int)x)));
	fraction = const_cast<char*>(frac.c_str());
	
	length = strlen(integral)+strlen(fraction)+2;  /* Total length including point and terminator */
	
	/* Fraction must be two digits so allow for it */
	if(strlen(fraction)<2)
		++length;
	
	str = (char*)malloc(length);        /* Allocate memory for total */
	strcpy(str, integral);              /* Copy the integral part    */
	strcat(str, ".");                   /* Append decimal point      */
	
	if(strlen(fraction)<2)              /* If fraction less than two digits */
		strcat(str,"0");                  /* Append leading zero       */
	
	strcat(str, fraction);              /* Append fractional part    */

	
	std::string buffer(str);
	delete str;
	return buffer;
}

#ifdef WIN32
	std::string WChar2Ansi(const std::wstring& pwszSrc)
	{
		int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc.c_str(), -1, NULL, 0, NULL, NULL);
		if (nLen<= 0) return std::string("");
		char* pszDst = new char[nLen];
		if (NULL == pszDst) return std::string("");
		WideCharToMultiByte(CP_ACP, 0, pwszSrc.c_str(), -1, pszDst, nLen, NULL, NULL);
		pszDst[nLen -1] = 0;
		std::string strTemp(pszDst);
		delete [] pszDst;
		return strTemp;
	}

	std::wstring Ansi2WChar(const std::string& pszSrc, int nLen)
	{
		if(pszSrc.size() == 0)
			return L"\0";
		int nSize = MultiByteToWideChar(CP_ACP, 0, pszSrc.c_str(), nLen, 0, 0);
		if(nSize <= 0) return NULL;
		WCHAR *pwszDst = new WCHAR[nSize+1];
		if( NULL == pwszDst) return NULL;
		MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc.c_str(), nLen, pwszDst, nSize);
		pwszDst[nSize] = 0;
		if( pwszDst[0] == 0xFEFF)                    // skip Oxfeff
			for(int i = 0; i < nSize; i ++)
				pwszDst[i] = pwszDst[i+1];
		std::wstring wcharString(pwszDst);
		delete pwszDst;
		return wcharString;
	}
#endif

std::string ws2s(const std::wstring& ws)
{
#ifdef WIN32
	return WChar2Ansi(ws);
#endif
    std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
    setlocale(LC_CTYPE, "chs");
    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    std::string result = _Dest;
    delete []_Dest;
    setlocale(LC_CTYPE, curLocale.c_str());
    return result;
}

std::wstring s2ws(const std::string& s)
{
#ifdef WIN32
	return Ansi2WChar(s, s.size());
#endif
    setlocale(LC_CTYPE, "chs"); 
    const char* _Source = s.c_str();
    size_t _Dsize = s.size() + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest,_Source,_Dsize);
    std::wstring result = _Dest;
    delete []_Dest;
    setlocale(LC_CTYPE, "C");
    return result;
}
	
	std::string ws2s2(const std::wstring& ws) {
		std::string buffer(ws.length(), ' ');
		std::copy(ws.begin(), ws.end(), buffer.begin());
		return buffer;
	}
	
	std::wstring s2ws2(const std::string& s) {
		std::wstring buffer(s.length(), L' ');
		std::copy(s.begin(), s.end(), buffer.begin());
		return buffer;
	}

	
inline size_t skipSpaces(const std::string& str, size_t currPos) {
	char c = str[currPos];
	while(c == ' ' || c == '\n' || c == '\r' || c == '\t') ++currPos;
	return currPos;
}

void deliStr(std::vector<std::string>& c, const std::string& str, char deli) {
	size_t start = 0, end = 0;
	while(end < str.size()) {
		while(str[end] != deli && end < str.size()) { ++end; }
		if(end == str.size()) {
			c.push_back( str.substr(start, str.size()) );
			break;
		}
		c.push_back( str.substr(start, end-start) );
		
		++end; end = skipSpaces(str, end);
		start = end;
	}
}

} // namespace sora