//
//  SoraBox2dPhysicBody.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/3/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraBox2dPhysicBody.h"
#include "Box2D/Box2D.h"

namespace sora {

    float SoraBox2dPhysicBody::mScale = 50.f;
    
    void SoraBox2dPhysicBody::SetScale(float scale) {
        mScale = scale;
        sora_assert(mScale != 0.f);
    }
    
    float SoraBox2dPhysicBody::GetScale() {
        return mScale;
    }
    
    b2Vec2 SoraBox2dPhysicBody::ScaleTo(const b2Vec2& vec) {
        b2Vec2 result;
        result.x = vec.x / mScale;
        result.y = vec.y / mScale;
        return result;
    }
    
    float SoraBox2dPhysicBody::ScaleTo(float v) {
        return (v / mScale);
    }
    
    float SoraBox2dPhysicBody::ScaleFrom(float v) {
        return (v * mScale);
    }
    
    b2Vec2 SoraBox2dPhysicBody::ScaleFrom(const b2Vec2& vec) {
        b2Vec2 result;
        result.x = vec.x * mScale;
        result.y = vec.y * mScale;
        return result;
    }

    SoraBox2dPhysicBody::SoraBox2dPhysicBody(const SoraPhysicBodyDef& def, b2World* world):
    SoraPhysicBody(def),
    mWorld(world) {
        sora_assert(mWorld);
        
        b2BodyDef bodyDef;
        if(def.mType == SoraPhysicBodyDef::DynamicBody)
            bodyDef.type = b2_dynamicBody;
        else
            bodyDef.type = b2_staticBody;
        
        bodyDef.position = ScaleTo(b2Vec2(def.mPosition.x, def.mPosition.y));
        
        mBody = world->CreateBody(&bodyDef);
        sora_assert(mBody);
        
        mBody->SetUserData(this);
    }
    
    SoraBox2dPhysicBody::~SoraBox2dPhysicBody() {
        if(mBody) {
            mWorld->DestroyBody(mBody);
        }
    }
    
    void SoraBox2dPhysicBody::createFixture(const SoraPhysicFixtureDef& def) {
        sora_assert(mBody);
        
        b2FixtureDef fixtureDef;
        fixtureDef.density = def.mDensity;
        fixtureDef.friction = def.mFriction;
        
        switch(def.mShape.type()) {
            case SoraPhysicShape::ShapeEdge: {
                b2PolygonShape shape;
                shape.SetAsEdge(ScaleTo(b2Vec2(def.mShape.mVertices[0].x, def.mShape.mVertices[0].y)),
                                ScaleTo(b2Vec2(def.mShape.mVertices[1].x, def.mShape.mVertices[1].y)));
                fixtureDef.shape = &shape;
                mBody->CreateFixture(&fixtureDef);
                break;
            }
            
            case SoraPhysicShape::ShapeBox: {
                b2PolygonShape shape;
                shape.SetAsBox(ScaleTo(def.mShape.mVertices[0].x), 
                               ScaleTo(def.mShape.mVertices[0].y),
                               ScaleTo(b2Vec2(def.mShape.mVertices[1].x, def.mShape.mVertices[1].y)),
                               def.mShape.mVertices[2].x);
                fixtureDef.shape = &shape;
                mBody->CreateFixture(&fixtureDef);
                break;
            }
                
            case SoraPhysicShape::ShapeCircle: {
                b2CircleShape shape;
                shape.m_radius = ScaleTo(def.mShape.mVertices[0].x);
                fixtureDef.shape = &shape;
                mBody->CreateFixture(&fixtureDef);
                break;
            }
                
            case SoraPhysicShape::ShapePolygon: {
                b2PolygonShape shape;
                shape.Set((b2Vec2*)def.mShape.mVertices.begin(), def.mShape.mVertices.size());
                fixtureDef.shape = &shape;
                mBody->CreateFixture(&fixtureDef);
                break;
            }
        }
    }
    
    void SoraBox2dPhysicBody::setMass(float mass, float cx, float cy) {
        sora_assert(mBody);
        
        b2MassData massData;
        massData.mass = mass;
        massData.center = ScaleTo(b2Vec2(cx, cy));
        massData.I = 0;
        mBody->SetMassData(&massData);
    }
    
    float SoraBox2dPhysicBody::getMass() const {
        sora_assert(mBody);
        
        return mBody->GetMass();
    }
    
    void SoraBox2dPhysicBody::setSpeed(float x, float y) {
        sora_assert(mBody);
        
        mBody->SetLinearVelocity(b2Vec2(x, y));
    }
    
    SoraVector SoraBox2dPhysicBody::getSpeed() const {
        sora_assert(mBody);
        
        b2Vec2 vec = mBody->GetLinearVelocity();
        return SoraVector(vec.x, vec.y);
    }
    
