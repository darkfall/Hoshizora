//
//  SoraBasicParticleEmitter.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/29/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraBasicParticleEmitter.h"
#include "SoraCore.h"

namespace sora {
    
    namespace particle {
        
        BasicParticleEmitter::BasicParticleEmitter():
        mSpeed(0.f),
        mSpin(0.f),
        mFriction(0.f),
        mTrigAcceleration(0.f),
        mGravity(0.f),
        mParticleLifeTime(1.f),
        mStartScale(SoraVector(1.f, 1.f)),
        mEndScale(SoraVector(1.f, 1.f)),
        mStartColor(0xFFFFFFFF),
        mEndColor(0xFFFFFFFF) {
            
        }
        
        void BasicParticleEmitter::setSpeed(float speed) {
            mSpeed = speed;
        }
        
        float BasicParticleEmitter::getSpeed() {
            return mSpeed;
        }
        
        void BasicParticleEmitter::setSpin(float spin) {
            mSpin = spin;
        }
        
        float BasicParticleEmitter::getSpin() const {
            return mSpin;
        }
        
        void BasicParticleEmitter::setScale(float start, float end) {
            mStartScale.x = mStartScale.y = start;
            mEndScale.x = mEndScale.y = end;
        }
        
        void BasicParticleEmitter::setScale(const SoraVector& start, const SoraVector& end) {
            mStartScale = start;
            mEndScale = end;
        }
        
        SoraVector BasicParticleEmitter::getScaleStart() const {
            return mStartScale;
        }
        
        SoraVector BasicParticleEmitter::getScaleEnd() const {
            return mEndScale;
        }
        
        void BasicParticleEmitter::setColor(uint32 start, uint32 end) {
            mStartColor = start;
            mEndColor = end;
        }
        
        uint32 BasicParticleEmitter::getStartColor() const {
            return mStartColor;
        }
        
        uint32 BasicParticleEmitter::getEndColor() const {
            return mEndColor;
        }
        
        void BasicParticleEmitter::setFriction(float friction) {
            mFriction = friction;
        }
        
        float BasicParticleEmitter::getFriction() const {
            return mFriction;
        }
        
        void BasicParticleEmitter::setTriganleAcceleration(float taccel) {
            mTrigAcceleration = taccel;
        }
        
        float BasicParticleEmitter::getTriangleAcceleration() const {
            return mTrigAcceleration;
        }
        
        void BasicParticleEmitter::setAcceleration(const SoraVector& accel) {
            mAcceleration = accel;
        }
        
        SoraVector BasicParticleEmitter::getAcceleration() const {
            return mAcceleration;
        }
        
        void BasicParticleEmitter::setGravity(float gravity) {
            mGravity = gravity;
        }
        
        float BasicParticleEmitter::getGravity() const {
            return mGravity;
        }
        
        float BasicParticleEmitter::getParticleLifeTime() const {
            return mParticleLifeTime;
        }
        
        void BasicParticleEmitter::setParticleLifeTime(float life) {
            mParticleLifeTime = life;
        }
        
        void BasicParticleEmitter::emitBasic(BasicParticleNode* node) {
            node->mFriction = mFriction;
            node->mAcceleration = mAcceleration;
            node->mScale = mStartScale;
            node->mScaleVar = (mStartScale - mEndScale) / getParticleLifeTime();

            node->mSpinVar = mSpin;
            node->mColor = mStartColor;
            node->mColorVar = (uint32)((mStartColor - mEndColor) / getParticleLifeTime());
            
            node->mTrigAcc = mTrigAcceleration;
            node->mGravity = mGravity;
            
            ParticleEmitter::emit(node);
        }
        
    } // namespace particle
    
} // namespace sora
