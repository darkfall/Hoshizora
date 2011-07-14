/*
 *  FileDlgOsx.mm
 *  Sora
 *
 *  Created by Griffin Bu on 3/4/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "FileDlgOsx.h"

#ifdef __APPLE_CC__

#import <Cocoa/Cocoa.h>	
	
	FileDlg::FileDlg(ulong32 hWnd) {
		ext = "*;";
	}
		
	bool FileDlg::FileOpenDlg(ulong32 hWnd, char* pstrFileName, char* pstrTitleName) {
		assert(pstrFileName != NULL);
		assert(pstrTitleName != NULL);
		
		NSOpenPanel* openDlg = [NSOpenPanel openPanel];
		
		[openDlg setAllowsMultipleSelection:NO];
		
		NSMutableArray* fileTypes = [NSMutableArray arrayWithObject:@"*"];
		size_t prevPos = 0;
		for(size_t i=0; i<filter.size(); ++i) {
			if(filter[i] == ';') {
				[fileTypes addObject: [[NSString alloc] initWithUTF8String: filter.substr(prevPos, i).c_str()]];
				prevPos = i+1;
				
			//	printf("%s\n", filter.substr(prevPos, i).c_str());

			}
		}
		
		NSString* filePath = [[NSString alloc] initWithUTF8String: path.c_str()];
		int result = [openDlg runModalForDirectory:filePath
											 file:nil 
											 types:fileTypes];

		if (result == NSOKButton) {
			NSArray *filesToOpen = [openDlg filenames];
			int count = [filesToOpen count];
			if(count > 0) {
				std::string fileName = [[filesToOpen objectAtIndex:0] UTF8String];
				strcpy(pstrFileName, fileName.c_str());
				size_t deliPos = fileName.rfind('/');
				strcpy(pstrTitleName, deliPos!=std::string::npos?fileName.substr(deliPos+1, fileName.size()).c_str():fileName.c_str());
				
			//	printf("%s, %s\n", pstrFileName, pstrTitleName);
			}
            
            [filePath release];
			return true;
		}
		return false;
	}
	
	bool FileDlg::FileSaveDlg(ulong32 hWnd, char* pstrFileName, char* pstrTitleName) {
		assert(pstrFileName != NULL);
		assert(pstrTitleName != NULL);
		
		NSSavePanel* saveDlg = [NSSavePanel savePanel];
		
		//[sp setAccessoryView:newView];
		NSString* fileExt = [[NSString alloc] initWithUTF8String: ext.c_str()];
		[saveDlg setRequiredFileType:fileExt];
		
		NSString* filePath = [[NSString alloc] initWithUTF8String: path.c_str()];
		int result = [saveDlg runModalForDirectory:filePath
											 file:nil];
		
		[fileExt release];
		[filePath release];
		if (result == NSOKButton) {
			std::string fileName = [[saveDlg filename] UTF8String];
			strcpy(pstrFileName, fileName.c_str());
			size_t deliPos = fileName.rfind('/');
			strcpy(pstrTitleName, deliPos!=std::string::npos?fileName.substr(deliPos, fileName.size()).c_str():fileName.c_str());
			
			return true;
		}
		return false;
	}
		
	void FileDlg::SetFilter(const char* pstrFilter) {
		filter = pstrFilter;
	}
	
	void FileDlg::SetDefaultExt(const char* pstrExt) {
		ext = pstrExt;
	}
	
	void FileDlg::SetDefaultPath(const char* pstrPath) {
		path = pstrPath;
	}
	
#endif
	