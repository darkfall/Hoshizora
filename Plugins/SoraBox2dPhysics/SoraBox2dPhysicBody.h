//
//  SoraBox2dPhysicBody.h
//  Sora
//
//  Created by Ruiwei Bu on 10/3/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraBox2dPhysicBody_h
#define Sora_SoraBox2dPhysicBody_h

#include "physics/SoraPhysicBody.h"

#include "Box2D/Box2D.h"

namespace sora {
        
    class SoraBox2dPhysicBody: public SoraPhysicBody {
    public:
        friend class SoraBox2dPhysicWorld;
        
        SoraBox2dPhysicBody(const SoraPhysicBodyDef& def, b2World* world);
        ~SoraBox2dPhysicBody();
        
        void createFixture(const SoraPhysicFixtureDef& def);
        
        void setMass(float mass, float cx, float cy);
        float getMass() const;
        
        void setSpeed(float x, float y);
        SoraVector getSpeed() const;
        
        void setAngularSpeed(float r);
        float getAngularSpeed() const;
        
        void setTransform(float x, float y, float r);
        float getRotation() const;
        SoraVector getPosition() const;

        void setActive(bool flag);
        bool isActive() const;
        
        void setSleep(bool flag);
        bool isSleep() const;
        
        void setBullet(bool flag);
        bool isBullet() const;
                
        SoraVector getCenter() const;
        SoraVector getLocalCenter() const;
        SoraVector worldToLocal(const SoraVector& vec) const;
        
        void applyForce(float fx, float fy);
        void applyForce(float fx, float fy, float px, float py);
        
        void applyTorque(float t);
        void applyLinearImpulse(float fx, float fy, float px, float py);
        void applyAngularImpulse(float p);
        
        SoraRect getBoundingBox();
        
        static void SetScale(float scale);
        static float GetScale();
        static b2Vec2 ScaleTo(const b2Vec2& vec);
        static float ScaleTo(float v);
        static b2Vec2 ScaleFrom(const b2Vec2& vec);
        static float ScaleFrom(float v);
        
        static float mScale;
        
    private:
        b2Body* mBody;
        b2World* mWorld;
    };
    
} // namespace  sora


#endif
