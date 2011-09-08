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
		
        SoraiOSInitializer(): mVerticalSync(true) {}
        ~SoraiOSInitializer() {}
        
	public:
		
        void SoraiOSStart(SoraWindowInfoBase* window, bool isOGLES2API=false);
		void setTimer(SoraTimer* timer);
		
        void setVerticalSync(bool flag);
        bool update();
        void SoraiOSUpdateSystems();
        void SoraiOSShutDown();
        void enableMultisampling(bool flag);
    
	private:
        
        SoraTimer* pTimer;
		SoraiOSInput* input;
        
        bool mVerticalSync;
	};

	static SoraiOSInitializer* SORA_IOS = SoraiOSInitializer::Instance();
	
#define GET_IOS_RESOURCE_NAME(name) SoraiOSInitializer::getResourceName(name)

} // namespace sora

#endif // SORA_IPHONE_DELEGATE_H_