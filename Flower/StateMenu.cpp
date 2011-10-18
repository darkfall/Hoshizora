//
//  StateMenu.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "StateMenu.h"

#include "ObjectController.h"
#include "PlayerController.h"
#include "HudController.h"

#include "timer/SoraSimpleTimerManager.h"

namespace flower {
    
    bool timerFunc(sora::SoraSimpleTimer* timer, float dt) {
        ObjectController::Instance()->throwObject(rand() % 480, rand() % 20 + 10, sora::SoraVector(0.f,  -80), "basic", OBJ_ITEM);
        return true;
    }
    
    bool timerFunc2(sora::SoraSimpleTimer* timer, float dt) {
        ObjectController::Instance()->throwObject(rand() % 480, rand() % 20 + 10, sora::SoraVector(0.f,  -80), "bomb1", OBJ_BOMB);
        return true;
    }
    
    void StateMenu::onEnter() {
        mTimer = sora::CreateSimpleTimer(timerFunc);
        mTimer->start(1.f, 0, 0.f);
        
        mTimer2 = sora::CreateSimpleTimer(timerFunc2);
        mTimer2->start(3.f, 0, 0.f);
        
        mBg = sora::SoraCore::Instance()->createSprite("bg.png");
        PlayerController::Instance()->createPlayer(240.f, 300.f, "TestPlayer", PT_LOCAL_PLAYER);
        
        target = sora::SoraCore::Instance()->createTarget(320, 480);
        mFBO = new sora::SoraSprite(sora::SoraCore::Ptr->getTargetTexture(target));
    }
    
    void StateMenu::onLeave() {
        
    }
    
    void StateMenu::onUpdate(float dt) {
        ObjectController::Instance()->update(dt);
        PlayerController::Instance()->update(dt);
    }
    
    void StateMenu::onRender() {
        
        sora::SoraCore::Instance()->beginScene(0xFFFF0000);
        mBg->render();
        
     //   ObjectController::Instance()->render();
      //  PlayerController::Instance()->render();
        
     //   HudController::Instance()->render();
        
        sora::SoraCore::Instance()->endScene();
        
    }
    
} // namespace flower
