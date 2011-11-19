//
//  SoraBox2dPhysicWorld.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/3/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraBox2dPhysicWorld.h"
#include "SoraBox2dPhysicBody.h"
#include "SoraBox2dPhysicJoint.h"

namespace sora {
    
    class SoraB2ContactListener: public b2ContactListener {
    public:
        SoraB2ContactListener(SoraBox2dPhysicWorld* world):
        mWorld(world) { }
        
        void BeginContact(b2Contact* contact) {
            SoraBox2dPhysicBody* fA = (SoraBox2dPhysicBody*)contact->GetFixtureA()->GetBody()->GetUserData();
            SoraBox2dPhysicBody* fB = (SoraBox2dPhysicBody*)contact->GetFixtureB()->GetBody()->GetUserData();
            if(fA && fB) {
                SoraPhysicContactInfo info;
                
                info.mBodyA = fA;
                info.mBodyB = fB;
                b2Vec2 normal = contact->GetManifold()->localNormal;
                info.mNormal = SoraVector(normal.x, normal.y);
                b2Vec2 pos = SoraBox2dPhysicBody::ScaleFrom(contact->GetManifold()->localPoint);
                info.mPosition = SoraVector(pos.x, pos.y);
                info.mIsTouching = contact->IsTouching();
                
                mWorld->publishContact(info, true);
            }
        }
        
        void EndContact(b2Contact* contact) {
            SoraBox2dPhysicBody* fA = (SoraBox2dPhysicBody*)contact->GetFixtureA()->GetBody()->GetUserData();
            SoraBox2dPhysicBody* fB = (SoraBox2dPhysicBody*)contact->GetFixtureB()->GetBody()->GetUserData();
            if(fA && fB) {
                SoraPhysicContactInfo info;
                
                info.mBodyA = fA;
                info.mBodyB = fB;
                b2Vec2 normal = contact->GetManifold()->localNormal;
                info.mNormal = SoraVector(normal.x, normal.y);
                b2Vec2 pos = SoraBox2dPhysicBody::ScaleFrom(contact->GetManifold()->localPoint);
                info.mPosition = SoraVector(pos.x, pos.y);
                info.mIsTouching = contact->IsTouching();
                
                mWorld->publishContact(info, false);
            }
        }
        
    private:
        SoraBox2dPhysicWorld* mWorld;
    };
    
    class SoraB2RaycastReport: public b2RayCastCallback {
    public:
        float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
            if(fixture) {
                mDef.mBody = static_cast<SoraBox2dPhysicBody*>(fixture->GetBody()->GetUserData());
                mDef.mPoint = SoraVector(SoraBox2dPhysicBody::ScaleFrom(point.x), SoraBox2dPhysicBody::ScaleFrom(point.y));
                mDef.mNormal = SoraVector(normal.x, normal.y);
                mDef.mFraction = fraction;
                return fraction;
            }
			return 0.f;
        }
        
        SoraPhysicRaycastResult mDef;
    };
            
    SoraBox2dPhysicWorld::SoraBox2dPhysicWorld(float gx, float gy, bool doSleep):
    mWorld(0),
    mVelocityIteration(10), 
    mPositionIteration(8),
    mGravityX(gx),
    mGravityY(gy) {
        mWorld = new b2World(b2Vec2(mGravityX, mGravityY), doSleep);
        sora_assert(mWorld);
        
        SoraB2ContactListener* listener = new SoraB2ContactListener(this);
        mWorld->SetContactListener(listener);
    }
    
    SoraBox2dPhysicWorld::~SoraBox2dPhysicWorld() {
        delete mWorld;
    }
    
    SoraPhysicBody* SoraBox2dPhysicWorld::createBody(const SoraPhysicBodyDef& def) {
        SoraBox2dPhysicBody* body = new SoraBox2dPhysicBody(def, mWorld);
        return body;
    }
    
    void SoraBox2dPhysicWorld::destroyBody(SoraPhysicBody* body) {
        delete body;
    }
    
    SoraPhysicJoint* SoraBox2dPhysicWorld::createJoint(const SoraPhysicJointDef& def) {
        sora_assert(mWorld);
        sora_assert(def.mType != SoraPhysicJointDef::None);
        
        b2DistanceJointDef b2def;
        b2def.Initialize(static_cast<SoraBox2dPhysicBody*>(def.mBodyA)->mBody,
                         static_cast<SoraBox2dPhysicBody*>(def.mBodyB)->mBody,
                         static_cast<SoraBox2dPhysicBody*>(def.mBodyA)->mBody->GetWorldCenter(),
                         static_cast<SoraBox2dPhysicBody*>(def.mBodyB)->mBody->GetWorldCenter());
        
        b2Joint* joint = mWorld->CreateJoint(&b2def);
        
        SoraBox2dPhysicJoint* myJoint = new SoraBox2dPhysicJoint;
        myJoint->mUserData = joint;
        myJoint->mBodyA = def.mBodyA;
        myJoint->mBodyB = def.mBodyB;
        return myJoint;
    }
    
    void SoraBox2dPhysicWorld::destroyJoint(SoraPhysicJoint* joint) {
        sora_assert(mWorld);
        sora_assert(joint);
        
        mWorld->DestroyJoint(static_cast<b2Joint*>(joint->mUserData));
        delete joint;
    }
    
    uint32 SoraBox2dPhysicWorld::bodyCount() {
        return mWorld->GetBodyCount();
    }
    
    void SoraBox2dPhysicWorld::update(float dt) {
        mWorld->Step(dt, mVelocityIteration, mPositionIteration);
        mWorld->ClearForces();
    }
    
    SoraPhysicRaycastResult SoraBox2dPhysicWorld::rayCast(const SoraVector& start, const SoraVector& dir) {
        SoraB2RaycastReport report;
        
        b2Vec2 p1(SoraBox2dPhysicBody::ScaleTo(start.x), SoraBox2dPhysicBody::ScaleTo(start.y));
        b2Vec2 p2(SoraBox2dPhysicBody::ScaleTo(dir.x), SoraBox2dPhysicBody::ScaleTo(dir.y));
        mWorld->RayCast(&report, p1, p2);
        
        return report.mDef;
    }
    
    void SoraBox2dPhysicWorld::addContactListener(SoraPhysicContactListener* listener) {
        mContactListeners.push_back(listener);
    }
    
    void SoraBox2dPhysicWorld::delContactListener(SoraPhysicContactListener* listener) {
        mContactListeners.remove(listener);
    }
    
    void SoraBox2dPhysicWorld::publishContact(const SoraPhysicContactInfo& info, bool start) {
        if(!mContactListeners.empty()) {
            ContactListenerList::const_iterator it = mContactListeners.begin();
            ContactListenerList::const_iterator end = mContactListeners.end();
            
            for(; it != end; ++it) {
                if(start) {
                    (*it)->onContactBegin(info);
                } else {
                    (*it)->onContactEnd(info);
                }
            }
        }
    }
    
    

    
} // namespace sora
