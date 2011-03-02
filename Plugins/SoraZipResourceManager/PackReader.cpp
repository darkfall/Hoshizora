//////////////////////////////////////////////////////////////////////////////////////
//
// Copyright(C) 2009 BOGY.CN
//
// Author: Î¢ÃîµÄÆ½ºâ
// Mailto: bogy.cn@gmail.com
//
//////////////////////////////////////////////////////////////////////////////////////
#include "PackReader.h"

#include <string>
#include <iostream>
#include <memory.h>
#include <string.h>

#if !defined(__APPLE__)
#include <malloc.h>
#endif

using namespace std;

// memory must be release by caller
#ifndef WIN32
inline char* _strupr(char* a) {
	char *ret = a;
	while(*a != '\0') {
		if(islower(*a))
			*a = toupper(*a);
		++a;
    }
	return ret;
}
#endif

namespace BOGY
{

PackReader::PackReader(void)
{
	m_UnzFile = NULL;	// modeRead
	memset(m_szCode, 0, sizeof(m_szCode));
}

PackReader::~PackReader(void)
{
	m_Seeker.clear();
}

bool PackReader::Open(const char* lpszPackPath, const char* lpszPackCode)
{
	if (0 == lpszPackPath || 0 == *lpszPackPath)
		return false;

	if (0 == lpszPackCode || 0 == *lpszPackCode)
		lpszPackCode = "";

	//bool lResult = false;

	Close();

	strcpy(m_szCode, lpszPackCode);
	m_UnzFile = unzOpen(lpszPackPath);

	if(m_UnzFile != 0) {
		cache();
		return true;
	}
	return false;
}

bool PackReader::From(void* lpszPackData, uLong nPackSize, const char* lpszPackCode)
{
	if (0 == lpszPackData || 0 == nPackSize)
		return false;

	if (0 == lpszPackCode || 0 == *lpszPackCode)
		lpszPackCode = "";

	Close();

	strcpy(m_szCode, lpszPackCode);
	m_UnzFile = unzOpenFromMemory(lpszPackData, nPackSize);

	if(m_UnzFile != 0) {
		cache();
		return true;
	}
	return false;
}


bool PackReader::Close()
{
	bool lResult = false;

	if (0 != m_UnzFile) {
		lResult = (Z_OK == unzClose(m_UnzFile));
		m_UnzFile = 0;
	}

	m_Seeker.clear();
	return lResult;
}

uLong PackReader::GetFileCRC(const char* lpszFileName)
{
	unz_file_info* lpUnzFileInfo = GetUnzFileInfo(lpszFileName);
	return lpUnzFileInfo ? lpUnzFileInfo->crc : 0;
}

uLong PackReader::GetFileSize(const char* lpszFileName)
{
	std::string sFileName = lpszFileName;
	for (size_t nChar = 0; nChar < sFileName.size(); ++nChar) {
		if (sFileName[nChar] == '\\')
			sFileName[nChar] = '/';
	}
	lpszFileName = _strupr(const_cast<char*>(sFileName.c_str()));

	TSEEKER::iterator anItr;
	anItr = m_Seeker.find(lpszFileName);
	if (anItr != m_Seeker.end()) {
		return anItr->second.fileSize;
	}
	return 0;
}

uLong PackReader::ReadFile(const char* lpszFileName, void* lpReadData, uLong nReadSize)
{
	if (0 == m_UnzFile)
		return 0;

	if (0 == lpszFileName || 0 == *lpszFileName)
		return 0;

	if (0 == lpReadData || 0 == nReadSize)
		return 0;

	const char*	lpszPackCode = (0 == m_szCode[0] ? NULL : m_szCode);

	char szFileName[512];
	strcpy(szFileName,lpszFileName);
	for (size_t nChar = 0; szFileName[nChar]; ++nChar) {
		if (szFileName[nChar] == '\\')
			szFileName[nChar] = '/';
	}
	lpszFileName = _strupr(szFileName);

	unzFile hZipFile = m_UnzFile;
	int nResult = UNZ_ERRNO;

	TSEEKER::iterator anItr;
	anItr = m_Seeker.find(lpszFileName);
	if (anItr != m_Seeker.end()) {
		nResult = unzSetOffset(hZipFile, (uLong)anItr->second.seekPos);
	}

	if (UNZ_OK != nResult) {
		nResult = unzGoToFirstFile(hZipFile);
	}

	while (UNZ_OK == nResult) {
		char szCurrentFile[512];
		unz_file_info _UnzFileInfo;
		unzGetCurrentFileInfo(hZipFile, &_UnzFileInfo, szCurrentFile, sizeof(szCurrentFile), NULL, 0, NULL, 0);
		for (size_t nChar = 0; szCurrentFile[nChar]; ++nChar) {
			if (szCurrentFile[nChar] == '\\')
				szCurrentFile[nChar] = '/';
		}
		const char* lpszCurrentFile = _strupr(szCurrentFile);

		if (0 == strcmp(lpszCurrentFile, lpszFileName)) {
			if (UNZ_OK == unzOpenCurrentFilePassword(hZipFile, lpszPackCode)) {
				if (nReadSize > _UnzFileInfo.uncompressed_size)
					nReadSize = _UnzFileInfo.uncompressed_size;

				int nReadBytes = unzReadCurrentFile(hZipFile, lpReadData, nReadSize);

				unzCloseCurrentFile(hZipFile);

				if (nReadBytes == nReadSize)
					return nReadSize;
			}
			return 0;
		}

		nResult = unzGoToNextFile(hZipFile);
	}

	return 0;
}

unz_file_info* PackReader::GetUnzFileInfo(const char* lpszFileName)
{
	if (0 == m_UnzFile)
		return NULL;

	if (0 == lpszFileName || 0 == *lpszFileName)
		return NULL;

	const char*	lpszPackCode = ('\0' == m_szCode[0] ? NULL : m_szCode);

	char szFileName[512];
	strcpy(szFileName, lpszFileName);
	for (size_t nChar = 0; szFileName[nChar]; ++nChar) {
		if (szFileName[nChar] == '\\')
			szFileName[nChar] = '/';
	}
	lpszFileName = _strupr(szFileName);

	unzFile hZipFile = m_UnzFile;

	int nResult = UNZ_ERRNO;
	if (0 != m_Seeker.size()) {
		TSEEKER::iterator anItr;
		anItr = m_Seeker.find(lpszFileName);
		if (anItr != m_Seeker.end()) {
			nResult = unzSetOffset(hZipFile, (uLong)anItr->second.seekPos);
		}
	}

	if (UNZ_OK != nResult) {
		nResult = unzGoToFirstFile(hZipFile);
	}

	while (UNZ_OK == nResult) {
		char szCurrentFile[512];
		unz_file_info _UnzFileInfo;
		unzGetCurrentFileInfo(hZipFile, &_UnzFileInfo, szCurrentFile, 260, NULL, 0, NULL, 0);
		for (size_t nChar = 0; szCurrentFile[nChar]; ++nChar) {
			if (szCurrentFile[nChar] == '\\')
				szCurrentFile[nChar] = '/';
		}
		const char* lpszCurrentFile = _strupr(szCurrentFile);

		if (0 == strcmp(lpszFileName, lpszCurrentFile)) {
			if (UNZ_OK != unzOpenCurrentFilePassword(hZipFile, lpszPackCode)) {
				return NULL;
			}

			unzCloseCurrentFile(hZipFile);

			memcpy(&m_UnzFileInfo, &_UnzFileInfo, sizeof(_UnzFileInfo));
			return &m_UnzFileInfo;
		}

		nResult = unzGoToNextFile(hZipFile);
	}

	return NULL;
}

