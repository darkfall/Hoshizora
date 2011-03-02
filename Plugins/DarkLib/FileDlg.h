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
�ļ��Ի�����
Darklib��A Windows API C++ Wrapper library, belongs to USEDefender, Copywrite darkfall 2007-2008
*/

class FileDlg
{
public:
    FileDlg(HWND hWnd = 0);

	/*
	FileName���ش��ļ�������·��
	TitleName���ش��ļ��ı���
	*/
    BOOL FileOpenDlg(HWND hWnd, TCHAR *pstrFileName, TCHAR *pstrTitleName);
    BOOL FileSaveDlg(HWND hWnd, TCHAR *pstrFileName, TCHAR *pstrTitleName);

	/*
	���ù���������������ʽ����TEXT("˵��\0���˸�ʽ\0
	                                  ˵��\0���˸�ʽ\0
									  ..............\0\0");
				    ���˸�ʽ�ļ�����ʽ֮����;����
					��TEXT("ħ�޵�ͼ(*.w3x,*.w3m)\0*.w3x;*.w3m\0\0");
	*/
	void SetFilter(const TCHAR* f) { ofn.lpstrFilter = f; }

	/*
	����Ĭ�ϵ��ļ���׺���������ļ�ʱ���û�������ļ���ʽ������ô˸�ʽ����
	    ��ʽ��TEXT(".w3x")
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