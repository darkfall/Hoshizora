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
#include "SoraPath.h"

bool MainWindow::updateFunc() {
    
    return false;
}

bool MainWindow::renderFunc() {
    sora::SORA->beginScene();
    
    if(mSprite) {
        mSprite->setRotation(0.f);
        mSprite->setScale(1.f, 1.f);
        mSprite->setColor(0xFFFFFFFF);
        mSprite->render();
        
        mSprite->setColor(0xFFFF0000);
        mSprite->render(410.f, 0.f);
        
        mSprite->setColor(0xFFFFFFFF);
        mSprite->setScale(0.5f, 0.5f);
        mSprite->render(0.f, 400.f);

        mSprite->setRotation(sora::F_PI_4);
        mSprite->render(400.f, 300.f);
    }
    
    sora::SORA->endScene();
    
    return false;
}

void MainWindow::init() {
    printf("%s\n", sora::SoraPath::current().c_str());
    sora::SORA->setFPS(60);
    mSprite = sora::SORA->createSprite(L"geotest.png");
    if(!mSprite)
        THROW_SORA_EXCEPTION("Error creating sprite");
}