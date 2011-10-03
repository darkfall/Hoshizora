//
//  SoraPhysicJoint.h
//  Sora
//
//  Created by Ruiwei Bu on 10/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPhysicJoint_h
#define Sora_SoraPhysicJoint_h

#include "SoraPlatform.h"

namespace sora {
    
    class SoraPhysicBody;
    
    struct SoraPhysicJoint {
        SoraPhysicBody* mBody1;
        SoraPhysicBody* mBody2;
    };
    
} // namespace sora

#endif
