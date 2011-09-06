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
#include "SoraShaderManager.h"

#include "SoraModifierAdapter.h"
#include "modifiers/SoraFontModifiers.h"
#include "modifiers/SoraCameraModifiers.h"
#include "SoraExternalRenderObject.h"

#include "timer/SoraSimpleTimerManager.h"

#include "signal/SoraSignal.h"

#include "entity/SoraEntity.h"
#include "../Components/RenderComponent.h"
#include "../Components/PositionComponent.h"

#include "SoraStringTokenlizer.h"

#include "SoraParticleSystem2/Emitters/CircleEmitter.h"

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

sora::SoraEntity entity;

void handleCommand(sora::SoraConsoleEvent* evt) {
    sora::SoraStringTokenlizer token(evt->getParams(), " ");
    if(token.size() == 2) {
        entity.setProperty(token.front(), (float)atof(token.back().c_str()));
        sora::SoraPropertyBase* p = entity.getPropertyBase(token.front());
        evt->pushResult(std::string("Property ")+token.front()+" to "+p->toString());
    }
}

SORA_DEF_CONSOLE_EVT_FUNC(handleCommand, "setprop");

bool mainWindow::updateFunc() {
    
    entity.update(sora::SORA->getDelta());
    
    mScene1->update(sora::SORA->getDelta());
    return false;
}

sora::SoraSprite* spr;

void myRenderFunc(sora::SoraObject* obj) {
    spr->render();
}

bool mainWindow::renderFunc() {
    sora::SORA->beginScene();
	mScene1->render();
    
    entity.render();

    sora::SORA->endScene();
    
	return false;
}

void myFunc(sora::SoraConsoleEvent* evt) {
    sora::SORA->messageBox(evt->getCmd(), evt->getParams(), MB_OK);
}

SORA_DEF_CONSOLE_EVT_FUNC(myFunc, "test");


#include "factory/SoraSpriteFactory.h"
#include "timer/SoraSimpleTimerManager.h"
#include "SoraFileUtility.h"

sora::SoraMusicFile* file = 0;

void mainWindow::init() {
    
    ulong32 pack = sora::SoraCore::Instance()->loadResourcePack(L"Textures.zip");
     pack = sora::SoraCore::Instance()->loadResourcePack(sora::SoraFileUtility::getApplicationPath()+L"/../");
    pack = sora::SoraCore::Instance()->loadResourcePack(L"~/");
    pack = sora::SoraCore::Instance()->loadResourcePack(L"/Users/darkfall/Application.app");
    pack = sora::SoraCore::Instance()->loadResourcePack(L"/Application/myapp.app/Contents/Resources");

    
    sora::particle::CircleEmitter* emitter = new sora::particle::CircleEmitter(&entity);
    emitter->setPosition(300, 300);
    emitter->setSprite(L"textures/Particle004.png");
    emitter->setEmitInterval(1.f);
    emitter->setLifeTime(10.f);
    emitter->fireAt(300, 300);
    emitter->setEmitNum(10);
    entity.addComponent(emitter);
    
    sora::SORA->setFPS(60);
	sora::SORA->attachResourcePack(sora::SORA->loadResourcePack(L"resource.SoraResource"));
	sora::SORA->setSystemFont(L"cour.ttf", 16);
   
    mScene1 = new sora::SoraScene(getWindowWidth(), getWindowHeight());
 //   mScene1->enableRenderToCanvas(true);
    mScene2 = new sora::SoraScene(1000, 1000);
    
    mCamera = new sora::SoraCamera(100, 100.f, 500.f, 500.f);

  //  mCamera->zoomTo(2.f, 2.f, 20.f);
//    mScene1->setCamera(mCamera);
    mFont = sora::SORA->createFont(L"font.otf", 20);
    if(!mFont)
        sora::SORA->messageBox("sds", "Sds", MB_OK);
    
    
    mScene1->add(mScene2);
    sora::SoraSprite* bg = sora::SORA->createSprite(L"background.png");
    bg->setCenter(300.f, 200.f);
    mScene1->add(bg, 0);
    
    mScene2->add(new sora::SoraExternalRenderObject(myRenderFunc), -1);
    spr = sora::CreateSpriteType("sprite", L"geotest.png");
    spr->setCenter(100, 100);
    
    //spr->setPosition(300, 300);
    mScene2->add(spr);
    mScene2->setPosition(200.f, 200.f);
    mScene2->moveTo(300.f, 300.f, 10.f);
  //  mScene2->setRotation(sora::F_PI_4/2);
 //   mScene2->setScale(2.f, 2.f);
    
    sora::SoraScene* mScene3 = new sora::SoraScene(150, 150);
    mScene3->add(sora::SORA->createSprite(L"test.png"), 1);
    mScene3->add(sora::SORA->createSprite(L"bullet2.png"), 0);
    mScene2->add(mScene3);
    
}

void mainWindow::onKeyEvent(sora::SoraKeyEvent* kev) {

    
}