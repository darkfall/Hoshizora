//
//  SoraBasicParticleModifier.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/29/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraBasicParticleModifier.h"
#include "SoraParticleNode.h"
#include "SoraCore.h"

namespace sora {
    
    namespace particle {
        
        BasicParticleModifier::BasicParticleModifier():
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
        
        void BasicParticleModifier::setSpeed(float speed) {
            mSpeed = speed;
        }
        
        float BasicParticleModifier::getSpeed() {
            return mSpeed;
        }
        
        void BasicParticleModifier::setSpin(float spin) {
            mSpin = spin;
        }
        
        float BasicParticleModifier::getSpin() const {
            return mSpin;
        }
        
        void BasicParticleModifier::setScale(float start, float end) {
            mStartScale.x = mStartScale.y = start;
            mEndScale.x = mEndScale.y = end;
        }
        
        void BasicParticleModifier::setScale(const SoraVector& start, const SoraVector& end) {
            mStartScale = start;
            mEndScale = end;
        }
        
        SoraVector BasicParticleModifier::getScaleStart() const {
            return mStartScale;
        }
        
        SoraVector BasicParticleModifier::getScaleEnd() const {
            return mEndScale;
        }
        
        void BasicParticleModifier::setColor(uint32 start, uint32 end) {
            mStartColor = start;
            mEndColor = end;
        }
        
        uint32 BasicParticleModifier::getStartColor() const {
            return mStartColor;
        }
        
        uint32 BasicParticleModifier::getEndColor() const {
            return mEndColor;
        }
        
        void BasicParticleModifier::setFriction(float friction) {
            mFriction = friction;
        }
        
        float BasicParticleModifier::getFriction() const {
            return mFriction;
        }
        
        void BasicParticleModifier::setTriganleAcceleration(float taccel) {
            mTrigAcceleration = taccel;
        }
        
        float BasicParticleModifier::getTriangleAcceleration() const {
            return mTrigAcceleration;
        }
        
        void BasicParticleModifier::setAcceleration(const SoraVector& accel) {
            mAcceleration = accel;
        }
        
        SoraVector BasicParticleModifier::getAcceleration() const {
            return mAcceleration;
        }
        
        void BasicParticleModifier::setGravity(float gravity) {
            mGravity = gravity;
        }
        
        float BasicParticleModifier::getGravity() const {
            return mGravity;
        }
        
        float BasicParticleModifier::getParticleLifeTime() const {
            return mParticleLifeTime;
        }
        
        void BasicParticleModifier::setParticleLifeTime(float life) {
            mParticleLifeTime = life;
        }
        
        void BasicParticleModifier::modify(ParticleNode* node) {
            node->mFriction = mFriction;
            node->mAcceleration = mAcceleration;
            node->mScale = mStartScale;
            node->mScaleVar = (mStartScale - mEndScale) / getParticleLifeTime();

            node->mSpinVar = mSpin;
            node->mColor = mStartColor;
            node->mColorVar = (uint32)((mStartColor - mEndColor) / getParticleLifeTime());
            
            node->mTrigAcc = mTrigAcceleration;
            node->mGravity = mGravity;
        }
        
    } // namespace particle
    
} // namespace sora
