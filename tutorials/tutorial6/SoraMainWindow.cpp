//
//  SoraMainWindow.cpp
//  Hoshizora
//
//  Created by Ruiwei Bu on 7/16/11.
//  Copyright 2011 Robert Bu. All rights reserved.
//

#include "SoraMainWindow.h"
#include "SoraCore.h"
#include "SoraMath.h"

#include "SoraModifierAdapter.h"
#include "modifiers/SoraFontModifiers.h"

#include "SoraDelegate.h"
#include "SoraShaderManager.h"

bool MainWindow::updateFunc() {
    sora::SoraCore* core = sora::SORA;
    if(core->keyDown(SORA_KEY_DOWN)) {
        mCamera->setPosition(mCamera->getPositionX(), 
                             mCamera->getPositionY()+3.f);
    }
    else if(core->keyDown(SORA_KEY_UP)) {
        mCamera->setPosition(mCamera->getPositionX(), 
                             mCamera->getPositionY()-3.f);
    }
    if(core->keyDown(SORA_KEY_LEFT)) {
        mCamera->setPosition(mCamera->getPositionX()-3.f, 
                             mCamera->getPositionY());
    }
    else if(core->keyDown(SORA_KEY_RIGHT)) {
        mCamera->setPosition(mCamera->getPositionX()+3.f, 
                             mCamera->getPositionY());
    }
    return false;
}

bool MainWindow::renderFunc() {
    sora::SoraCore* inst = sora::SoraCore::Instance();  
    inst->beginScene();

    
    mScene1->render();
    if(mFont) {
        mFont->render(0.f, getWindowHeight()-60.f, sora::FONT_ALIGNMENT_LEFT, L"Usage: Press 1 to apply camera\n 2 to disable camera\n   Arrows to Move camera");
    }
    
    sora::SORA->endScene();
    
    return false;
}

void MainWindow::onKeyEvent(sora::SoraKeyEvent* kev) {
    if(kev->isKeyPressed(SORA_KEY_1)) {
        mScene1->setCamera(mCamera);
    } 
    else if(kev->isKeyPressed(SORA_KEY_2)) {
        mScene1->setCamera(NULL);
    }
}


void MainWindow::init() {
    sora::SORA->setFPS(60);    
    
    registerEventFunc(this, &MainWindow::onKeyEvent);
    sora::SoraEventManager::Instance()->registerInputEventHandler(this);
    
    ulong32 resourcePack = sora::SORA->loadResourcePack(L"resource.zip");
    if(!resourcePack)
        THROW_SORA_EXCEPTION("Error loading resource pack");
    else
        sora::SORA->attachResourcePack(resourcePack);
    
    mFont = sora::SORA->createFont(L"ThonburiBold.ttf", 16);
    
    mSprite1 = sora::SORA->createSprite(L"geotest.png");
    mSprite2 = sora::SORA->createSprite(L"test.png");
    mSprite3 = sora::SORA->createSprite(L"titlebg_optd copy.png");
    
    mScene1 = new sora::SoraScene(getWindowWidth(), getWindowHeight());
    mScene2 = new sora::SoraScene(500, 500);
    mScene2->setPosition(100.f,100.f);
    
    mScene1->add(mScene2);
    mScene1->add(mSprite3);
    mScene2->add(mSprite1, 1);
    mScene2->add(mSprite2, 0);
    
    mCamera = new sora::SoraCamera(50.f, 50.f, 500.f, 500.f);
}