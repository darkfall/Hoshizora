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

#include "SoraPNGOps/SoraPNGOptimizer.h"
#include "Debug/SoraAutoProfile.h"

#include "cmd/SoraConsole.h"

#include "SoraGifSprite/SoraGifSprite.h"
#include "SoraGenericObjects/SoraCustomShapeSprite.h"

#include "../MEAD/bulletLuaExport.h"
#include "SoraSoundManager/SoundManagerLuaExport.h"

#include "SoraThread/SoraThreadPool.h"

#include "SoraThread/SoraCountDownLatch.h"
#include "SoraThread/SoraBlockingQueue.h"

#include "SoraBox2D/SoraPhysicalWorld.h"
#include "SoraBox2D/SoraPhysicalObject.h"

#include "SoraShaderManager.h"

#include "SoraModifierAdapter.h"
#include "modifiers/SoraFontModifiers.h"


mainWindow::mainWindow() {
	sora = sora::SoraCore::Instance();
    
	registerEventFunc(this, &mainWindow::onKeyEvent);
	sora::SORA_EVENT_MANAGER->registerInputEventHandler(this);
}

mainWindow::~mainWindow() {
}

float32 cx = 300.f, cy = 400.f, cz = 1.f;
void transform3d(float32& x, float32& y, float32 z) {
	float scale = (cz - z) / cz;
	x = (x - cx) * scale + cx;
	y = (y - cy) * scale + cy;
	z = 0.f;
}

bool mainWindow::updateFunc() {
    
}

bool mainWindow::renderFunc() {
    sora::SORA->beginScene();
	mScene1->render();
    sora::SORA->endScene();
    
	return false;
}

void mainWindow::init() {
    sora::SORA->setFPS(60);
	sora::SORA->attachResourcePack(sora::SORA->loadResourcePack(L"resource.SoraResource"));
	sora::SORA->setSystemFont(L"cour.ttf", 16);
   
    mScene1 = new sora::SoraScene(getWindowWidth(), getWindowHeight());
    mScene2 = new sora::SoraScene(300, 300);
    
    mScene1->add(mScene2);
    
    sora::SoraSprite* bg = sora::SORA->createSprite(L"background.png");
    mScene1->add(bg, 0);
    
    sora::SoraSprite* spr = sora::SORA->createSprite(L"geotest.png");
    mScene2->add(spr);
    mScene2->setPosition(200.f, 200.f);
  //  mScene2->setRotation(sora::F_PI_4/2);
    mScene2->setScale(2.f, 2.f);
    
    sora::SoraScene* mScene3 = new sora::SoraScene(150, 150);
    mScene3->add(sora::SORA->createSprite(L"test.png"));
    mScene2->add(mScene3);
    mScene3->setRotation(90);
}

void mainWindow::onKeyEvent(sora::SoraKeyEvent* kev) {

    
}