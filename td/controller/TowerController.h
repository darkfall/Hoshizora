//
//  TowerManager.h
//  Sora
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_TowerManager_h
#define Sora_TowerManager_h

#include "SoraPlatform.h"
#include "SoraAutoUpdate.h"
#include "SoraComponent.h"
#include "message/SoraMessageEvent.h"

namespace atd {
    
    class Tower;
    
    class TowerController: public sora::SoraComponent {
    public:
        SORA_DEF_COMPONENT(TowerController);
        
        // towers in SoraAutoListElement
        void onUpdate(float dt);
        
        void onMessage(sora::SoraMessageEvent* evt);
        
        void clear();
        
        void loadTowerConfig(const sora::StringType& path, bool isDir=false);
        
        Tower* buildTower(float x, float y, const sora::StringType& towerType);
        Tower* getTowerByPosition(float x, float y) const;
        void removeTower(Tower* tower);
        
    private:
        typedef std::list<Tower*> TowerList;
        TowerList mTowers;
    };
    
    
} // namespace sora

#endif
