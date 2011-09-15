/*
 *  Sora_iPhoneDelegate.h
 *  Sora-iPhone
 *
 *  Created by GriffinBu on 1/28/11.
 *  Copyright 2011 Project Sora. All rights reserved.
 *
 */

#ifndef SORA_IPHONE_DELEGATE_H_
#define SORA_IPHONE_DELEGATE_H_

#include "SoraCore.h"
#include "Debug/SoraInternalLogger.h"

#include "SoraiOSGLRenderer.h"
#include "SoraiOSGLRenderer_ES2.h"
#include "SoraiOSInput.h"

#include "SoraiOSFontManager/SoraiOSFontManager.h"

namespace sora {
	
	class SoraiOSInitializer: public SoraSingleton<SoraiOSInitializer> {
		friend class SoraSingleton<SoraiOSInitializer>;
		
        void SoraiOSInit(bool isOGLES2API, bool multisampling=false);
		
        SoraiOSInitializer() {}
        ~SoraiOSInitializer() {}
        
	public:
		
        void SoraiOSStart(SoraWindowInfoBase* window, bool isOGLES2API=false);
		void setTimer(SoraTimer* timer);
		void setRenderSystem(SoraiOSGLRenderer* sys);
        
        bool update();
        void SoraiOSUpdateSystems();
        void SoraiOSShutDown();
        void enableMultisampling(bool flag);
        
        int32 getScreenWidth();
        int32 getScreenHeight();
    
        void setOrientation(int portrait);
        int getOrientation() const;
        
        void enableAccelerometer(bool flag);
        void getAccelerometerAttr(float* x, float* y, float* z);
        
	private:
        SoraTimer* pTimer;
		SoraiOSInput* input;
    
        SoraiOSGLRenderer* mRenderSystem;
	};

	static SoraiOSInitializer* SORA_IOS = SoraiOSInitializer::Instance();
	
} // namespace sora

#endif // SORA_IPHONE_DELEGATE_H_