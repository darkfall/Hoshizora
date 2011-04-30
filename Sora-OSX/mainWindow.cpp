/*
 *  mainWindow.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/29/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "mainWindow.h"
#include "SoraEventManager.h"
#include "SoraLocalizer.h"

#include "graphicEffects.h"

#include "SoraGUIChan/guichansetup.h"
#include "gcnExtend/gcnDraggableImageButtonIncubator.h"
#include "gcnExtend/gcnSelectableContainer.h"
#include "gcnExtend/gcnDraggableIcon.h"
#include "gcnExtend/gcnConnectable.h"


#include "SoraSoundManager/SoraBGMManager.h"

#include "../MEAD/bulletLuaExport.h"
#include "../MEAD/meadBossManager.h"
#include "../MEAD/bulletLuaHelper.h"
#include "SoraLua/SoraLuaExport.h"

mainWindow::mainWindow() {
	sora = sora::SoraCore::Instance();
	
	registerEventFunc(this, &mainWindow::onKeyEvent);
	sora::SORA_EVENT_MANAGER->registerInputEventHandler(this);
}

mainWindow::~mainWindow() {
}

bool mainWindow::updateFunc() {
	
	if(sora->keyDown(SORA_KEY_ESCAPE))
		sora->shutDown();
	if(sora->keyDown(SORA_KEY_F)) {
	
	}
	
	mead::globalBulletManagerUpdate();
	mead::meadBossManager::Instance()->update();

    return false;
}

sora::SoraSprite* pSpr2;

bool mainWindow::renderFunc() {
	
	sora->beginScene();
	sora::GCN_GLOBAL->gcnLogic();
	sora::GCN_GLOBAL->gcnDraw();
		
	pFont->print(0.f, getWindowHeight()-20.f, sora::FONT_ALIGNMENT_LEFT, L"FPS: %f", sora::SORA->getFPS());
	
	luaObject.update(0.f);
	luaObject.render();
	
	mead::meadBossManager::Instance()->render();
	
	sora->endScene();
	return false;
}

void mainWindow::init() {
    sora::SORA->setFPS(60);

	pFont = sora::SORA->createFont(L"ARIALN.ttf", 20);
	pFont->setColor(0xFFFFFFFF);
	
	if(sora::GCN_GLOBAL->initGUIChan(L"ARIALN.ttf", 20) ) {
		sora::GCN_GLOBAL->createTop();
	}
	
	mead::globalBulletManagerInit();
	mead::exportBulletManager(luaObject.getState());
	
	luaObject.doScript(L"mybullettest.lua");
}

void mainWindow::onKeyEvent(const sora::SoraKeyEvent* kev) {
	if(kev->type == SORA_INPUT_KEYDOWN) {
		if(kev->key == SORA_KEY_1 ) {
			mead::meadBossManager::Instance()->clearAllBosses();
			luaObject.doScript(L"mybullettest.lua");
		} else if(kev->key == SORA_KEY_2) {
			mead::meadBossManager::Instance()->reloadScripts();
		}
	}
}