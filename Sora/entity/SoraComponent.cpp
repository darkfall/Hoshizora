//
//  SoraComponent.cpp
//  Sora
//
//  Created by Robert Bu on 8/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraComponent.h"
#include "SoraLightWeightEntity.h"

namespace sora {
    
    SoraPropertyBase* SoraComponent::getProperty(const std::string& prop) const {
        if(mOwner != 0)
            return mOwner->getPropertyBase(prop);
        else 
            THROW_SORA_EXCEPTION(RuntimeException, "caught component without owner");
    }
    
    
}