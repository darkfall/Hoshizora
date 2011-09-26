//
//  Sensor.h
//  Sora
//
//  Created by Ruiwei Bu on 9/24/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_Sensor_h
#define Sora_Sensor_h

#include "SoraComponent.h"
#include "signal/SoraSignal.h"
#include "base/circle.h"
#include "base/vector2.h"

namespace atd {
    
    using namespace sora;
    
    class Enemy;
    class Object;
    /**
     * A basic a sensor that listens enemy movement
     * And notify the tower
     */
    
    class Sensor: public SoraComponent {
    public:  
        Sensor(Object* obj);
        
        void testEnemy(Enemy* enemy);
        
        void onEnemyDied(Enemy* enemy);
        void onMessage(SoraMessageEvent* message);
        
        void setPosition(const Vector2& pos);
        Vector2 getPosition() const;
        
        void setRadius(float r);
        float getRadius() const;
        
        void onUpdate(float dt);
        
        void setAvailable(bool flag);
        bool isAvailable() const;
        
        bool checkEnemy(Enemy* enemy) const;
        bool checkPosition(const Vector2& pos) const;
        
        Enemy* pickFirstEnemy() const;
        Enemy* pickLastEnemy() const;
        
        SORA_DEF_COMPONENT(Sensor);
        
    private:
        struct EnemyConnect {
            Enemy* enemy;
            SoraConnection conn;
            
            EnemyConnect(Enemy* enemy, const SoraConnection& connection):
            enemy(enemy),
            conn(connection) {
            }
            
            EnemyConnect():
            enemy(0) {
                
            }
            
            bool operator==(Enemy* e) {
                return enemy == e;
            }
        } ;
        typedef std::list<EnemyConnect> EnemyList;
        EnemyList mEnemyList;
        
        bool mAvailable;
        Circle mSenseCircle;
    };
    
} // namespace atd


#endif
