#ifndef FILEDLG_H
#define FILEDLG_H

#define MAX_FILTER 1024

#include <windows.h>
#include <commdlg.h>

#pragma comment(lib, "comdlg32.lib")

#include <string>
using std::wstring;
/*
Library Dark
Open&Save File Dialog Class
文件对话框类
Darklib，A Windows API C++ Wrapper library, belongs to USEDefender, Copywrite darkfall 2007-2008
*/

class FileDlg
{
public:
    FileDlg(HWND hWnd = 0);

	/*
	FileName返回打开文件的完整路径
	TitleName返回打开文件的标题
	*/
    BOOL FileOpenDlg(HWND hWnd, TCHAR *pstrFileName, TCHAR *pstrTitleName);
    BOOL FileSaveDlg(HWND hWnd, TCHAR *pstrFileName, TCHAR *pstrTitleName);

	/*
	设置过滤器，过滤器格式形如TEXT("说明\0过滤格式\0
	                                  说明\0过滤格式\0
									  ..............\0\0");
				    过滤格式的几个格式之间以;隔开
					如TEXT("魔兽地图(*.w3x,*.w3m)\0*.w3x;*.w3m\0\0");
	*/
	void SetFilter(const TCHAR* f) { ofn.lpstrFilter = f; }

	/*
	设置默认的文件后缀，当保存文件时如果没有输入文件格式，则采用此格式保存
	    格式如TEXT(".w3x")
	*/
	void SetDefaultExt(const TCHAR *f) { ext = f;}

	void SetDefaultPath(const TCHAR* f) { initpath = f; }

private:
	OPENFILENAME ofn;

	wchar_t filter[512];
	wstring ext;
	wstring initpath;
};


#endif