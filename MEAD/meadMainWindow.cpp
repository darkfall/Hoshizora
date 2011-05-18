/*
 *  meadmeadMainWindow.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/14/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "meadMainWindow.h"

#include "SoraEventManager.h"
#include "SoraGUIChan/guichansetup.h"

#include "SoraSpriteAnimation/AnimationLuaExport.h"
#include "SoraSoundManager/SoundManagerLuaExport.h"


#include "bulletLuaExport.h"
#include "bulletLuaHelper.h"
#include "meadBulletManager.h"
#include "meadPlayerManager.h"
#include "meadBossManager.h"

namespace mead {
	
	meadMainWindow::meadMainWindow() {
		sora = sora::SoraCore::Instance();
		
		registerEventFunc(this, &meadMainWindow::onKeyEvent);
		sora::SORA_EVENT_MANAGER->registerInputEventHandler(this);
	}
	
	meadMainWindow::~meadMainWindow() {
	}
	
	bool meadMainWindow::updateFunc() {
		
		if(sora->keyDown(SORA_KEY_ESCAPE))
			sora->shutDown();
		
		return false;
	}
	
	bool meadMainWindow::renderFunc() {
		
		sora->beginScene(0x00000000);
		sora::GCN_GLOBAL->gcnLogic();
		sora::GCN_GLOBAL->gcnDraw();
	
		luaObject.update(0.f);
		luaObject.render();
		
		sora->endScene();
		return false;
	}
	
	void meadMainWindow::init() {
		sora::SORA->setFPS(999);
		
		if(sora::GCN_GLOBAL->initGUIChan(L"ARIALN.ttf", 20) ) {
			sora::GCN_GLOBAL->createTop();
		}
		
		mead::globalBulletManagerInit();
		mead::exportBulletManager(luaObject.getState());
		mead::exportGlobal(luaObject.getState());
		sora::exportSoundManager(luaObject.getState());
		sora::exportSpriteAnimation(luaObject.getState());
		
		luaObject.doScript(L"mybullettest.lua");
	}
	
	void meadMainWindow::onKeyEvent(const sora::SoraKeyEvent* kev) {
		if(kev->type == SORA_INPUT_KEYDOWN) {
			if(kev->key == SORA_KEY_1 ) {
				mead::meadBossManager::Instance()->clearAllBosses();
				luaObject.doScript(L"mybullettest.lua");
			} else if(kev->key == SORA_KEY_2) {
				mead::meadBossManager::Instance()->reloadScripts();
			}
		}
	}


} // namespace mead