#include "SoraPhysicalWorld.h"

namespace sora {

void SoraPhysicalWorld::initBox2DWorld(b2Vec2 gravity, bool doSleep) {
    if(physicalWorld && bInitialized) {
        delete physicalWorld;
    }
	physicalWorld = new b2World(gravity, doSleep);

	worldGravity = gravity;
	bInitialized = true;
    
    if(!bPluginInstalled) {
        SORA->registerPlugin(&plugin);
        bPluginInstalled = true;
    }
}

void SoraPhysicalWorld::stepWorld() {
	if(!bInitialized) return;

	physicalWorld->Step(dt, velocityIteration, positionIteration);
	physicalWorld->ClearForces();
}

void SoraPhysicalWorld::stepWorldEx(float32 dt, int32 vIteration, int32 pIteration) {
	if(!bInitialized) return;

	physicalWorld->Step(dt, vIteration, pIteration);
	physicalWorld->ClearForces();
}

b2Body* SoraPhysicalWorld::createBody(float32 px, float32 py, const b2Shape& box, bool isDynamic, float32 density, float friction) {
	if(!bInitialized) return NULL;

	b2BodyDef bodyDef;
	if(isDynamic) bodyDef.type = b2_dynamicBody;
	else bodyDef.type = b2_staticBody;

	bodyDef.position.Set(pixel2b2cor(px), pixel2b2cor(py));
	b2Body* body = physicalWorld->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	
	body->CreateFixture(&fixtureDef);

	return body;
}
    
    b2Body* SoraPhysicalWorld::getBodyList() {
        if(bInitialized)
            return physicalWorld->GetBodyList();
        return NULL;
    }

b2Body* SoraPhysicalWorld::createBody(float32 px, float32 py, bool bDynamicBody) {
	if(!bInitialized) return NULL;

	b2BodyDef bodyDef;
	if(bDynamicBody) bodyDef.type = b2_dynamicBody;
	else bodyDef.type = b2_staticBody;

	bodyDef.position.Set(px, py);
	b2Body* body = physicalWorld->CreateBody(&bodyDef);
	return body;
}

b2Body* SoraPhysicalWorld::createBody(const b2BodyDef& bodyDef, const b2FixtureDef& fixtureDef) {
	if(!bInitialized) return NULL;

	b2Body* body = physicalWorld->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	
	return body;
}

void SoraPhysicalWorld::createJoint(const b2JointDef& jdef) {
    if(!bInitialized) return;
    
	physicalWorld->CreateJoint(&jdef);
}

void SoraPhysicalWorld::destroyBody(b2Body* body) {
    if(!bInitialized) return;

	physicalWorld->DestroyBody(body);
}

void SoraPhysicalWorld::destroyJoint(b2Joint* joint) {
    if(!bInitialized) return;

	physicalWorld->DestroyJoint(joint);
}
    
    
	// wrappers to help build shapes fast
    b2PolygonShape SoraPhysicalWorld::generateBox(float32 w, float32 h, float32 cx, float32 cy, float32 rot) {
		b2PolygonShape shape;
		shape.SetAsBox(pixel2b2cor(w), pixel2b2cor(h), b2Vec2(pixel2b2cor(cx), pixel2b2cor(cy)), 0.f);
		return shape;
	}
    
    b2CircleShape SoraPhysicalWorld::generateCircle(float32 r) {
		b2CircleShape shape;
		shape.m_radius = pixel2b2cor(r);
		return shape;
	}
    
    b2PolygonShape SoraPhysicalWorld::generatePolygen(b2Vec2* vertices, size_t count) {
		b2PolygonShape shape;
		shape.Set(vertices, count);
		return shape;
	}
    
    b2PolygonShape SoraPhysicalWorld::generateEdge(float32 v1, float32 v2, float32 v3, float32 v4) {
		b2PolygonShape shape;
		b2Vec2 bv1(v1, v2);
		b2Vec2 bv2(v3, v4);
		shape.SetAsEdge(pixel2b2cor(bv1), pixel2b2cor(bv2));
		return shape;
	}


} // namespace sora