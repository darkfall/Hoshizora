/*
 *  SoraiOSMainWindow.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
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
		sora::SORA->beginScene(0, fbo);
		pBG->render();
	    if(sprTest) sprTest->render(0.f, 0.f);
		//sprm->render(0.f, 0.f, 0.f);
		sora::SORA->endScene();
		
		sora::SORA->beginScene();
		sora::SoraSprite* pp = new sora::SoraSprite(sora::SORA->getTargetTexture(fbo));
		if(pp)
			pp->render();
		sora::SORA->endScene();
		
		return false;
	}
	
	void init() {
		sora::SORA->registerResourceManager(new sora::SoraZipResourceManager);
		sora::SORA->attachResourcePack(sora::SORA->loadResourcePack(sora::SoraFileUtility::getApplicationPath()+L"data.rfResource"));
		
		printf("%s\n", sora::ws2s(sora::iOSGetResourceName(L"magicCircle.png")).c_str());
		sprTest = sora::SORA->createSprite(L"magicCircle.png");
		sprTest->setScale(0.5f, 0.5f);
		pBG = sora::SORA->createSprite(L"background1.png");
		pBG->setScale(pBG->getSpriteWidth()/getWindowWidth(), pBG->getSpriteHeight()/getWindowHeight());
		
		fbo = sora::SORA->createTarget(getWindowWidth(), getWindowHeight());
	}
	
	int32 getWindowWidth() { return sora::iOSGetScreenWidth(bScreenRotated); }
	int32 getWindowHeight() { return sora::iOSGetScreenHeight(bScreenRotated); }
	
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
	ulong32 fbo;
};

#endif
