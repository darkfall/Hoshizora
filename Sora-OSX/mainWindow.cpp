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

#include "Debug/SoraAutoProfile.h"

#include "cmd/SoraConsole.h"

#include "SoraModifierAdapter.h"
#include "modifiers/SoraFontModifiers.h"
#include "modifiers/SoraCameraModifiers.h"
#include "SoraExternalRenderObject.h"

#include "timer/SoraSimpleTimerManager.h"

#include "signal/SoraSignal.h"

#include "entity/SoraEntity.h"

#include "SoraStringTokenlizer.h"

#include "SoraParticleSystem2/Emitters/CircleEmitter.h"
#include "SoraEventManager.h"
#include "SoraEvent.h"

mainWindow::mainWindow() {
	sora = sora::SoraCore::Instance();
    
	registerEventFunc(this, &mainWindow::onKeyEvent);
	sora::SORA_EVENT_MANAGER->registerInputEventHandler(this);
}

mainWindow::~mainWindow() {
}

float cx = 300.f, cy = 400.f, cz = 1.f;
void transform3d(float& x, float& y, float z) {
	float scale = (cz - z) / cz;
	x = (x - cx) * scale + cx;
	y = (y - cy) * scale + cy;
	z = 0.f;
}


bool mainWindow::updateFunc() {
        
    mScene1->update(sora::SoraCore::Ptr->getDelta());
    return false;
}

sora::SoraSprite* spr;

void myRenderFunc(sora::SoraObject* obj) {
    spr->render();
}

bool mainWindow::renderFunc() {
    sora::SoraCore::Ptr->beginScene();
	mScene1->render();
   
    sora::SoraCore::Ptr->endScene();
    
	return false;
}

void myFunc(sora::SoraConsoleEvent* evt) {
    sora::SoraCore::Ptr->messageBox(evt->getCmd(), evt->getParams(), MB_OK);
}

SORA_DEF_CONSOLE_EVT_FUNC(myFunc, "test");


#include "factory/SoraSpriteFactory.h"
#include "timer/SoraSimpleTimerManager.h"

sora::SoraMusicFile* file = 0;

void mainWindow::init() {
    
    SoraHandle pack = sora::SoraCore::Instance()->loadResourcePack("Textures.zip");
    
    sora::SoraCore::Ptr->setFPS(60);
	sora::SoraCore::Ptr->loadResourcePack(L"resource.SoraResource");
	sora::SoraCore::Ptr->setSystemFont(L"cour.ttf", 16);
   
    mScene1 = new sora::Sora2DScene(getWindowWidth(), getWindowHeight());
 //   mScene1->enableRenderToCanvas(true);
    mScene2 = new sora::Sora2DScene(1000, 1000);
    
    mCamera = new sora::Sora2DCamera(100, 100.f, 500.f, 500.f);

  //  mCamera->zoomTo(2.f, 2.f, 20.f);
//    mScene1->setCamera(mCamera);
    mFont = sora::SoraCore::Ptr->createFont(L"font.otf", 20);
    if(!mFont)
        sora::SoraCore::Ptr->messageBox("sds", "Sds", MB_OK);
    
    
    mScene1->add(mScene2);
    sora::SoraSprite* bg = sora::SoraCore::Ptr->createSprite(L"background.png");
    bg->setCenter(300.f, 200.f);
    mScene1->add(bg, 0);
    
    mScene2->add(new sora::SoraExternalRenderObject(myRenderFunc), -1);
    spr->setCenter(100, 100);
    
    //spr->setPosition(300, 300);
    mScene2->add(spr);
    mScene2->setPosition(200.f, 200.f);
    mScene2->moveTo(300.f, 300.f, 10.f);
  //  mScene2->setRotation(sora::F_PI_4/2);
 //   mScene2->setScale(2.f, 2.f);
    
    sora::Sora2DScene* mScene3 = new sora::Sora2DScene(150, 150);
    mScene3->add(sora::SoraCore::Ptr->createSprite(L"test.png"), 1);
    mScene3->add(sora::SoraCore::Ptr->createSprite(L"bullet2.png"), 0);
    mScene2->add(mScene3);
    
}

void mainWindow::onKeyEvent(sora::SoraKeyEvent* kev) {

    
}