	void PackReader::cache() {
		unzFile hFile = m_UnzFile;
		int result = unzGoToFirstFile(hFile);

		while(result == UNZ_OK) {
			unz_file_info info;
			char szCurrentFile[512];
			unzGetCurrentFileInfo(hFile, &info, szCurrentFile, 512, NULL, 0, NULL, 0);
			for (size_t nChar = 0; szCurrentFile[nChar]; ++nChar) {
				if (szCurrentFile[nChar] == '\\')
					szCurrentFile[nChar] = '/';
			}
			const char* lpszCurrentFile = _strupr(szCurrentFile);

			uLong nSeekPos = unzGetOffset(hFile);
			m_Seeker.insert(TSEEKER::value_type(szCurrentFile, fileInfo(lpszCurrentFile, info.uncompressed_size, nSeekPos)));

			result = unzGoToNextFile(hFile);
		}
	}

/* Extended by GriffinBu */
	void  PackReader::GetFileList(std::vector<std::string>& vfiles) {
		unzFile hFile = m_UnzFile;
		int result = unzGoToFirstFile(hFile);

		while(result == UNZ_OK) {
			unz_file_info info;
			char szCurrentFile[512];
			unzGetCurrentFileInfo(hFile, &info, szCurrentFile, 512, NULL, 0, NULL, 0);
			vfiles.push_back(szCurrentFile);

			result = unzGoToNextFile(hFile);
		}
	}
};//namespace BOGY
