#include "FileDlg.h"
#include <Windows.h>
FileDlg::FileDlg(HWND hwnd)
{
	 //memset(&ofn, 0, sizeof(OPENFILENAME));
     ofn.lStructSize       = sizeof(OPENFILENAME);
     ofn.hwndOwner         = hwnd;
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
}

BOOL FileDlg::FileOpenDlg(HWND hwnd, TCHAR *pstrFileName, TCHAR *pstrTitleName)
{
//	memset(&ofn, 0, sizeof(OPENFILENAME));

     ofn.hwndOwner         = hwnd ;
     ofn.lpstrFile         = pstrFileName ;
     ofn.lpstrFileTitle    = pstrTitleName ;
     ofn.Flags             = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST ;
	 ofn.lpstrInitialDir = initpath.c_str();
	 ofn.lpstrDefExt = ext.c_str();
   //  ofn.lpstrFileTitle = L"test\0\0";

     return GetOpenFileName (&ofn) ;
}

BOOL FileDlg::FileSaveDlg(HWND hwnd, TCHAR *pstrFileName, TCHAR *pstrTitleName)
{
//	memset(&ofn, 0, sizeof(OPENFILENAME));
    ofn.hwndOwner         = hwnd ;
    ofn.lpstrFile         = pstrFileName ;
    ofn.lpstrFileTitle    = pstrTitleName ;
    ofn.Flags             = OFN_OVERWRITEPROMPT ;
	ofn.lpstrInitialDir = initpath.c_str();
	ofn.lpstrDefExt = ext.c_str();
	 
    return GetSaveFileName (&ofn) ;
}