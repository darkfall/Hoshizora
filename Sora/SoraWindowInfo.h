#ifndef SORA_WINDOW_INFO_H
#define SORA_WINDOW_INFO_H

#include "SoraPlatform.h"
#include "SoraException.h"

namespace sora {

	typedef ulong32 SoraWindowHandle;

	class SoraWindowInfoBase {
	public:
		virtual int32 getWindowWidth() = 0;
		virtual int32 getWindowHeight() = 0;
		
		virtual int32 getWindowPosX() = 0;
		virtual int32 getWindowPosY() = 0;
		
		virtual SoraString getWindowName() = 0;
		virtual SoraString getWindowId() = 0;
		
		virtual void init() = 0;
		
		virtual bool isWindowed() = 0;
		virtual bool hideMouse() = 0;

		virtual bool renderFunc() = 0;
		virtual bool updateFunc() = 0;
	};
	
} // namespace sora
	

#endif