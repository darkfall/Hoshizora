//
//  SoraParticleSystem.h
//  Sora
//
//  Created by Robert Bu on 8/29/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraParticleSystem_2_h
#define Sora_SoraParticleSystem_2_h

#include "SoraMath.h"
#include "SoraColor.h"

namespace sora {
    
    class SoraSprite;
    
    namespace particle {
        
        struct ParticleNode {
            ParticleNode(const SoraVector& pos, float life):
            mPosition(pos),
            mCurrLifetime(0.f),
            mLifetime(life),
            mDead(false),
            mSprite(0) {
                
            }
            
            SoraVector mSpeed;
            SoraVector mPosition;
                   
            float32 mCurrLifetime;
            float32 mLifetime;
            
            bool mDead;
            SoraSprite* mSprite;
            
            inline bool isDead() const {
                return mDead;
            }
            
            virtual void transform(float32 dt) {
                if(mDead)
                    return;
                
                mPosition += mSpeed * dt;
                               
                mCurrLifetime += dt;
                if(mCurrLifetime >= mLifetime)
                    mDead = true;
            }
            
            virtual void render() {
                if(!mDead && mSprite) {
                    mSprite->render(mPosition.x, mPosition.y);
                }
            }
        };
        
    } // namespace particle
    
    
} // namespace sora



#endif
