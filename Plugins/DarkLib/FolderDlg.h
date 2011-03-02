#ifndef FOLDERDLG_H
#define FOLDERDLG_H

#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>

/*
Library Dark
Open Folder Dialog Class
文件夹对话框类
*/

class FolderDlg
{
public:
	//构造函数
	FolderDlg() {tmpPath[0] = '\0'; path[0] = '\0';}

	//打开文件夹
    bool OpenFolder(HWND hWnd, TCHAR *szTitle);

	//获取对话框内选择的文件夹路径
	TCHAR *GetFolderPath() {return path;}

private:
	TCHAR path[MAX_PATH];
	TCHAR tmpPath[MAX_PATH];
};

#endif