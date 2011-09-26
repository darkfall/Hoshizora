//
//  Sensor.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/24/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "Sensor.h"
#include "base/enemy.h"
#include "base/constants.h"

namespace atd {
    
    Sensor::Sensor(Object* obj):
    SoraComponent(obj, false),
    mAvailable(false) {
        
    }
    
    void Sensor::testEnemy(Enemy* enemy) {
        sora_assert(enemy);
        if(!mAvailable) return;
        
        if(mSenseCircle.testPoint(enemy->getPosition())) {
            EnemyList::iterator it = std::find(mEnemyList.begin(), mEnemyList.end(), enemy);
            if(it == mEnemyList.end()) {
                mEnemyList.push_back(EnemyConnect(enemy, 
                                                  enemy->subscribeToEnemyDeath(sora::Bind(this, &Sensor::onEnemyDied))));
                
                getOwner()->sendMessage(SID_EVENT_ENEMY_ENTER, enemy);
            }
        }
    }
    
    void Sensor::onEnemyDied(Enemy* enemy) {
        EnemyList::iterator it = mEnemyList.begin();
        EnemyList::iterator end = mEnemyList.end();
        
        for(; it != end; ++it) {
            if(it->enemy == enemy) {
                mEnemyList.erase(it);
                
                // send message
                getOwner()->sendMessage(SID_EVENT_ENEMY_DEATH, enemy);
                break;
            }
        }
    }
    
    Enemy* Sensor::pickFirstEnemy() const {
        if(!mEnemyList.empty()) {
            return mEnemyList.front().enemy;
        }
        return 0;
    }
    
    Enemy* Sensor::pickLastEnemy() const {
        if(!mEnemyList.empty()) {
            return mEnemyList.back().enemy;
        }
        return 0;
    }
    
    bool Sensor::checkEnemy(Enemy* enemy) const {
        sora_assert(enemy);
        
        return checkPosition(enemy->getPosition());
    }
    
    bool Sensor::checkPosition(const Vector2& pos) const {
        return mSenseCircle.testPoint(pos);
    }
    
    void Sensor::onMessage(SoraMessageEvent* message) {
        
    }
    
    void Sensor::setPosition(const Vector2& pos) {
        mSenseCircle.setPosition(pos);
    }
    
    Vector2 Sensor::getPosition() const {
        return mSenseCircle.getPosition();
    }
    
    void Sensor::setRadius(float r) {
        mSenseCircle.setRadius(r);
    }
    
    float Sensor::getRadius() const {
        return mSenseCircle.getRadius();
    }
    
    void Sensor::onUpdate(float dt) {
        EnemyList::iterator it = mEnemyList.begin();
        EnemyList::iterator end = mEnemyList.end();
        
        for(; it != end; ++it) {
            if(!mSenseCircle.testPoint(it->enemy->getPosition())) {
                it->conn.disconnect();
                
                getOwner()->sendMessage(SID_EVENT_ENEMY_LEAVE, it->enemy);
                
                it = mEnemyList.erase(it);
            }
        }
        
        Enemy::iterator itEnemy = Enemy::begin();
        Enemy::iterator itEnemyEnd = Enemy::end();
        for(; it != end; ++it) {
            testEnemy(*itEnemy);
        }
    }
    
    void Sensor::setAvailable(bool flag) {
        mAvailable = flag;
    }
    
    bool Sensor::isAvailable() const {
        return mAvailable;
    }
    
    
} // namespace atd
