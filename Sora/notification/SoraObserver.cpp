//
//  SoraObserver.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/11/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraObserver.h"

namespace sora {
    
    SoraAbstractObserver::SoraAbstractObserver() {
        
    }
    
    SoraAbstractObserver::SoraAbstractObserver(const SoraAbstractObserver& observer) {
        
    }
    
    SoraAbstractObserver::~SoraAbstractObserver() {
        
    }
    
    SoraAbstractObserver& SoraAbstractObserver::operator =(const SoraAbstractObserver& rhs) {
        return *this;
    }
    
} // namespace sora