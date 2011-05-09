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
#include "SoraSoundManager/SoundManagerLuaExport.h"

#include "SoraSpriteAnimation/AnimationLuaExport.h"

#include "../MEAD/bulletLuaExport.h"
#include "../MEAD/meadBossManager.h"
#include "../MEAD/bulletLuaHelper.h"
#include "SoraLua/SoraLuaExport.h"

#include "../ReflectionTD/rftdEnemyObject.h"
#include "../ReflectionTD/rftdCommon.h"

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
	
    return false;
}

rftd::rftdEnemyObject* test;
float32 posx = 0;
#include "../ReflectionTD/rftdMap.h"

rftd::rftdMap* mtmap;
rftd::MapConf conf;

bool mainWindow::renderFunc() {
	
	sora->beginScene(0x00000000);
	sora::GCN_GLOBAL->gcnLogic();
	sora::GCN_GLOBAL->gcnDraw();
		
	//pFont->print(0.f, getWindowHeight()-20.f, sora::FONT_ALIGNMENT_LEFT, L"FPS: %f", sora::SORA->getFPS());
	
//	pSpr->render4V(256.f, 128.f, 256.f, 256.f, 376.f, 256.f, 376.f, 128.f);
/*	pSpr2->update(sora::SORA->getDelta());
	pSpr2->render(0.f, -128.f);*/

/*	pressAnyKey->render(128+posx, 512.f);
	pressAnyKey->setTextureRect(posx, 0.f, pressAnyKey->getTextureWidth()-posx, pressAnyKey->getSpriteHeight());
	posx += 1;*/
	
//	test->update(sora::SORA->getDelta());
//	test->render();
	
	pFont->print(getWindowWidth(), 0.f, sora::FONT_ALIGNMENT_RIGHT, L"%.2f/%.2f", mtmap->getCurrLevelInterval(), mtmap->getLevelInterval());
	pFont->print(getWindowWidth(), 20.f, sora::FONT_ALIGNMENT_RIGHT, L"level: %u", mtmap->getCurrLevel());
	pFont->print(getWindowWidth(), 40.f, sora::FONT_ALIGNMENT_RIGHT, L"enemy: %d", mtmap->getEnemySize());
	pFont->render(getWindowWidth(), 60.f, sora::FONT_ALIGNMENT_RIGHT, mtmap->getCurrLevelName().c_str());
	pFont->render(getWindowWidth(), 80.f, sora::FONT_ALIGNMENT_RIGHT, conf.mapName.c_str());
	
//	luaObject.update(0.f);
//	luaObject.render();
	
	sora->endScene();
	return false;
}

void mainWindow::init() {
    sora::SORA->setFPS(60);

	pFont = sora::SORA->createFont(L"Bank Gothic Medium BT.ttf", 20);
	pFont->setColor(0xFFFFCC00);
	
	if(sora::GCN_GLOBAL->initGUIChan(L"ARIALN.ttf", 20) ) {
		sora::GCN_GLOBAL->createTop();
	}
	
	pSpr = sora::SORA->createSprite(L"rtfdRoad.png");
	pSpr->setTextureRect(0.f, 0.f, 64.f, 128.f);
	
	pSpr2 = sora::SORA->createSprite(L"rfTDbghl.png");
	pSpr2->addEffect(new sora::IEFade(0.f, 1.f, 1.f, sora::IMAGE_EFFECT_PINGPONG));

	pressAnyKey = sora::SORA->createSprite(L"PressAnyKey.png");
	
	psSpr = sora::SORA->createSprite(L"particle2.png");
	ps = new sora::SoraParticleSystem(L"cparticle.psi.sps", psSpr);
	
	rftd::rftdInitialize();
	
	conf = rftd::GET_MAP(L"MyMapPack1", 0);
	mtmap = new rftd::rftdMap;
	mtmap->readMapConf(conf.mapPath);
	mtmap->start();
	sora::GCN_GLOBAL->addWidget(mtmap, "top");
	
/*	mead::globalBulletManagerInit();
	mead::exportBulletManager(luaObject.getState());
	mead::exportGlobal(luaObject.getState());
	sora::exportSoundManager(luaObject.getState());
	sora::exportSpriteAnimation(luaObject.getState());
	
	luaObject.doScript(L"mybullettest.lua");*/
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