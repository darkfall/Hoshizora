//
//  SoraBasicParticleEmitter.h
//  Sora
//
//  Created by Ruiwei Bu on 8/29/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraBasicParticleEmitter_h
#define Sora_SoraBasicParticleEmitter_h

#include "SoraParticleEmitter.h"

namespace sora {
    
    namespace particle {

        /**
         * Particle Emitter with
         * Speed
         * Acceleration(Force and Friction)
         * Position offset
         * Triangle Acceleration
         * Spin
         * Support
         **/
        struct BasicParticleNode: public ParticleNode {
            BasicParticleNode(const SoraVector& pos, float life):
            ParticleNode(pos, life),
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
            
            float32 mFriction;
            SoraVector mAcceleration;
            
            float32 mTrigAcc;
            float32 mGravity;
            
            SoraVector mScale;
            SoraVector mScaleVar;
            
            float32 mSpin;
            float32 mSpinVar;
            
            uint32 mColor;
            uint32 mColorVar;
            
            virtual void transform(float dt) {
                ParticleNode::transform(dt);
                
                mSpeed += mAcceleration * dt;
                
                if(mFriction != 0.f) {
                    float angle = mSpeed.Angle();
                    float df = mFriction * dt;
                    mSpeed -= SoraVector(df * cosf(angle),
                                         dt * sinf(angle));
                }
                
                if(mTrigAcc != 0.f)
                    mPosition.Rotate(mTrigAcc * dt);
                
                if(mGravity != 0.f)
                    mPosition.y += mGravity * dt;
                
                mScale += mScaleVar * dt;
                
                if(mSpinVar != 0.f)
                    mSpin += mSpinVar * dt;
                
                if(mColorVar != 0)
                    mColor += mColorVar * dt;
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
        
        class BasicParticleEmitter: public ParticleEmitter {
        public:
            BasicParticleEmitter();
            
            void setParticleLifeTime(float life);
            float getParticleLifeTime() const;
            
            void setSpeed(float speed);
            float getSpeed();
            
            void setSpin(float spin);
            float getSpin() const;
            
            void setScale(float start, float end);
            void setScale(const SoraVector& start, const SoraVector& end);
            SoraVector getScaleStart() const;
            SoraVector getScaleEnd() const;
            
            void setColor(uint32 start, uint32 end);
            uint32 getStartColor() const;
            uint32 getEndColor() const;
                       
            void setFriction(float friction);
            float getFriction() const;
            
            void setTriganleAcceleration(float taccel);
            float getTriangleAcceleration() const;
            
            void setAcceleration(const SoraVector& accel);
            SoraVector getAcceleration() const;
            
            void setGravity(float gravity);
            float getGravity() const;
            
            virtual void emitterUpdate(float dt) = 0;
            virtual void emitBasic(BasicParticleNode* node);
            
        private:
            float mSpeed;
            float mSpin;
            float mFriction;
            float mTrigAcceleration;
            float mGravity;
            float mParticleLifeTime;
            
            SoraVector mStartScale;
            SoraVector mEndScale;
            SoraVector mAcceleration;

            uint32 mStartColor;
            uint32 mEndColor;
        };
        
    } // namespace particle
    
} // namespace sora


#endif
