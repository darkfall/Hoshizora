/*
 *  SoraiOSMainWindow.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */
#ifndef SORA_IOS_TEST_MAIN_WINDOW_H
#define SORA_IOS_TEST_MAIN_WINDOW_H

#include "SoraCore.h"
#include "SoraiOSDeviceHelper.h"

#include "SoraiOSGLRenderer/SoraiOSMainWindow.h"

#include "SoraFileUtility.h"
#include "SoraiOSFontManager/SoraiOSFontManager.h"

#include "SoraFont.h"

#include "SoraGUIChan/guichansetup.h"

#include "gcnExtend/gcnDraggableImageButtonIncubator.h"
#include "gcnExtend/gcnSelectableContainer.h"

class mainwindow: public sora::SoraiOSMainWindow {
public:
	mainwindow(): bScreenRotated(false) {}
	
	virtual bool updateFunc() {
		return false;
	}
	
	virtual bool renderFunc() {
		sora::SoraCore::Ptr->beginScene();
        pBG->render(0.f, 0.f);

		sora::GCN_GLOBAL->gcnLogic();

		sora::GCN_GLOBAL->gcnDraw();
        
		if(pFont)
            pFont->print(0.f, getWindowHeight()-20, sora::SoraFont::AlignmentLeft, L"FPS: %.2f", sora::SoraCore::Ptr->getFPS());
		sora::SoraCore::Ptr->endScene();
		
		return false;
	}
	
	void init() {
		sora::SoraCore::Ptr->loadResourcePack(sora::SoraFileUtility::getApplicationPath()+L"data.rfResource");
		
        pBG = sora::SoraCore::Ptr->createSprite(L"background1.png");

		sora::GCN_GLOBAL->initGUIChan(L"STHeitiJ-Medium", 20);
		sora::GCN_GLOBAL->createTop();
		
		pFont = sora::SoraCore::Ptr->createFont(L"STHeitiJ-Medium", 20);
		
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
		
		sc->add(di);
		
        printf("%s\n", sora::iOSDeviceTypeToString(sora::getiOSDeviceType()).c_str());
	}
	
	
private:
	bool bScreenRotated;
	sora::SoraSprite* sprTest;
	sora::SoraSprite* pBG;
	sora::SoraFont* pFont;
	sora::SoraFont* pFont2;
	ulong32 fbo;
};

#endif
