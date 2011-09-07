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
		
		inline void SoraiOSInit(bool isOGLES2API) {
            if(isOGLES2API)
                SORA->registerRenderSystem(new SoraiOSGLRenderer_ES2);
            else
                SORA->registerRenderSystem(new SoraiOSGLRenderer);
            
            mVerticalSync = false;
		//	input = new SoraiOSInput;
		//	SORA->registerInput(input);
		}
		
        SoraiOSInitializer() {}
        ~SoraiOSInitializer() {}
        
	public:
		
		inline void SoraiOSStart(SoraWindowInfoBase* window, bool isOGLES2API=false) {
			SoraiOSInit(isOGLES2API);
			
			try {
                SORA->registerFontManager(new sora::SoraiOSFontManager);
                SORA->registerInput(new sora::SoraiOSInput);
                
				SORA->createWindow(window);
				SORA->setFPS(60);
				SORA->start();
			} catch(SoraException& e) {
				printf("EXCEPTION %s\n", e.what());
			}
		}
		
		void setTimer(SoraTimer* timer) { pTimer = timer; }
		
        void setVerticalSync(bool flag) {
            mVerticalSync = flag;
        }
        
		inline bool update() {
            if(mVerticalSync)
                return true;
            else if(pTimer)
				return pTimer->update();
			return false;
		}
		
		inline void SoraiOSUpdateSystems() {
			SORA->update();
		}
		
		inline void SoraiOSShutDown() {
			SORA->shutDown();
		}
		
		
	private:
		SoraTimer* pTimer;
		SoraiOSInput* input;
        
        bool mVerticalSync;
	};

	
	static SoraiOSInitializer* SORA_IOS = SoraiOSInitializer::Instance();
	
#define GET_IOS_RESOURCE_NAME(name) SoraiOSInitializer::getResourceName(name)

} // namespace sora

#endif // SORA_IPHONE_DELEGATE_H_