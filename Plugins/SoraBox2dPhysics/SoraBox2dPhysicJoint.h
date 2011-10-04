//
//  SoraBox2dPhysicJoint.h
//  Sora
//
//  Created by Ruiwei Bu on 10/3/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraBox2dPhysicJoint_h
#define Sora_SoraBox2dPhysicJoint_h

#include "SoraPlatform.h"
#include "physics/SoraPhysicJoint.h"

namespace sora {
    
    struct SoraBox2dPhysicJoint: public SoraPhysicJoint {
        SoraVector getAnchorA();
        SoraVector getAnchorB();
    };


} // namespace sora


#endif
