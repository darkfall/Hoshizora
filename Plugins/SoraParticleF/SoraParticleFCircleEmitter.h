//
//  SoraPartcileFCircleEmitter.h
//  SoraF
//
//  Created by Robert Bu on 2/27/12.
//  Copyright (c) 2012 Game Master Studio. All rights reserved.
//

#ifndef SoraF_SoraPartcileFCircleEmitter_h
#define SoraF_SoraPartcileFCircleEmitter_h

#include "SoraParticleFBasicEmitter.h"

namespace sora {
    
    class SoraParticleFCircleEmitter: public SoraParticleFBasicEmitter {
    public:
        SoraParticleFCircleEmitter();
        virtual ~SoraParticleFCircleEmitter();
        
    public:
        virtual const SoraString& getName() const;
        virtual void doEmit(SoraParticleFNode& node, float tratio, uint32 em);
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(uint32, Radius, m);
    
    public:
        
    };
    
}


#endif
