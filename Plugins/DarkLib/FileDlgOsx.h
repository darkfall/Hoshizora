/*
 *  FileDlgOsx.h
 *  Sora
 *
 *  Created by Griffin Bu on 3/4/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef FILE_DLG_OSX_H_
#define FILE_DLG_OSX_H_

#include <string>
	
#ifdef __APPLE_CC__

#ifndef ulong32
#define ulong32 unsigned long
#endif
	
	/*
	 File Open/Save Dialog for OSX
	 @param hWnd is no use, just for compability with HWND in win32 version
	 */
	
	class FileDlg {
	public:
		FileDlg(ulong32 hWnd=0);
		
		bool FileOpenDlg(ulong32 hWnd, char* pstrFileName, char* pstrTitleName);
		bool FileSaveDlg(ulong32 hWnd, char* pstrFileName, char* pstrTitleName);
		
		/*
		 Filters should be delimed by ;
		 txt;doc; etc
		 */
		void SetFilter(const char* pstrFilter);
		void SetDefaultExt(const char* pstrExt);
		void SetDefaultPath(const char* pstrPath);
		
	private:
		std::string filter;
		std::string ext;
		std::string path;
	};


#endif
		
#endif