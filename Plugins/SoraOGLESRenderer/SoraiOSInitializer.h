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

#include "SoraOGLESRenderer.h"
#include "SoraiOSInput.h"

namespace sora {
	
class SoraiOSMainWindow: public SoraWindowInfoBase {
public:
	SoraiOSMainWindow(): bScreenRotated(false) {}
	
	virtual bool updateFunc() {
		return false;
	}
	
	virtual bool renderFunc() {
		SORA->beginScene();
	    if(sprTest) sprTest->render(0.f, 0.f);
		//sprm->render(0.f, 0.f, 0.f);
		SORA->endScene();
		return false;
	}
	
	void init() {
		sprTest = SORA->createSprite(L"magicCircle.png");
		sprTest->setScale(1.f, 1.f);
	}
	
	int32 getWindowWidth() { return bScreenRotated?SORA_IOS_WINDOW_WIDTH:SORA_IOS_WINDOW_WIDTH_ROT; }
	int32 getWindowHeight() { return bScreenRotated?SORA_IOS_WINDOW_HEIGHT:SORA_IOS_WINDOW_HEIGHT_ROT; }
	
	int32 getWindowPosX() { return 0; }
	int32 getWindowPosY() { return 0; }
	
	SoraString getWindowName() { return "HoshiNoSora"; }
	SoraString getWindowId() { return "MainWindow"; }
	
	bool isWindowSubWindow() { return false; }	
	bool isWindowed() { return false; }
	bool hideMouse() { return false; }
	
	void rotScreen(bool flag) { bScreenRotated = flag; }
	
private:
	bool bScreenRotated;
	SoraSprite* sprTest;
};
	
	class SoraiOSInitializer: public SoraSingleton<SoraiOSInitializer> {
		friend class SoraSingleton<SoraiOSInitializer>;
		
		inline void SoraiOSInit() {
			SORA->registerRenderSystem(new SoraOGLESRenderer);
				
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
			return pTimer->update();
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

} // namespace sora

#endif // SORA_IPHONE_DELEGATE_H_