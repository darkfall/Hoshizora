#ifndef FOLDERDLG_H
#define FOLDERDLG_H

#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>

/*
Library Dark
Open Folder Dialog Class
�ļ��жԻ�����
*/

class FolderDlg
{
public:
	//���캯��
	FolderDlg() {tmpPath[0] = '\0'; path[0] = '\0';}

	//���ļ���
    bool OpenFolder(HWND hWnd, TCHAR *szTitle);

	//��ȡ�Ի�����ѡ����ļ���·��
	TCHAR *GetFolderPath() {return path;}

private:
	TCHAR path[MAX_PATH];
	TCHAR tmpPath[MAX_PATH];
};

#endif