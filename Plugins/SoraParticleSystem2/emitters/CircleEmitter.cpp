//
//  CircleEmitter.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/29/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "CircleEmitter.h"

namespace sora {
    
    namespace particle {
        
        CircleEmitter::CircleEmitter(int mode):
        mMode(mode),
        mInterval(0.f),
        mEmitNum(0),
        mCurrInterval(0.f),
        mCurrRadius(0.f),
        mRadiusInterval(F_PI_12) {
            
        }
        
        void CircleEmitter::emitterUpdate(float dt) {
            if(mInterval != 0.f) {
                mCurrInterval += dt;
                if(mCurrInterval >= mInterval) {
                    mCurrInterval = 0.f;
                    doEmit();
                }
            } else
                doEmit();
        }
        
        void CircleEmitter::doEmit() {
            switch(mMode) {
                case MODE_ALL:
                    if(mEmitNum != 0) {
                        float dr = F_PI / mEmitNum;
                        for(int i=0; i<mEmitNum; ++i) {
                            BasicParticleNode* node = new BasicParticleNode(getPosition(), getParticleLifeTime());
                            node->mSpeed.x = getSpeed() * cosf(dr * i);
                            node->mSpeed.y = getSpeed() * sinf(dr * i);
                            emitBasic(node);
                        }
                    }
                    break;
                case MODE_LOOP:
                    if(mEmitNum != 0) {
                        for(int i=0; i<mEmitNum; ++i) {
                            BasicParticleNode* node = new BasicParticleNode(getPosition(), getParticleLifeTime());
                            node->mSpeed.x = getSpeed() * cosf(mCurrRadius * i);
                            node->mSpeed.y = getSpeed() * sinf(mCurrRadius * i);
                            emitBasic(node);
                            
                            mCurrRadius += mRadiusInterval;
                        }
                    }
                    break;
            }
        }
        
        void CircleEmitter::setMode(int mode) {
            mMode = mode;
        }
        
        int CircleEmitter::getMode() const {
            return mMode;
        }
        
        void CircleEmitter::setEmitInterval(float interval) {
            mInterval = interval;
        }
        
        float CircleEmitter::getEmitInterval() const {
            return mInterval;
        }
        
        void CircleEmitter::setRadiusInterval(float r) {
            mRadiusInterval = r;
        }
        
        float CircleEmitter::getRadiusInterval() const {
            return mRadiusInterval;
        }
        
        void CircleEmitter::setEmitNum(int num) {
            mEmitNum = num;
        }
        
        int CircleEmitter::getEmitNum() const {
            return mEmitNum;
        }
        
    } // namespace particle
    
} // namespace sora
