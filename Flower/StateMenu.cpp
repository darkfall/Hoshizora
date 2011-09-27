//
//  StateMenu.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "StateMenu.h"

#include "ItemController.h"
#include "PlayerController.h"
#include "HudController.h"

namespace flower {
    
    bool timerFunc(float& dt) {
        ItemController::Instance()->throwItem(rand() % 480, rand() % 100 + 50, sora::SoraVector(0.f,  -80), "basic");
    }
    
    void StateMenu::onEnter() {
        mTimer = sora::SoraCore::Instance()->createTimer(sora::Delegate(timerFunc));
        mTimer->start(1.f, 0, 0.f);
        
        mBg = sora::SoraCore::Instance()->createSprite(L"bg.png");
        
        PlayerController::Instance()->createPlayer(240.f, 300.f, "TestPlayer", PT_LOCAL_PLAYER);
    }
    
    void StateMenu::onLeave() {
        
    }
    
    void StateMenu::onUpdate(float dt) {
        ItemController::Instance()->update(dt);
        PlayerController::Instance()->update(dt);
    }
    
    void StateMenu::onRender() {
        
        sora::SoraCore::Instance()->beginScene(0);
        mBg->render();
        
        ItemController::Instance()->render();
        PlayerController::Instance()->render();
        
        HudController::Instance()->render();
        
        sora::SoraCore::Instance()->endScene();
    }
    
} // namespace flower
