#ifndef SORA_MISCTOOL_WIN32
#define SORA_MISCTOOL_WIN32

#include <windows.h>

#include "Defaults\SoraDefaultMiscTool.h"

#include "SoraStringConv.h"

namespace sora {

class SoraMiscToolWin32: public SoraDefaultMiscTool {
public:
	SoraMiscToolWin32(): _hWnd(0) { }

	void messageBox(const SoraString& sMessage, const SoraString& sTitle, int32 msgCode) {
		MessageBoxW(_hWnd, s2ws(sMessage.c_str()).c_str(), s2ws(sTitle.c_str()).c_str(), msgCode);
	}
	void messageBox(const SoraWString& sMessage, const SoraWString& sTitle, int32 msgCode) { 
		MessageBoxW(_hWnd, sMessage.c_str(), sTitle.c_str(), msgCode);
	}

	void setMainWindowHandle(ulong32 mainWindowHandle) { _hWnd = (HWND)mainWindowHandle; }

private:
	HWND _hWnd;
};

} // namespace sora

#endif // SORA_MISCTOOL_WIN32