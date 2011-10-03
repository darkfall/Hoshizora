//
//  SoraPhysicalWorld.h
//  Sora
//
//  Created by Ruiwei Bu on 10/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPhysicalWorld_h
#define Sora_SoraPhysicalWorld_h

#include "SoraPlatform.h"
#include "SoraMath.h"

namespace sora {
    
    class SoraPhysicBody;
    class SoraPhysicBodyDef;
    class SoraPhysicContactListener;
    
    class SoraPhysicWorld {
    public:
        virtual ~SoraPhysicWorld() {}
        
        virtual SoraPhysicBody* createBody(const SoraPhysicBodyDef& def) const = 0;
        virtual void destroyBody(SoraPhysicBody* body) = 0;
        
        virtual void createJoint(SoraPhysicBody* b1, SoraPhysicBody* b2) = 0;
        
        virtual uint32 bodyCount() = 0;
        
        virtual void update(float dt) = 0;
        
        virtual void addContactListener(SoraPhysicContactListener* listener) = 0;
        virtual void delContactListener(SoraPhysicContactListener* listener) = 0;
        
        static SoraPhysicBody* CreateBody(const SoraPhysicBodyDef& def);
        static void            DestroyBody(SoraPhysicBody* body);
        static void            CreateJoint(SoraPhysicBody* b1, SoraPhysicBody* b2);
        static void            AddContactListener(SoraPhysicContactListener* listener);
        static void            DelContactListener(SoraPhysicContactListener* listener);
    };
    
} // namespace sora



#endif
