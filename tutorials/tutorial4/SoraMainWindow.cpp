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

bool MainWindow::updateFunc() {
    return false;
}

bool MainWindow::renderFunc() {
    sora::SoraCore* inst = sora::SoraCore::Instance();  
    inst->beginScene();
    
    if(mFont) {
        mFont->render(0.f, 0.f, sora::FONT_ALIGNMENT_LEFT, L"Resource loaded from zip file");
        mFont->print(0.f, 20.f, sora::FONT_ALIGNMENT_LEFT, L"Pressed key: %c", mPressedKey);
        mFont->print(0.f, 40.f, sora::FONT_ALIGNMENT_LEFT, L"Timer Count: %d", mTimeCount);
    }

    sora::SORA->endScene();
    
    return false;
}

void MainWindow::onInputEvent(sora::SoraKeyEvent* kev) {
    mPressedKey = kev->chr;
}

void MainWindow::onConsoleCmdEvent(sora::SoraConsoleEvent* cev) {
    if(cev->getCmd().compare("message") == 0) {
        sora::SORA->messageBox(cev->getParams(), "message", MB_OK);
    }
}

void MainWindow::onTimerEvent(sora::SoraTimerEvent* tev) {
    ++mTimeCount;
}

void MainWindow::init() {
    sora::SORA->setFPS(60);

    // register event callback funcs
    registerEventFunc(this, &MainWindow::onInputEvent);
    registerEventFunc(this, &MainWindow::onConsoleCmdEvent);
    registerEventFunc(this, &MainWindow::onTimerEvent);
    
    // shows interest on specific events
    sora::SoraEventManager* eventManager = sora::SoraEventManager::Instance();
    eventManager->registerInputEventHandler(this);
    eventManager->createTimerEvent(this, 1.f);

    sora::SORA->getConsole()->registerCmdHandler(this, "message");
    
    ulong32 resourcePack = sora::SORA->loadResourcePack(L"resource.zip");
    if(!resourcePack)
        THROW_SORA_EXCEPTION("Error loading resource pack");
    else
        sora::SORA->attachResourcePack(resourcePack);
    
    mFont = sora::SORA->createFont(L"ThonburiBold.ttf", 16);
    if(!mFont)
        THROW_SORA_EXCEPTION("Error creating font");
    
    mTimeCount = 0;
    mPressedKey = '?';
}