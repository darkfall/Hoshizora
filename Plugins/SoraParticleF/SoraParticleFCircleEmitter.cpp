//
//  SoraParticleFCircleEmitter.cpp
//  SoraF
//
//  Created by Robert Bu on 2/27/12.
//  Copyright (c) 2012 Game Master Studio. All rights reserved.
//

#include "SoraParticleFCircleEmitter.h"

namespace sora {
    
    const SoraString& SoraParticleFCircleEmitter::getName() const {
        static SoraString name = "CircleEmitter";
        return name;
    }
    
    void SoraParticleFCircleEmitter::doEmit(SoraParticleFNode& node, float tratio, uint32 em) {
        
    }
    
}
