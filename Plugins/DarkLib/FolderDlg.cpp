#include "FolderDlg.h"

int CALLBACK BrowseProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    if(uMsg==BFFM_SELCHANGED||uMsg==BFFM_INITIALIZED) {
        if(uMsg==BFFM_INITIALIZED) {
            SendMessage(hwnd,BFFM_SETSELECTION,TRUE,
                LPARAM(lpData));
        }
    }
    return 0;
}

bool FolderDlg::OpenFolder(HWND hWnd, TCHAR *szTitle)
{
	BROWSEINFO bi={0};
    bi.hwndOwner = hWnd;
    bi.pidlRoot  = NULL;
    bi.lpszTitle = szTitle;
    bi.ulFlags   = BIF_RETURNONLYFSDIRS | BIF_EDITBOX;
    bi.lpfn      = BrowseProc; 
    bi.lParam    = (LPARAM)tmpPath;
    
    LPITEMIDLIST pIdl;
    if((pIdl = SHBrowseForFolder(&bi)) != NULL) {
        TCHAR pbuffer[MAX_PATH];  
        SHGetPathFromIDList(pIdl, pbuffer);
		wcscpy(path, pbuffer);
		wcscpy(tmpPath, pbuffer);
		return true;
    }
	return false;
}
