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
#include "SoraSprite.h"

namespace sora {
    
    class SoraSprite;
    
    namespace particle {
        
        struct ParticleNode {
            ParticleNode(const SoraVector& pos):
            mPosition(pos),
            mCurrLifetime(0.f),
            mLifetime(0.f),
            mDead(false),
            mSprite(0),
            mFriction(0.f),
            mTrigAcc(0.f),
            mGravity(0.f),
            mScale(SoraVector(1.f, 1.f)),
            mScaleVar(SoraVector(0.f, 0.f)),
            mSpin(0.f),
            mSpinVar(0.f),
            mColor(0xFFFFFFFF),
            mColorVar(0) {
                
            }
            
            SoraVector mSpeed;
            SoraVector mPosition;
                   
            float mCurrLifetime;
            float mLifetime;
            
            float mFriction;
            SoraVector mAcceleration;
            
            float mTrigAcc;
            float mGravity;
            
            SoraVector mScale;
            SoraVector mScaleVar;
            
            float mSpin;
            float mSpinVar;
            
            uint32 mColor;
            uint32 mColorVar;
            
            bool mDead;
            SoraSprite* mSprite;
            
            inline bool isDead() const {
                return mDead;
            }
            
            virtual void transform(float dt) {
                if(mDead)
                    return;
                
                mPosition += mSpeed * dt;
                
                
                mSpeed += mAcceleration * dt;
                
                if(mFriction != 0.f) {
                    float angle = mSpeed.angle();
                    float df = mFriction * dt;
                    mSpeed -= SoraVector(df * cosf(angle),
                                         dt * sinf(angle));
                }
                
                if(mTrigAcc != 0.f)
                    mPosition.rotate(mTrigAcc * dt);
                
                if(mGravity != 0.f)
                    mPosition.y += mGravity * dt;
                
                mScale += mScaleVar * dt;
                
                if(mSpinVar != 0.f)
                    mSpin += mSpinVar * dt;
                
                if(mColorVar != 0)
                    mColor += mColorVar * dt;
                               
                if(mLifetime != 0.f) {
                    mCurrLifetime += dt;
                    if(mCurrLifetime >= mLifetime)
                        mDead = true;
                }
            }
            
            virtual void render() {
                if(!mDead && mSprite) {
                    mSprite->setRotation(mSpin);
                    mSprite->setScale(mScale.x, mScale.y);
                    mSprite->setColor(mColor);
                    mSprite->render(mPosition.x, mPosition.y);
                }
            }
        };
        
    } // namespace particle
    
    
} // namespace sora



#endif
