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

#include "SoraPNGOps/SoraCompressedTexture.h"

#include "SoraGUIChan/Modifiers/CloseModifier.h"

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

bool mainWindow::renderFunc() {
	
	sora->beginScene(0x00000000);
	sora::GCN_GLOBAL->gcnLogic();
	sora::GCN_GLOBAL->gcnDraw();
		
	pFont->print(0.f, getWindowHeight()-20.f, sora::FONT_ALIGNMENT_LEFT, L"FPS: %f", sora::SORA->getFPS());

	pSpr->update(sora::SORA->getDelta());
	pSpr->render();

	sora->endScene();
	return false;
}

void mainWindow::init() {
    sora::SORA->setFPS(60);
	sora::SORA->attachResourcePack(sora::SORA->loadResourcePack(L"resource.SoraResource"));
	
	pFont = sora::SORA->createFont(L"Bank Gothic Medium BT.ttf", 20);
	pFont->setColor(0xFFFFCC00);

		
	sora::GCN_GLOBAL->initGUIChan(L"ARIALN.ttf", 20);
	
	pSpr = sora::SORA->createSprite(L"magicCircle.png");
	pSpr->addEffect((new sora::SoraImageEffectList(sora::IMAGE_EFFECT_REPEAT))
					->add(new sora::IEFade(1.f, 0.5f, 1.f))
					->add(new sora::IETransitions(0.f, 0.f, 500.f, 500.f, 2.f)));
	
	gcn::Container* cont = new gcn::Container;
	cont->setDimension(gcn::Rectangle(100, 0, 800, 800));
	cont->setOpaque(true);
	cont->addModifier(new gcn::CloseModifier(600, gcn::CloseModifier::MODE_UL_TO_LR));
	sora::GCN_GLOBAL->addWidget(cont, NULL);
	
}

void mainWindow::onKeyEvent(const sora::SoraKeyEvent* kev) {
	if(kev->type == SORA_INPUT_KEYDOWN) {
		if(kev->key == SORA_KEY_1 ) {
		
		}
	}
}