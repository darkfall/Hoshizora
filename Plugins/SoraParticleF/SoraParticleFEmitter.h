//
//  SoraParticleFEmitter.h
//  SoraF
//
//  Created by Robert Bu on 2/26/12.
//  Copyright (c) 2012 Game Master Studio. All rights reserved.
//

#ifndef SoraF_SoraParticleFEmitter_h
#define SoraF_SoraParticleFEmitter_h

#include "SoraParticleFNode.h"
#include "SoraAction.h"
#include "SoraString.h"
#include "SoraPreDeclare.h"
#include "SoraSprite.h"
#include "SoraRect.h"

#include <vector>

namespace sora {
    
    class SoraParticleFSystem;
    class SoraParticleFRenderer;
    
    // base class 
    class SoraParticleFEmitter: public SoraFiniteAction {
    public:
        SoraParticleFEmitter();
        SoraParticleFEmitter(uint32 num, float duration, float interval, uint32 repeat_time, const SoraString& renderer="default");
        virtual ~SoraParticleFEmitter();
        
    public:
        // SoraAction
        
        virtual void startWithTarget(SoraObject* system);
        virtual void step(float dt);
        virtual bool isDone() const;
                
    public:
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraParticleFSystem*, System, m);

        // emit [EmissionNum] partcile in [EmissionTime] period
        // and wait [EmissionInterval] then repeat [EmissionRepeatTime] times;
        SORA_CLASS_DEF_FIELD_SET_GET_P(uint32, EmissionNum, m);
        SORA_CLASS_DEF_FIELD_SET_GET_P(int32, EmissionRepeatTime, m);
        SORA_CLASS_DEF_FIELD_SET_GET_P(float, EmissionInterval, m);
        SORA_CLASS_DEF_FIELD_SET_GET_P(float, EmissionDuration, m);
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraVector3, Position, m);
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraString, RendererName, m);

    public:
        // emiiter
        virtual const SoraString& getName() const = 0;

        virtual void doEmit(SoraParticleFNode& node, float tratio, uint32 em) = 0;
        
        typedef std::vector<SoraParticleFNode> ParticleList;
        
        ParticleList& getParticles();
        const ParticleList& getParticles() const;
        
    private:
        uint32 mCurrentRepeatTime;
        float  mToEmitTime;
        
        ParticleList mParticles; 
        
    };
    
}

#endif
