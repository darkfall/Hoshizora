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

#include "SoraSoundManager/SoundManagerLuaExport.h"

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
    return false;
}

bool mainWindow::renderFunc() {
    sora::SORA->beginScene();
	mScene1->render();
//    mScene1->getCanvas()->render();
    sora::SORA->endScene();
    
	return false;
}
sora::SoraMusicFile* mu = 0;

void mainWindow::init() {
	mu = sora::SORA->createMusicFile(L"Kalimba.mp3", true);
	if(mu)
		mu->play();
	sora::SORA->setFPS(60);
	sora::SORA->loadResourcePack(L"resource.SoraResource");
	sora::SORA->setSystemFont(L"cour.ttf", 16);
   
    mScene1 = new sora::SoraScene(getWindowWidth(), getWindowHeight());
 //   mScene1->enableRenderToCanvas(true);
    mScene2 = new sora::SoraScene(1000, 1000);
    
    mCamera = new sora::SoraCamera(100, 100.f, 500.f, 500.f);
    mCamera->zoomTo(2.f, 2.f, 20.f);
    mScene1->setCamera(mCamera);
    
    mScene1->add(mScene2);
    sora::SoraSprite* bg = sora::SORA->createSprite(L"bg_optd.png");
    mScene1->add(bg, 0);
    
    sora::SoraSprite* spr = sora::SORA->createSprite(L"bullet2.png");
    spr->setCenter(100, 100);
    //spr->setPosition(300, 300);
    mScene2->add(spr);
    mScene2->setPosition(200.f, 200.f);
  //  mScene2->setRotation(sora::F_PI_4/2);
 //   mScene2->setScale(2.f, 2.f);
    
    sora::SoraScene* mScene3 = new sora::SoraScene(150, 150);
    mScene3->add(sora::SORA->createSprite(L"boxbutton.png"));
    mScene2->add(mScene3);
  //  mScene3->setRotation(sora::F_PI_4);
    
   // mScene2->moveTo(800.f, 400.f, 10.f);
}

void mainWindow::onKeyEvent(sora::SoraKeyEvent* kev) {

    
}