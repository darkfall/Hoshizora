//
//  SoraBox2dPhysicJoint.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/3/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraBox2dPhysicJoint.h"
#include "Box2D/Box2D.h"

#include "SoraBox2dPhysicBody.h"

namespace sora {
    
    SoraVector SoraBox2dPhysicJoint::getAnchorA() {
        sora_assert(mUserData);
    
        b2Vec2 vec = SoraBox2dPhysicBody::ScaleFrom(static_cast<b2Joint*>(mUserData)->GetAnchorA());
        return SoraVector(vec.x, vec.y);
    }
    
    SoraVector SoraBox2dPhysicJoint::getAnchorB() {
        sora_assert(mUserData);
        
        b2Vec2 vec = SoraBox2dPhysicBody::ScaleFrom(static_cast<b2Joint*>(mUserData)->GetAnchorB());
        return SoraVector(vec.x, vec.y);
    }
    
} // namespace sora
