#ifndef SORA_MISCTOOL_WIN32
#define SORA_MISCTOOL_WIN32

#include "SoraDefaultMiscTool.h"
#include "SoraStringConv.h"
#include "SoraPlatform.h"

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

private:
	HWND _hWnd;
};

} // namespace sora

#endif // OS_WIN32

#endif // SORA_MISCTOOL_WIN32