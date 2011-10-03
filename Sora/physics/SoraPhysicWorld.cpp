//
//  SoraPhysicWorld.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/3/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraPhysicWorld.h"
#include "SoraCore.h"

namespace sora {
    
    SoraPhysicBody* SoraPhysicWorld::CreateBody(const SoraPhysicBodyDef& def) {
        SoraPhysicWorld* world = SoraCore::Instance()->getPhysicWorld();
        if(world)
            return world->createBody(def);
        return 0;
    }
    
    void SoraPhysicWorld::DestroyBody(SoraPhysicBody* body) {
        SoraPhysicWorld* world = SoraCore::Instance()->getPhysicWorld();
        if(world)
            world->destroyBody(body);
    }
    
    void SoraPhysicWorld::CreateJoint(SoraPhysicBody* b1, SoraPhysicBody* b2) {
        SoraPhysicWorld* world = SoraCore::Instance()->getPhysicWorld();
        if(world)
            world->createJoint(b1, b2);
    }
    
    void SoraPhysicWorld::AddContactListener(SoraPhysicContactListener* listener) {
        SoraPhysicWorld* world = SoraCore::Instance()->getPhysicWorld();
        if(world)
            world->addContactListener(listener);
    }
    
    void SoraPhysicWorld::DelContactListener(SoraPhysicContactListener* listener) {
        SoraPhysicWorld* world = SoraCore::Instance()->getPhysicWorld();
        if(world)
            world->delContactListener(listener);
    }
    
} // namespace sora
