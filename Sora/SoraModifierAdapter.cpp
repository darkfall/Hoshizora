//
//  SoraImageEffectAutomator.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/11/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraModifierAdapter.h"

namespace sora {
    
    SoraAbstractModiferAdapter::Members SoraAbstractModiferAdapter::members;
    
    SoraAbstractModiferAdapter::SoraAbstractModiferAdapter() {
        
    }
    
    SoraAbstractModiferAdapter::~SoraAbstractModiferAdapter() {
    }
        
    void SoraAbstractModiferAdapter::insert() {
        members.push_back(this);
    }
    
    void SoraAbstractModiferAdapter::remove() {
        members.remove(this);
    }
} // namespace sora