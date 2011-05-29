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


#include "../ReflectionRev/rfMap.h"
#include "../ReflectionRev/rfShape.h"

#include "SoraSoundManager/SoraBGMManager.h"

#include "../../../MEAD/bulletLuaExport.h"
#include "../../../MEAD/meadBossManager.h"
#include "../../../MEAD/bulletLuaHelper.h"

#include "SoraSoundManager/SoundManagerLuaExport.h"
#include "SoraSpriteAnimation/AnimationLuaExport.h"

mainWindow::mainWindow() {
	sora = sora::SoraCore::Instance();
	
	sora::SORA_EVENT_MANAGER->registerEvent("MainWindow", this, 0);
}

mainWindow::~mainWindow() {
}

bool mainWindow::updateFunc() {
	

    return false;
}

sora::SoraSprite* pSpr2;
sora::SoraSprite* pSpr3;

bool mainWindow::renderFunc() {
	sora->beginScene();
	sora::GCN_GLOBAL->gcnLogic();
	sora::GCN_GLOBAL->gcnDraw();

	pSpr->render();
	pSpr3->render();
	pSpr2->render();

	sora->endScene();
	return false;
}

void mainWindow::init() {
    sora::SORA->setFPS(60);

	pSpr = sora::SORA->createSprite(L"bg_optd.png");
	pSpr2 = sora::SORA->createSprite(L"grass.png");
	pSpr3 = sora::SORA->createSprite(L"road.png");

	pSpr->setBlendMode(BLEND_DEFAULT_Z);
	pSpr2->setBlendMode(BLEND_DEFAULT_Z);
	pSpr3->setBlendMode(BLEND_DEFAULT_Z);

	pSpr->setZ(1.f);
	pSpr3->setZ(0.5f);
	pSpr2->setZ(0.0f);
	/*mead::globalBulletManagerInit();
	mead::exportBulletManager(luaObject.getState());
	mead::exportGlobal(luaObject.getState());
	sora::exportSoundManager(luaObject.getState());
	sora::exportSpriteAnimation(luaObject.getState());
	
	luaObject.doScript(L"mybullettest.lua");
	*/

	registerEventFunc(this, &mainWindow::onKeyEvent);
	sora::SORA_EVENT_MANAGER->registerInputEventHandler(this);
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