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
	
	ps->update(sora::SORA->getDelta());

	
    return false;
}

bool mainWindow::renderFunc() {
	
	sora->beginScene(0x00000000);
//	sora::GCN_GLOBAL->gcnLogic();
//	sora::GCN_GLOBAL->gcnDraw();
		
	pFont->print(0.f, getWindowHeight()-20.f, sora::FONT_ALIGNMENT_LEFT, L"FPS: %f", sora::SORA->getFPS());
	pFont->print(0.f, getWindowHeight()-40.f, sora::FONT_ALIGNMENT_LEFT, L"NUM: %d", ps->getLiveParticle());

//	pSpr->update(sora::SORA->getDelta());
//	pSpr->render();
	
	float mx, my;
	sora::SORA->getMousePos(&mx, &my);
	ps->moveTo(mx, my, 0.f);
	ps->render();
//	pSpr->render4V(256.f, 128.f, 256.f, 256.f, 376.f, 256.f, 376.f, 128.f);
/*	pSpr2->update(sora::SORA->getDelta());
	pSpr2->render(0.f, -128.f);*/

/*	pressAnyKey->render(128+posx, 512.f);
	pressAnyKey->setTextureRect(posx, 0.f, pressAnyKey->getTextureWidth()-posx, pressAnyKey->getSpriteHeight());
	posx += 1;*/
	
//	test->update(sora::SORA->getDelta());
//	test->render();

	sora->endScene();
	return false;
}

void mainWindow::init() {
    sora::SORA->setFPS(120);
	sora::SORA->attachResourcePack(sora::SORA->loadResourcePack(L"resource.SoraResource"));
	
	pFont = sora::SORA->createFont(L"Bank Gothic Medium BT.ttf", 20);
	pFont->setColor(0xFFFFCC00);
	
	pSpr = sora::SORA->createSprite(L"pics/particles.png");
	
	if(sora::GCN_GLOBAL->initGUIChan(L"ARIALN.ttf", 20) ) {
		sora::GCN_GLOBAL->createTop();
	}
	
	ps = new sora::SoraParticleSystem;
	ps->emit(L"Default.sps", pSpr);
	ps->fire();
}

void mainWindow::onKeyEvent(const sora::SoraKeyEvent* kev) {
	if(kev->type == SORA_INPUT_KEYDOWN) {
		if(kev->key == SORA_KEY_1 ) {
		
		}
	}
}