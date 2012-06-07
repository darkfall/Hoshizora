//
//  SoraParticleFBasicEmitter.h
//  SoraF
//
//  Created by Robert Bu on 2/27/12.
//  Copyright (c) 2012 Game Master Studio. All rights reserved.
//

#ifndef SoraF_SoraParticleFBasicEmitter_h
#define SoraF_SoraParticleFBasicEmitter_h

#include "SoraParticleFEmitter.h"

namespace sora {
    
    class SoraParticleFBasicEmitter: public SoraParticleFEmitter {
    public:
        SoraParticleFBasicEmitter();
        virtual ~SoraParticleFBasicEmitter();
        
    public:
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraVector, MaxAngle, m);
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraVector, MinAngle, m);
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(float, MaxSpeed, m);
        SORA_CLASS_DEF_FIELD_SET_GET_P(float, MinSpeed, m);
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraColorRGBA, StartColor, m);
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraColorRGBA, EndColor, m);
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraVector3, MaxSpin, m);
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraVector3, MinSpin, m);
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(float, MaxLifeTime, m);
        SORA_CLASS_DEF_FIELD_SET_GET_P(float, MinLifeTime, m);
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraVector3, StartScale, m);
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraVector3, EndScale, m);
        
    public:
        virtual const SoraString& getName() const;
        virtual void doEmit(SoraParticleFNode& node, float tratio, uint32 em);
    };
    
}

#endif
