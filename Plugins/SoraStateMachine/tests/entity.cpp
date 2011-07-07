//
//  entity.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/7/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "entity.h"

namespace sora {
    
    namespace test {
                
        void state_patrol::onEnter() {
            printf("Character %s entered patrol", getOwner()->getName().c_str());
        }
        
        void state_patrol::onExecute() {
            if(rand() % 100 < 10) {
                SimpleCharacter* enemy = new SimpleCharacter(100, rand() % 10, "Enemy Unknow");
                
            }
        }
        
        void state_patrol::onLeave() {
            printf("Character %s leaved patrol", getOwner()->getName().c_str());
        }
        
        void attack_action::onUpdate() {
            if(!enemy->isDied()) 
                getOwner()->attack(enemy);
            else 
                finish();
        }
    
        
        state_attack::state_attack() {
            addAction(&actionAttack);
        }
        
        void state_attack::setEnemy(SimpleCharacter* c) {
            actionAttack.setEnemy(c);
        }
        
        void state_attack::onEnter() {
            printf("Character %s entered attack mode", getOwner()->getName().c_str());
        }
        
        void state_attack::onExecute() {
            
        }
        
        void state_attack::onLeave() {
            printf("Character %s leaved attack mode", getOwner()->getName().c_str());
        }
    }
    
    
}
