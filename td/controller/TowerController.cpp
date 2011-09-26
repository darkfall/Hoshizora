//
//  TowerManager.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "TowerController.h"
#include "base/Tower.h"

#include "config/SoraConfigParser.h"

namespace atd {
    
    // enemies in SoraAutoListElement
    void TowerController::onUpdate(float dt) {
        TowerList::const_iterator it = mTowers.begin();
        TowerList::const_iterator end = mTowers.end();
        
        for(; it != end; ++it) {
            (*it)->onUpdate(dt);
        }
    }
    
    void TowerController::onMessage(sora::SoraMessageEvent* evt) {
        TowerList::const_iterator it = mTowers.begin();
        TowerList::const_iterator end = mTowers.end();
        
        for(; it != end; ++it) {
            (*it)->sendMessageT(evt);
        }
    }
    
    void TowerController::loadTowerConfig(const sora::StringType& path, bool isDir) {
        
    }
    
    Tower* TowerController::buildTower(float x, float y, const sora::StringType& towerType)  {
        
    }
    
    Tower* TowerController::getTowerByPosition(float x, float y) const {
        
    }
    
    void TowerController::removeTower(Tower* tower) {
        
    }
    
} // namespace atd