#ifndef SORA_WINDOW_INFO_H
#define SORA_WINDOW_INFO_H

#include "SoraPlatform.h"
#include "SoraEvent.h"
#include "SoraException.h"

namespace sora {

	typedef ulong32 SoraWindowHandle;

	class SORA_API SoraWindowInfoBase: public SoraEventHandler {
	public:
        SoraWindowInfoBase(): mActive(true) {}
        
		virtual int32 getWindowWidth() = 0;
		virtual int32 getWindowHeight() = 0;
		
		virtual int32 getWindowPosX() = 0;
		virtual int32 getWindowPosY() = 0;
		
		virtual util::String getWindowName() = 0;
		virtual util::String getWindowId() = 0;
		
		virtual void init() = 0;
		
		virtual bool isWindowed() = 0;
		virtual bool hideMouse() = 0;

		virtual bool renderFunc() = 0;
		virtual bool updateFunc() = 0;

		virtual const char* getIcon() { return NULL; }
		virtual const char* getCursor() { return NULL; }

		virtual void pauseFunc() {}
        
        virtual void reinit() {}
        
        bool isActive() const { return mActive; }
        
        virtual bool isConsoleApp() { return false; }
        
    private:
        friend class SoraCore;
        void setActive(bool flag) { mActive = flag; }
        
        bool mActive;
	};
	
} // namespace sora
	

#endif