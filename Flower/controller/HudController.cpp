//
//  HudController.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "HudController.h"
#include "SoraCore.h"

#include "PlayerController.h"
#include "ScoreController.h"


#include "../core/player.h"

namespace flower {
    
    HudController::HudController():
    mFont(0) {
    }
    
    HudController::~HudController() {
        
    }
    
    void HudController::setFont(const std::wstring& fontName, int32 fontSize) {
        if(mFont)
            sora::SoraCore::Instance()->releaseFont(mFont);
        
        mFont = sora::SoraCore::Instance()->createFont(fontName, fontSize);
    }
    
    sora::SoraFont* HudController::getFont() const {
        return mFont;
    }
    
    void HudController::render() {
        if(mFont) {
            mFont->render(0.f, 0.f, L"本地玩家");
            Player* localPlayer;
            
            if((localPlayer = PlayerController::Instance()->getLocalPlayer()) != 0)
                mFont->print(0.f, 20.f, sora::FONT_ALIGNMENT_LEFT, L"Score: %d", localPlayer->getScore());
        }
    }
    
    void HudController::update(float dt) {
        
    }
    
} // namespace flower