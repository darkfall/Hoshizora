//
//  SoraPhysicBodyDef.h
//  Sora
//
//  Created by Ruiwei Bu on 10/3/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPhysicBodyDef_h
#define Sora_SoraPhysicBodyDef_h

#include "SoraPhysicShape.h"
#include "SoraMath.h"

namespace sora {
    
    struct SoraPhysicFixtureDef {
        float mFriction;
        float mDensity;
        
        SoraPhysicShape mShape;
        
        SoraPhysicFixtureDef():
        mFriction(0.f),
        mDensity(1.f) { }
        
        SoraPhysicFixtureDef(const SoraPhysicShape& shape, float friction=0.f, float density=1.0f):
        mFriction(friction),
        mDensity(density),
        mShape(shape) { }
    };
    
    struct SoraPhysicBodyDef {
        enum BodyType {
            DynamicBody,
            StaticBody,
        };
        
        BodyType mType;
        SoraVector mPosition;
        
        SoraPhysicBodyDef():
        mType(DynamicBody) { }
        
        SoraPhysicBodyDef(BodyType type, const SoraVector& pos):
        mType(type),
        mPosition(pos) { }
    };
    
} // namespace sora



#endif
