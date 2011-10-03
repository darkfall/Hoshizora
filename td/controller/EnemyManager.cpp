//
//  EnemeyManager.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "EnemyManager.h"
#include "base/enemy.h"

namespace atd {
    
    void EnemyManager::onUpdate(float dt) {
        Enemy::const_iterator it = Enemy::begin();
        Enemy::const_iterator end = Enemy::end();
        
        for(; it != end; ++it) {
            (*it)->onUpdate(dt);
        }
    }
    
    void EnemyManager::postEvent(sora::SoraMessageEvent* evt) {
        Enemy::const_iterator it = Enemy::begin();
        Enemy::const_iterator end = Enemy::end();
        
        for(; it != end; ++it) {
            (*it)->sendMessage(evt);
        }
    }
    
    
} // namespace sora
