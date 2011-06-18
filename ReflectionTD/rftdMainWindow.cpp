/*
 *  rftdrftdMainWindow.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/14/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "rftdMainWindow.h"

#include "SoraEventManager.h"
#include "SoraGUIChan/guichansetup.h"

#include "SoraSpriteAnimation/AnimationLuaExport.h"
#include "SoraSoundManager/SoundManagerLuaExport.h"


#include "rftdCommon.h"
#include "rftdSystem.h"

namespace rftd {

	rftdMainWindow::rftdMainWindow() {
		sora = sora::SoraCore::Instance();
		
		registerEventFunc(this, &rftdMainWindow::onKeyEvent);
		sora::SORA_EVENT_MANAGER->registerInputEventHandler(this);
	}
	
	rftdMainWindow::~rftdMainWindow() {
	}
	
	bool rftdMainWindow::updateFunc() {
		
		if(sora->keyDown(SORA_KEY_ESCAPE))
			sora->shutDown();
		if(sora->keyDown(SORA_KEY_F)) {
			
		}
		
		return false;
	}	
	bool rftdMainWindow::renderFunc() {
		
		sora->beginScene(0x00000000);
		sora::GCN_GLOBAL->gcnLogic();
		sora::GCN_GLOBAL->gcnDraw();
		
		pFont->print(getWindowWidth(), 0.f, sora::FONT_ALIGNMENT_RIGHT, L"%.2f/%.2f", mtmap->getCurrLevelInterval(), mtmap->getLevelInterval());
		pFont->print(getWindowWidth(), 20.f, sora::FONT_ALIGNMENT_RIGHT, L"level: %u", mtmap->getCurrLevel());
		pFont->print(getWindowWidth(), 40.f, sora::FONT_ALIGNMENT_RIGHT, L"enemy: %d", mtmap->getEnemySize());
		pFont->render(getWindowWidth(), 60.f, sora::FONT_ALIGNMENT_RIGHT, mtmap->getCurrLevelName().c_str());
		pFont->render(getWindowWidth(), 80.f, sora::FONT_ALIGNMENT_RIGHT, conf.mapName.c_str());
		
		sora->endScene();
		return false;
	}
	
	void rftdMainWindow::init() {
		sora::SORA->setFPS(60);
		
		pFont = sora::SORA->createFont(L"Bank Gothic Medium BT.ttf", 20);
		pFont->setColor(0xFFFFCC00);
		
		if(sora::GCN_GLOBAL->initGUIChan(L"Bank Gothic Medium BT.ttf", 20) ) {
			sora::GCN_GLOBAL->createTop();
		}
		
		rftd::rftdInitialize();
		
		conf = rftd::GET_MAP(L"MyMapPack1", 0);
		mtmap = new rftd::rftdMap;
		mtmap->readMapConf(conf.mapPath);
		mtmap->start();
		sora::GCN_GLOBAL->addWidget(mtmap, "top");
	}
	
	void rftdMainWindow::onKeyEvent(sora::SoraKeyEvent* kev) {
		if(kev->type == SORA_INPUT_KEYDOWN) {
			if(kev->key == SORA_KEY_1 ) {
				
			}
		}
	}

} // namespace rftd