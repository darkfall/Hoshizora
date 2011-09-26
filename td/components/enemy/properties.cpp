//
//  properties.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "properties.h"

namespace atd {
    
    Property::Property(sora::SoraLightWeightEntity* owner):
    SoraComponent(owner, false),
    mHealth(0),
    mDefense(0) { }
    
    int Property::getHealth() const {
        return mHealth;
    }
    
    void Property::setHealth(int health) {
        mHealth = health;
    }
    
    int Property::getDefense() const {
        return mDefense;
    }
    
    void Property::setDefense(int defense) {
        mDefense = defense;
    }
    
    void Property::onDamage(int atk) {
        if(mHealth > 0) {
            mHealth -= atk > mDefense ? atk - mDefense : 1;
            if(mHealth <= 0)
                getOwner()->sendMessage(SID_EVENT_DEAD, mHealth);
        }
    }
    
    bool Property::isDead() {
        return mHealth <= 0;
    }
    
    void Property::onMessage(sora::SoraMessageEvent* evt) {
        if(evt->getMessage() == SID_EVENT_DAMAGE) {
            onDamage(evt->getData<int>());
        }
    }
    
}