//
//  SoraBox2dPhysicWorld.h
//  Sora
//
//  Created by Ruiwei Bu on 10/3/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraBox2dPhysicWorld_h
#define Sora_SoraBox2dPhysicWorld_h

#include "SoraPlatform.h"
#include "physics/SoraPhysicWorld.h"
#include "physics/SoraPhysicListener.h"

#include "Box2D/Box2D.h"

#include <list>

namespace sora {
        
    class SoraBox2dPhysicWorld: public SoraPhysicWorld {
    public:
        friend class SoraB2ContactListener;
        
        SoraBox2dPhysicWorld(float gravityX, float gravityY, bool doSleep=true);
        ~SoraBox2dPhysicWorld();
        
        SoraPhysicBody* createBody(const SoraPhysicBodyDef& def);
        void destroyBody(SoraPhysicBody* body);
        
        SoraPhysicJoint* createJoint(const SoraPhysicJointDef& def);
        void destroyJoint(SoraPhysicJoint* joint);
        
        uint32 bodyCount();
        
        void update(float dt);
        
        void addContactListener(SoraPhysicContactListener* listener);
        void delContactListener(SoraPhysicContactListener* listener);

    private:
        void publishContact(const SoraPhysicContactInfo& info, bool start);
        
        b2World* mWorld;
        
        int32 mVelocityIteration;
        int32 mPositionIteration;
        
        float mGravityX;
        float mGravityY;
        
        typedef std::list<SoraPhysicContactListener*> ContactListenerList;
        ContactListenerList mContactListeners;
    };
    
    
    
} // namespace sora

#endif
