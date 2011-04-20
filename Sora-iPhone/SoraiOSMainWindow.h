/*
 *  SoraiOSMainWindow.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio Symphonie. All rights reserved.
 *
 */
#ifndef SORA_IOS_MAIN_WINDOW_H_
#define SORA_IOS_MAIN_WINDOW_H_

#include "SoraCore.h"
#include "SoraiOSDeviceHelper.h"

#include "SoraZipResourceManager/SoraZipResourceManager.h"
#include "SoraFileUtility.h"

class SoraiOSMainWindow: public sora::SoraWindowInfoBase {
public:
	SoraiOSMainWindow(): bScreenRotated(false) {}
	
	virtual bool updateFunc() {
		return false;
	}
	
	virtual bool renderFunc() {
		sora::SORA->beginScene();
		pBG->render();
	    if(sprTest) sprTest->render(0.f, 0.f);
		//sprm->render(0.f, 0.f, 0.f);
		sora::SORA->endScene();
		return false;
	}
	
	void init() {
		sora::SORA->registerResourceManager(new sora::SoraZipResourceManager);
		sora::SORA->attachResourcePack(sora::SORA->loadResourcePack(sora::SoraFileUtility::getApplicationPath()+L"data.rfResource"));
		
		printf("%s\n", sora::ws2s(iOSGetResourceName(L"magicCircle.png")).c_str());
		sprTest = sora::SORA->createSprite(L"magicCircle.png");
		sprTest->setScale(0.5f, 0.5f);
		pBG = sora::SORA->createSprite(L"background1.png");
		pBG->setScale(pBG->getSpriteWidth()/getWindowWidth(), pBG->getSpriteHeight()/getWindowHeight());
	}
	
	int32 getWindowWidth() { return iOSGetScreenWidth(bScreenRotated); }
	int32 getWindowHeight() { return iOSGetScreenHeight(bScreenRotated); }
	
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
	sora::SoraSprite* sprTest;
	sora::SoraSprite* pBG;
};

#endif
