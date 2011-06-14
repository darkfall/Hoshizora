#ifndef SORA_MISCTOOL_WIN32
#define SORA_MISCTOOL_WIN32

#include "../SoraStringConv.h"
#include "../SoraPlatform.h"
#include "SoraDefaultMiscTool.h"

#ifdef OS_WIN32

#include <windows.h>

namespace sora {

class SoraMiscToolWin32: public SoraDefaultMiscTool {
public:
	SoraMiscToolWin32(): _hWnd(0) { }

	int32 messageBox(const SoraString& sMessage, const SoraString& sTitle, int32 msgCode) {
		return MessageBoxA(_hWnd, sMessage.c_str(), sTitle.c_str(), msgCode);
	}
	int32 messageBox(const SoraWString& sMessage, const SoraWString& sTitle, int32 msgCode) { 
		return MessageBoxW(_hWnd, sMessage.c_str(), sTitle.c_str(), msgCode);
	}

	void setMainWindowHandle(ulong32 mainWindowHandle) { _hWnd = (HWND)mainWindowHandle; }
	
	SoraWString fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL) {
		OPENFILENAMEA ofn;
		 ofn.lStructSize       = sizeof(OPENFILENAME);
		 ofn.hwndOwner         = _hWnd;
		 ofn.hInstance         = NULL;
		 ofn.lpstrFilter       = 0;
		 ofn.lpstrCustomFilter = NULL;
	     ofn.nMaxCustFilter    = 0;
	     ofn.nFilterIndex      = 0;
	     ofn.lpstrFile         = NULL;         
	     ofn.nMaxFile          = MAX_PATH;
	     ofn.lpstrFileTitle    = NULL;         
	     ofn.nMaxFileTitle     = MAX_PATH;
	     ofn.lpstrInitialDir   = NULL;
	     ofn.lpstrTitle        = 0;
	     ofn.Flags             = 0;           
	     ofn.nFileOffset       = 0;
	     ofn.nFileExtension    = 0;
	     ofn.lpstrDefExt       = 0;
	     ofn.lCustData         = 0L;
	     ofn.lpfnHook          = NULL;
	     ofn.lpTemplateName    = NULL;

		 char fileName[512];
		 char titleName[128];
		 fileName[0] = L'\0';
		 titleName[0] = L'\0';
		 ofn.lpstrFile         = fileName ;
		 ofn.lpstrFileTitle    = titleName ;
		 ofn.Flags             = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST ;
		 if(defaultPath)
			 ofn.lpstrInitialDir = defaultPath;
		 ofn.lpstrDefExt = NULL;
		 if(filter)
			 ofn.lpstrFilter = filter;
		 
		 if(GetOpenFileNameA(&ofn))
			return sora::s2ws(fileName);
		 return L"\0";
	}
	
	SoraWString fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL) {
		OPENFILENAMEA ofn;
		 ofn.lStructSize       = sizeof(OPENFILENAME);
		 ofn.hwndOwner         = _hWnd;
		 ofn.hInstance         = NULL;
		 ofn.lpstrFilter       = 0;
		 ofn.lpstrCustomFilter = NULL;
	     ofn.nMaxCustFilter    = 0;
	     ofn.nFilterIndex      = 0;
	     ofn.lpstrFile         = NULL;         
	     ofn.nMaxFile          = MAX_PATH;
	     ofn.lpstrFileTitle    = NULL;         
	     ofn.nMaxFileTitle     = MAX_PATH;
	     ofn.lpstrInitialDir   = NULL;
	     ofn.lpstrTitle        = 0;
	     ofn.Flags             = 0;           
	     ofn.nFileOffset       = 0;
	     ofn.nFileExtension    = 0;
	     ofn.lpstrDefExt       = 0;
	     ofn.lCustData         = 0L;
	     ofn.lpfnHook          = NULL;
	     ofn.lpTemplateName    = NULL;

		 char fileName[512];
		 char titleName[128];
		 fileName[0] = L'\0';
		 titleName[0] = L'\0';
		 ofn.lpstrFile         = fileName ;
		 ofn.lpstrFileTitle    = titleName ;
		 ofn.Flags             = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST ;
		 if(defaultPath)
			 ofn.lpstrInitialDir = defaultPath;
		 if(defaultExt)
			ofn.lpstrDefExt = defaultExt;
		 if(filter)
			 ofn.lpstrFilter = filter;
		 
		 if(GetSaveFileNameA(&ofn))
			 return sora::s2ws(fileName);
		 return L"\0";
	}
	

private:
	HWND _hWnd;
};

} // namespace sora

#endif // OS_WIN32

#endif // SORA_MISCTOOL_WIN32