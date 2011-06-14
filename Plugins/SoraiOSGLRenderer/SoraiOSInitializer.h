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
#include "SoraiOSInput.h"

namespace sora {
	
	class SoraiOSInitializer: public SoraSingleton<SoraiOSInitializer> {
		friend class SoraSingleton<SoraiOSInitializer>;
		
		inline void SoraiOSInit() {
			SORA->registerRenderSystem(new SoraiOSGLRenderer);
				
		//	input = new SoraiOSInput;
		//	SORA->registerInput(input);
		}
		
	public:
		
		inline void SoraiOSStart(SoraWindowInfoBase* window) {
			SoraiOSInit();
			
			try {
				SORA->createWindow(window);
				SORA->setFPS(60);
				SORA->start();
			} catch(SoraException& e) {
				printf("EXCEPTION %s\n", e.what().c_str());
			}
		}
		
		void setTimer(SoraTimer* timer) { pTimer = timer; }
		
		inline bool update() {
			if(pTimer)
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
	};

	
	static SoraiOSInitializer* SORA_IOS = SoraiOSInitializer::Instance();
	
#define GET_IOS_RESOURCE_NAME(name) SoraiOSInitializer::getResourceName(name)

} // namespace sora

#endif // SORA_IPHONE_DELEGATE_H_