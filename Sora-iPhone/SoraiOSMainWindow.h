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
#include "SoraiOSFontManager/SoraiOSFontManager.h"

#include "SoraFont.h"

#include "SoraGUIChan/guichansetup.h"

#include "gcnExtend/gcnDraggableImageButtonIncubator.h"
#include "gcnExtend/gcnSelectableContainer.h"

class SoraiOSMainWindow: public sora::SoraWindowInfoBase {
public:
	SoraiOSMainWindow(): bScreenRotated(false) {}
	
	virtual bool updateFunc() {

		return false;
	}
	
	virtual bool renderFunc() {
		sora::SORA->beginScene();
	
		sora::GCN_GLOBAL->gcnLogic();

		sora::GCN_GLOBAL->gcnDraw();
		//sprm->render(0.f, 0.f, 0.f);
		sora::SORA->endScene();
		
		return false;
	}
	
	void init() {
		sora::SORA->registerResourceManager(new sora::SoraZipResourceManager);
		sora::SORA->registerFontManager(new sora::SoraiOSFontManager);
		sora::SORA->registerInput(new sora::SoraiOSInput);
		
		sora::SORA->attachResourcePack(sora::SORA->loadResourcePack(sora::SoraFileUtility::getApplicationPath()+L"data.rfResource"));
		
		sora::GCN_GLOBAL->initGUIChan(L"STHeitiJ-Medium", 20);
		sora::GCN_GLOBAL->createTop();
		
		
		gcn::SelectableContainer* sc = new gcn::SelectableContainer;
		
		sc->setDimension(gcn::Rectangle(0, 0, 512, 512));
		sc->setImage("background1.png");
		sc->adjustSize();
		sc->setId("canvas");
		sora::GCN_GLOBAL->addWidget(sc, "top");
		
		gcn::DraggableImageButtonIncubator* di = new gcn::DraggableImageButtonIncubator();
		di->setImage("boxbutton.png", gcn::Rectangle(0, 64, 64, 64), gcn::Rectangle(0, 64, 64, 64)); 
		di->setNewItemImage("boxbutton.png", gcn::Rectangle(0, 0, 64, 64), gcn::Rectangle(64, 0, 64, 64));
		di->setBaseColor(gcn::Color(255, 255, 255, 155));
		
		sora::GCN_GLOBAL->addWidget(di, "canvas");
		
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
	sora::SoraFont* pFont;
	sora::SoraFont* pFont2;
	ulong32 fbo;
};

#endif
