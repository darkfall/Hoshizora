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

sora::SoraVertex vert[6];

bool mainWindow::renderFunc() {
	sora->beginScene();
	sora::GCN_GLOBAL->gcnLogic();
	sora::GCN_GLOBAL->gcnDraw();

	//pSpr->render();
	//pSpr->renderWithVertices(vert, 6, SORA_TRIANGLES_FAN);
	
	sora->beginZBufferSort();
	pSpr2->render();
	pSpr3->render();
	pSpr->render();
	
	sora->endZBufferSort();

	sora->endScene();
	return false;
}

void mainWindow::init() {
    sora::SORA->setFPS(60);
	sora::SORA->setSystemFont(L"cour.ttf", 16);

	pSpr = sora::SORA->createSprite(L"bg_optd.png");
	pSpr2 = sora::SORA->createSprite(L"grass.png");
	pSpr3 = sora::SORA->createSprite(L"road.png");

	pSpr->setZ(0.1f); 
	pSpr3->setZ(0.5f);
	pSpr2->setZ(1.0f);
	/*mead::globalBulletManagerInit();
	mead::exportBulletManager(luaObject.getState());
	mead::exportGlobal(luaObject.getState());
	sora::exportSoundManager(luaObject.getState());
	sora::exportSpriteAnimation(luaObject.getState());
	
	luaObject.doScript(L"mybullettest.lua");
	*/

		
	float32 px = 400.f, py = 300.f;
	for(int i=0; i<6; ++i) {
		vert[i].col = CSETA(0xFFFFFFFF, 255/6*(i+1));
		vert[i].z = 0.0f;
		
		vert[i].x = px + 300*cosf(sora::DGR_RAD(i*60));
		vert[i].y = py + 300*sinf(sora::DGR_RAD(i*60));
		
		vert[i].tx = (pSpr->getSpriteWidth() / 2 + 300 * cosf(sora::DGR_RAD(i*60))) / pSpr->getTextureWidth(false);
		vert[i].ty = (pSpr->getSpriteHeight() / 2 + 300*sinf(sora::DGR_RAD(i*60))) / pSpr->getTextureHeight(false);
	}
	
	registerEventFunc(this, &mainWindow::onKeyEvent);
	sora::SORA_EVENT_MANAGER->registerInputEventHandler(this);
}

void mainWindow::onKeyEvent(const sora::SoraKeyEvent* kev) {
	if(kev->type == SORA_INPUT_KEYDOWN) {
		if(kev->key == SORA_KEY_1 ) {
			mead::meadBossManager::Instance()->clearAllBosses();
//			luaObject.doScript(L"mybullettest.lua");
		} else if(kev->key == SORA_KEY_2) {
			mead::meadBossManager::Instance()->reloadScripts();
		}
	}
}