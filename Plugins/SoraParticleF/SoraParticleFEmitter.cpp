//
//  SoraParticleFEmitter.cpp
//  SoraF
//
//  Created by Robert Bu on 2/26/12.
//  Copyright (c) 2012 Game Master Studio. All rights reserved.
//

#include "SoraParticleFEmitter.h"
#include "SoraParticleFSystem.h"
#include "SoraForEach.h"

namespace sora {
    
    SoraParticleFEmitter::SoraParticleFEmitter():
    mSystem(0),
    mEmissionNum(0),
    mEmissionRepeatTime(0),
    mEmissionInterval(0),
    mEmissionDuration(0),
    mCurrentRepeatTime(0),
    mToEmitTime(0),
    mRendererName("default") {
        
    }
    
    SoraParticleFEmitter::SoraParticleFEmitter(uint32 num, float duration, float interval, uint32 repeat_time, const SoraString& renderer):
    mSystem(0),
    mEmissionNum(num),
    mEmissionRepeatTime(repeat_time),
    mEmissionDuration(duration),
    mEmissionInterval(interval),
    mCurrentRepeatTime(0),
    mToEmitTime(0),
    mRendererName(renderer) {
    }
    
    SoraParticleFEmitter::~SoraParticleFEmitter() {
        
    }
    
    SoraParticleFEmitter::ParticleList& SoraParticleFEmitter::getParticles() {
        return mParticles;
    }
    
    const SoraParticleFEmitter::ParticleList& SoraParticleFEmitter::getParticles() const {
        return mParticles;
    }
    
    void SoraParticleFEmitter::startWithTarget(SoraObject* obj) {
        mSystem = dynamic_cast<SoraParticleFSystem*>(obj);
        
        mCurrentRepeatTime = 0;
        this->setDuration(mEmissionDuration + mEmissionInterval);

        // mRenderer = mSystem->getRenderer(mRendererName);
        
        SoraAction::startWithTarget(obj);
    }
    
    bool is_particle_died(SoraParticleFNode& node) {
        return node.isDone();
    }
    
    void SoraParticleFEmitter::step(float dt) {
        SoraFiniteAction::step(dt);
        
        if(mSystem) {
            float current_time = getCurrentTime();
            
            if(current_time < mEmissionDuration) {
                
                mToEmitTime += dt;
                if(mToEmitTime > mEmissionDuration / mEmissionNum) {
                    uint32 em = mToEmitTime * mEmissionNum / mEmissionDuration;
                    for(uint32 i = 0;
                        i < em;
                        ++i) {
                        SoraParticleFNode node;
                        this->doEmit(node, getCurrentTime() / getDuration(), em);
                        
                        mParticles.push_back(node);
                    }
                    mToEmitTime = 0;
                }
                
            } else if(current_time > mEmissionDuration + mEmissionInterval) {
                if(mEmissionRepeatTime != -1) {
                    ++mCurrentRepeatTime;
                }
                // restart finite action
                SoraFiniteAction::startWithTarget(this->getTarget());
                
            } // else do nothing, wait for next loop
            
            
            mParticles.erase(std::remove_if(mParticles.begin(),
                                            mParticles.end(),
                                            is_particle_died),
                             mParticles.end());
        }
        
    }
    
    bool SoraParticleFEmitter::isDone() const {
        return !mSystem ||
                (mEmissionRepeatTime != -1 &&
                mCurrentRepeatTime >= mEmissionRepeatTime);
    }
    
}
