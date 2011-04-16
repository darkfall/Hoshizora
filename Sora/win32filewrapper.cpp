/*
 *  win32filewrapper.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/27/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "win32filewrapper.h"

#ifdef WIN32

namespace sorawin32 {

void enumFilesInFolder(std::vector<SoraWString>& cont, const SoraWString& folder) {
		WIN32_FIND_DATAW FindData;
		SoraWString newFolder = folder+L"\\*";
	
		HANDLE hFile = FindFirstFileW(newFolder.c_str(), &FindData);
		if(hFile == INVALID_HANDLE_VALUE)
			return;

		SoraWString tmpPath;
		SoraWString fName = FindData.cFileName;
		if(fName==L"."||fName==L".." )
			tmpPath = L"nop";
		else if((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			       tmpPath = folder;
				   tmpPath += L'/';
				   tmpPath += FindData.cFileName;

              enumFilesInFolder(cont, tmpPath);    //如果是文件夹，第归调用扫描
		}
		else {
			cont.push_back(folder+L"/"+fName);
		}

		BOOL FindOK = (INVALID_HANDLE_VALUE != hFile);  
	    while(FindOK) {  
			FindOK = FindNextFileW(hFile, &FindData);  
		    if(FindOK) {
				fName = FindData.cFileName;
				if(fName==L"."||fName==L"..")
					continue;
			    else if((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					    tmpPath = folder;
						tmpPath += L'/';
						tmpPath += FindData.cFileName;

						enumFilesInFolder(cont, tmpPath);    //如果是文件夹，第归调用扫描
				}
				else {
					cont.push_back(folder+L"/"+fName);
				}	
			}
		}
		FindClose(hFile);
}

} // namespace sorawin32

#endif