    void SoraBox2dPhysicBody::setAngularSpeed(float r) {
        sora_assert(mBody);
        
        mBody->SetAngularVelocity(r);
    }
    
    float SoraBox2dPhysicBody::getAngularSpeed() const {
        sora_assert(mBody);
        
        return mBody->GetAngularVelocity();
    }
    
    void SoraBox2dPhysicBody::setTransform(float x, float y, float r) {
        sora_assert(mBody);
        
        mBody->SetTransform(ScaleTo(b2Vec2(x, y)), r);
    }
    
    SoraVector SoraBox2dPhysicBody::getPosition() const {
        sora_assert(mBody);
        
        b2Vec2 vec = ScaleFrom(mBody->GetPosition());
        return SoraVector(vec.x, vec.y);
    }
    
    float SoraBox2dPhysicBody::getRotation() const {
        sora_assert(mBody);
        
        return mBody->GetAngle();
    }
    
    void SoraBox2dPhysicBody::setActive(bool flag) {
        sora_assert(mBody);
        
        mBody->SetActive(flag);
    }
    
    bool SoraBox2dPhysicBody::isActive() const {
        sora_assert(mBody);
        
        return mBody->IsActive();
    }
    
    void SoraBox2dPhysicBody::setSleep(bool flag) {
        sora_assert(mBody);
        
        mBody->SetAwake(!flag);
    }
    
    bool SoraBox2dPhysicBody::isSleep() const {
        sora_assert(mBody);
        
        return mBody->IsAwake();
    }
    
    void SoraBox2dPhysicBody::setBullet(bool flag) {
        sora_assert(mBody);
        
        mBody->SetBullet(flag);
    }
    
    bool SoraBox2dPhysicBody::isBullet() const {
        sora_assert(mBody);
        
        return mBody->IsBullet();
    }
    
    SoraVector SoraBox2dPhysicBody::getCenter() const {
        sora_assert(mBody);
        
        b2Vec2 vec = ScaleFrom(mBody->GetWorldCenter());
        return SoraVector(vec.x, vec.y);
    }
    
    SoraVector SoraBox2dPhysicBody::getLocalCenter() const {
        sora_assert(mBody);
        
        b2Vec2 vec = ScaleFrom(mBody->GetLocalCenter());
        return SoraVector(vec.x, vec.y);
    }
    
    SoraVector SoraBox2dPhysicBody::worldToLocal(const SoraVector& vec) const {
        sora_assert(mBody);
        
        b2Vec2 localvec = mBody->GetLocalPoint(b2Vec2(vec.x, vec.y));
        return SoraVector(localvec.x, localvec.y);
    }
    
    void SoraBox2dPhysicBody::applyForce(float fx, float fy) {
        sora_assert(mBody);
        
        mBody->ApplyForce(mBody->GetWorldVector(b2Vec2(fx, fy)),
                          mBody->GetWorldCenter());
    }
    
    void SoraBox2dPhysicBody::applyForce(float fx, float fy, float px, float py) {
        sora_assert(mBody);
        
        mBody->ApplyForce(mBody->GetWorldVector(b2Vec2(fx, fy)),
                          mBody->GetWorldVector(b2Vec2(px, py)));
    }
    
    void SoraBox2dPhysicBody::applyTorque(float t) {
        sora_assert(mBody);
        
        mBody->ApplyTorque(t);
    }
    
    void SoraBox2dPhysicBody::applyLinearImpulse(float fx, float fy, float px, float py) {
        sora_assert(mBody);
        
        mBody->ApplyLinearImpulse(mBody->GetWorldVector(b2Vec2(fx, fy)),
                                  mBody->GetWorldVector(b2Vec2(px, py)));
    }
                          
    void SoraBox2dPhysicBody::applyAngularImpulse(float p) {
        sora_assert(mBody);
        
        mBody->ApplyAngularImpulse(p);
    }
    
    SoraRect SoraBox2dPhysicBody::getBoundingBox() {
        sora_assert(mBody);
        
        b2AABB box;
        box.upperBound = b2Vec2(-10000.f, -10000.f);
        box.lowerBound = b2Vec2(10000.f, 10000.f);
        
        b2Fixture* fixture = mBody->GetFixtureList();
        while(fixture != NULL) {
            b2AABB tmpBox = fixture->GetAABB();
            box.Combine(box, tmpBox);
            
            fixture = fixture->GetNext();
        }
        
        box.upperBound = ScaleFrom(box.upperBound);
        box.lowerBound = ScaleFrom(box.lowerBound);
        return SoraRect(box.upperBound.x, box.upperBound.y,
                        box.lowerBound.x, box.lowerBound.y);
    }
    
} // namespace sora
