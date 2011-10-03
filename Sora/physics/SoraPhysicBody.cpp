//
//  SoraPhysicalBody.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraPhysicBody.h"

namespace sora {
    
    SoraPhysicBody::SoraPhysicBody(const SoraPhysicBodyDef& def):
    mDef(def),
    mUserData(0) {
        
    }
    
    SoraPhysicBody::~SoraPhysicBody() {
        
    }
    
    void SoraPhysicBody::setLocalAnchor(const SoraVector& anchor) {
        mLocalAnchor = anchor;
    }
    
    SoraVector SoraPhysicBody::getLocalAnchor() const {
        return mLocalAnchor;
    }
    
    void* SoraPhysicBody::getUserData() const {
        return mUserData;
    }
    
    void SoraPhysicBody::setUserData(void* data) {
        mUserData = data;
    }
    
    const SoraPhysicBodyDef& SoraPhysicBody::getDef() const {
        return mDef;
    }
    
} // namespace sora