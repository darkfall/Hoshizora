//////////////////////////////////////////////////////////////////////////////////////
// 
// Copyright(C) 2009 BOGY.CN
//
// Author: Î¢ÃîµÄÆ½ºâ
// Mailto: bogy.cn@gmail.com
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef PACK_READER_H_
#define PACK_READER_H_

#include "PackMemory.h"
#include <string>
#include <map>
#include <vector>

// modified and optimizations by Griffin Bu

namespace BOGY
{

class PackReader
{
public:
	PackReader(void);
	~PackReader(void);

public:
	bool			Open(const char* lpszPackPath, const char* lpszPackCode);
	bool			From(void* lpszPackData, uLong nPackSize, const char* lpszPackCode);
	bool			Close();

	uLong			ReadFile(const char* lpszFileName, void* lpReadData, uLong nReadSize); 

	uLong			GetFileCRC(const char* lpszFileName); 
	uLong			GetFileSize(const char* lpszFileName);
	
	/* Extended by GriffinBu */
	void  GetFileList(std::vector<std::string>&);

private:
	void cache();
	
	unz_file_info*	GetUnzFileInfo(const char* lpszFileName);
	unz_file_info	m_UnzFileInfo;	

private:
	unzFile m_UnzFile;		// modeRead
	char m_szCode[260];

	struct fileInfo {
		std::string fileName;
		uLong fileSize;
		uLong seekPos;
		
		fileInfo(const std::string& _fileName, uLong _fileSize, uLong _seekPos): fileName(_fileName), fileSize(_fileSize), seekPos(_seekPos) {}
	};
	
	typedef std::map<std::string, fileInfo> TSEEKER; 
	TSEEKER m_Seeker;
};

};//namespace BOGY

#endif