//
//  SoraPhysicListener.h
//  Sora
//
//  Created by Ruiwei Bu on 10/3/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPhysicListener_h
#define Sora_SoraPhysicListener_h

#include "SoraPlatform.h"

namespace sora {
    
    class SoraPhysicBody;
    
    class SoraPhysicContactInfo {
        SoraPhysicBody* mBody1;
        SoraPhysicBody* mBody2;
        
        // optional part
        // may not be filled depends on physical engine
        SoraVector mNormal;
        SoraVector mPosition;
        
        bool mIsTouching;
    };
    
    class SoraPhysicContactListener {
    public:
        virtual void contactBegin(const SoraPhysicContactInfo& info) = 0;
        virtual void contactEnd(const SoraPhysicContactInfo& info) = 0
    };
    
    
} // namespace sora


#endif
