//
//  SoraParticleFBasicEmitter.cpp
//  SoraF
//
//  Created by Robert Bu on 2/27/12.
//  Copyright (c) 2012 Game Master Studio. All rights reserved.
//

#include "SoraParticleFBasicEmitter.h"
#include "SoraParticleFSystem.h"

#include "SoraRandomizer.h"

namespace sora {
    
    SoraParticleFBasicEmitter::SoraParticleFBasicEmitter() {
        
    }

    SoraParticleFBasicEmitter::~SoraParticleFBasicEmitter() {
        
    }
    
    const SoraString& SoraParticleFBasicEmitter::getName() const {
        static SoraString name = "Basic";
        return name;
    }
    
    void SoraParticleFBasicEmitter::doEmit(SoraParticleFNode& node, float tratio, uint32 em) {
        float lifetime = SoraRandomizer::RandomFloat(getMinLifeTime(), getMaxLifeTime());
        
        node.life_time = lifetime;
        node.current_life_time = 0.f;
        
        node.speed = SoraRandomizer::RandomFloat(getMinSpeed(), getMaxSpeed());
        node.speed_var = 0;
        
        node.angle = SoraRandomizer::RandomVector(getMinAngle(), getMaxAngle());
        
        node.color = getStartColor();
        node.color_var = (getEndColor() - getStartColor()) / lifetime;

        node.transform.setRotation(0, 0, 0);
        node.transform.setScale(getStartScale());
        
        node.scale_var = (getEndScale() - getStartScale()) / lifetime;
        
        node.transform.setPosition(getPosition() + getSystem()->getPosition());
    }
}
