//
//  bomb.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "bomb.h"
#include "player.h"

#include "message/SoraMessageEvent.h"

namespace flower {
    
    Bomb::Bomb(float weight, float x, float y, int32 score):
    Object(weight, score) {
        setPosition(x, y);
    }
    
    void Bomb::die() {
        mIsDied = true;
    }
    
    void Bomb::onPlayerCollision(Player* player) {
        sora::SoraMessageEvent evt(SID_EVENT_ITEM_GET, getScore());
        player->sendMessage(&evt);
    }
    
} // namespace flower