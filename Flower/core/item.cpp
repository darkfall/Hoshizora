//
//  item.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "item.h"
#include "Player.h"

#include "../controller/defines.h"

namespace flower {

    Item::Item(float weight, float x, float y, int32 score):
    Object(weight, score) {
        setPosition(x, y);
    }
    
    void Item::die() {
        mIsDied = true;
    }
        
    void Item::onPlayerCollision(Player* player) {
        sora::SoraMessageEvent evt(SID_EVENT_ITEM_GET, getScore());
        player->sendMessage(&evt);
    }
} // namespace sora
