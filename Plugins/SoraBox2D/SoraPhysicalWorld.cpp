#include "SoraPhysicalWorld.h"

namespace sora {

void SoraPhysicalWorld::InitBox2DWorld(b2Vec2 gravity, bool doSleep) {
	physicalWorld = new b2World(gravity, doSleep);

	worldGravity = gravity;
	bInitialized = true;
}

void SoraPhysicalWorld::StepWorld() {
	if(!bInitialized) InitBox2DWorld(b2Vec2(SORA_PHYSICAL_DEFAULT_GRAVITY_X, SORA_PHYSICAL_DEFAULT_GRAVITY_Y));

	physicalWorld->Step(dt, velocityIteration, positionIteration);
	physicalWorld->ClearForces();
}

void SoraPhysicalWorld::StepWorldEx(float32 dt, int32 vIteration, int32 pIteration) {
	if(!bInitialized) InitBox2DWorld(b2Vec2(SORA_PHYSICAL_DEFAULT_GRAVITY_X, SORA_PHYSICAL_DEFAULT_GRAVITY_Y));

	physicalWorld->Step(dt, vIteration, pIteration);
	physicalWorld->ClearForces();
}

b2Body* SoraPhysicalWorld::CreateBody(float32 px, float32 py, const b2Shape& box, bool isDynamic, float32 density) {
	if(!bInitialized) InitBox2DWorld(b2Vec2(SORA_PHYSICAL_DEFAULT_GRAVITY_X, SORA_PHYSICAL_DEFAULT_GRAVITY_Y));

	b2BodyDef bodyDef;
	if(isDynamic) bodyDef.type = b2_dynamicBody;
	else bodyDef.type = b2_staticBody;

	bodyDef.position.Set(px, py);
	b2Body* body = physicalWorld->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;
	fixtureDef.density = density;
	fixtureDef.friction = 0.2f;
	
	body->CreateFixture(&fixtureDef);

	return body;
}

b2Body* SoraPhysicalWorld::CreateBody(float32 px, float32 py, bool bDynamicBody) {
	if(!bInitialized) InitBox2DWorld(b2Vec2(SORA_PHYSICAL_DEFAULT_GRAVITY_X, SORA_PHYSICAL_DEFAULT_GRAVITY_Y));

	b2BodyDef bodyDef;
	if(bDynamicBody) bodyDef.type = b2_dynamicBody;
	else bodyDef.type = b2_staticBody;

	bodyDef.position.Set(px, py);
	b2Body* body = physicalWorld->CreateBody(&bodyDef);
	return body;
}

b2Body* SoraPhysicalWorld::CreateBody(const b2BodyDef& bodyDef, const b2FixtureDef& fixtureDef) {
	if(!bInitialized) InitBox2DWorld(b2Vec2(SORA_PHYSICAL_DEFAULT_GRAVITY_X, SORA_PHYSICAL_DEFAULT_GRAVITY_Y));

	b2Body* body = physicalWorld->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	
	return body;
}

void SoraPhysicalWorld::CreateJoint(const b2JointDef& jdef) {
	physicalWorld->CreateJoint(&jdef);
}

void SoraPhysicalWorld::destroyBody(b2Body* body) {
	physicalWorld->DestroyBody(body);
}

void SoraPhysicalWorld::destroyJoint(b2Joint* joint) {
	physicalWorld->DestroyJoint(joint);
}

} // namespace sora