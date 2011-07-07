//
//  entity.h
//  Sora
//
//  Created by Ruiwei Bu on 7/7/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_entity_h
#define Sora_entity_h

#include "../SoraASSMStateMachine.h"

namespace sora {
    
    namespace test {
    
        class attackEvent: public SoraEvent {
        public:
            attackEvent(int32 attack): mAttack(attack) {}
            
            int32 getAttack() const {
                return mAttack;
            }
            void setAttack(int32 attack) {
                mAttack = attack;
            }
            
        private:
            int32 mAttack;
        };
        
        class SimpleCharacter;
    
        class state_patrol: public SoraASSMState<SimpleCharacter> {
        public:
            void onEnter();            
            void onExecute();
            
            void onLeave();
        };
        
        class attack_action: public SoraASSMAction<SimpleCharacter> {
        public:
            attack_action() {
            
            }
            
            void setEnemy(SimpleCharacter* c) {
                enemy = c;
            }
            void onUpdate();
            
        private:
            SimpleCharacter* enemy;
        };
        
        
        class state_attack: public SoraASSMState<SimpleCharacter> {
        public:
            state_attack();
            ~state_attack();
            
            void setEnemy(SimpleCharacter* c);
            
            void onEnter();
            void onExecute();
            
            void onLeave();
            
        private:
            attack_action actionAttack;
        };
        
        class SimpleCharacter: public SoraEventHandler {
        public:
            SimpleCharacter(int32 health, int32 attack, const std::string& name):
            mAttack(attack),
            mHealth(health),
            mDied(false),
            mName(name) {
                
            }
            
            int32 getHealth() const {
                return mHealth;
            }
            
            int32 getAttack() const {
                return mAttack;
            }
            
            void attack(SimpleCharacter* enemy) {
                if(mDied)
                    printf("Character already died!");
                attackEvent event(mAttack);  
                enemy->handleEvent(&event);
            }
            
            bool isDied() const {
                return mDied;
            }
            
            void onAttackEvent(attackEvent* ev) {
                if(mDied)
                    printf("Character already died!");
                else 
                    printf("Character attacked!");
                mHealth -= ev->getAttack();
                if(mHealth <= 0)
                    mDied = true;
            }
            
            std::string getName() const {
                return mName;
            }
            
            state_attack* getAttackState() {
                return &attackState;
            }
            
            state_patrol* getPartolState() {
                return &patrolState;
            }
            
        private:
            int32 mHealth;
            int32 mAttack;
            bool mDied;
            
            std::string mName;
            
            state_attack attackState;
            state_patrol patrolState;
        };
    
    }
}


#endif
