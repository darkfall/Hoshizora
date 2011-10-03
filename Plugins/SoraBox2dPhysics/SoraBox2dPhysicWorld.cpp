//
//  SoraBox2dPhysicWorld.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/3/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraBox2dPhysicWorld.h"
#include "SoraBox2dPhysicBody.h"

namespace sora {
        
    SoraBox2dPhysicWorld::SoraBox2dPhysicWorld(float gx, float gy, bool doSleep):
    mWorld(0),
    mVelocityIteration(10), 
    mPositionIteration(8),
    mGravityX(gx),
    mGravityY(gy) {
        mWorld = new b2World(b2Vec2(mGravityX, mGravityY), doSleep);
        sora_assert(mWorld);
    }
    
    SoraBox2dPhysicWorld::~SoraBox2dPhysicWorld() {
        delete mWorld;
    }
    
    SoraPhysicBody* SoraBox2dPhysicWorld::createBody(const SoraPhysicBodyDef& def) const {
        SoraBox2dPhysicBody* body = new SoraBox2dPhysicBody(def, mWorld);
        return body;
    }
    
    void SoraBox2dPhysicWorld::destroyBody(SoraPhysicBody* body) {
        delete body;
    }
    
    void SoraBox2dPhysicWorld::createJoint(SoraPhysicBody* b1, SoraPhysicBody* b2) {
        
    }
    
    uint32 SoraBox2dPhysicWorld::bodyCount() {
        return mWorld->GetBodyCount();
    }
    
    void SoraBox2dPhysicWorld::update(float dt) {
        mWorld->Step(dt, mVelocityIteration, mPositionIteration);
        mWorld->ClearForces();
    }
    
    void SoraBox2dPhysicWorld::addContactListener(SoraPhysicContactListener* listener) {
        mContactListeners.push_back(listener);
    }
    
    void SoraBox2dPhysicWorld::delContactListener(SoraPhysicContactListener* listener) {
        mContactListeners.remove(listener);
    }
    
} // namespace sora
