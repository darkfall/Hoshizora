#include "SoraPhysicalObject.h"

namespace sora {

	SoraPhysicalObject::SoraPhysicalObject(float32 posx, float32 posy, const b2Shape& shape, float density, bool dynamicBody) {
		body = SoraPhysicalWorld::Instance()->createBody(PHYSICAL_WORLD->pixel2b2cor(posx), PHYSICAL_WORLD->pixel2b2cor(posy), shape, dynamicBody, density);
		if(body) {
            localAnchor.Set(0.f, 0.f);
        } else
            DebugPtr->log("Error craeing physical body");
        setType(OBJ_PHYSICAL);
	}

	SoraPhysicalObject::SoraPhysicalObject(const b2BodyDef& bodyDef, const b2FixtureDef& fixtureDef) {
		body = SoraPhysicalWorld::Instance()->createBody(bodyDef, fixtureDef);
		if(body) {
            localAnchor.Set(0.f, 0.f);
        } else
            DebugPtr->log("Error craeing physical body");
        setType(OBJ_PHYSICAL);
	}

	SoraPhysicalObject::~SoraPhysicalObject() {
        if(body)
            SoraPhysicalWorld::Instance()->destroyBody(body);
	}

	void SoraPhysicalObject::createFixture(const b2FixtureDef& fdef) {
		if(body)
            body->CreateFixture(&fdef);
	}

	void SoraPhysicalObject::destroyFixture(b2Fixture* fdef) {
		if(body)
            body->DestroyFixture(fdef);
	}

	void SoraPhysicalObject::createFixture(const b2Shape& shape, float32 density) {
		if(body)
            body->CreateFixture(&shape, density);
	}

	void SoraPhysicalObject::setSpeed(float32 x, float32 y) {
		if(body)
            body->SetLinearVelocity(b2Vec2(x, y));
	}

	float32 SoraPhysicalObject::getSpeedX() const {
		if(body)
            return body->GetLinearVelocity().x;
        return 0.f;
	}
	
	float32 SoraPhysicalObject::getSpeedY() const {
		if(body)
            return body->GetLinearVelocity().y;
        return 0.f;
	}

	b2Vec2 SoraPhysicalObject::getSpeed() const {
		if(body)
            return body->GetLinearVelocity();
        return b2Vec2(0.f, 0.f);
	}

	void SoraPhysicalObject::setAngularSpeed(float32 omega) {
		if(body)
            body->SetAngularVelocity(omega);
	}

	float32 SoraPhysicalObject::getAngularSpeed() const {
		if(body)
            return body->GetAngularVelocity();
        return 0.f;
	}

	void SoraPhysicalObject::setMass(float32 mass) {
        if(!body)
            return;
        
		b2MassData massData;
		massData.mass = mass;
		massData.center = b2Vec2(body->GetLocalCenter().x, body->GetLocalCenter().y);
		massData.I = 0;
		body->SetMassData(&massData);
	}

	float32 SoraPhysicalObject::getMass() const {
		if(body)
            return body->GetMass();
        return 0.f;
	}

	void SoraPhysicalObject::applyForce(float32 fx, float32 fy) {
		if(body)
            body->ApplyForce(body->GetWorldVector(b2Vec2(fx, fy)), body->GetWorldPoint(b2Vec2(0.f, 0.f)));
	}

	void SoraPhysicalObject::applyForceF(float32 fx, float32 fy, float32 px, float32 py) {
        if(!body)
            return;
        
		b2Vec2 v = body->GetWorldPoint(b2Vec2(px, py));
		body->ApplyForce(body->GetWorldVector(b2Vec2(fx, fy)), v);
	}

	float32 SoraPhysicalObject::getAngle() const {
		if(body)
            return body->GetAngle();
        return 0.f;
	}

	b2Vec2 SoraPhysicalObject::getPosition() const {
        if(!body)
            return b2Vec2(0.f, 0.f);;
        
		b2Vec2 pos = body->GetPosition();
		return b2Vec2(PHYSICAL_WORLD->b2cor2pixel(pos.x), PHYSICAL_WORLD->b2cor2pixel(pos.y));
	}

	float32 SoraPhysicalObject::getPositionX() const {
		if(body)
            return PHYSICAL_WORLD->b2cor2pixel(body->GetPosition().x);
        return 0.f;
	}

	float32 SoraPhysicalObject::getPositionY() const {
		if(body)
            return PHYSICAL_WORLD->b2cor2pixel(body->GetPosition().y);
        return 0.f;
	}

	void SoraPhysicalObject::setBullet(bool flag) {
		if(body)
            body->SetBullet(flag);
	}

	bool SoraPhysicalObject::isBullet() {
		if(body)
            return body->IsBullet();
        return false;
	}

	void SoraPhysicalObject::setSleep(bool flag) {
		if(body)
            body->SetAwake(flag);
	}

	bool SoraPhysicalObject::isSleep() {
		if(body)
            return body->IsAwake();
        return false;
	}

	void SoraPhysicalObject::setActive(bool flag) {
		if(body)
            body->SetActive(flag);
	}

	bool SoraPhysicalObject::isActive() {
		if(body)
            return body->IsActive();
        return false;
	}

	b2Body* SoraPhysicalObject::getBody() const {
		if(body)
            return body;
        return NULL;
	}

	void SoraPhysicalObject::setLocalAnchor(float32 x, float32 y) {
		localAnchor.Set(x, y);
	}

	float32 SoraPhysicalObject::getLocalAnchorX() const {
		return localAnchor.x;
	}

	float32 SoraPhysicalObject::getLocalAnchorY() const {
		return localAnchor.y;
	}

	b2Vec2 SoraPhysicalObject::getLocalAnchor() const {
		return localAnchor;
	}

	void SoraPhysicalObject::add(SoraObject* obj) {
        if(!body)
            THROW_SORA_EXCEPTION("Body not available");
        
		// if is a physical object, connect the body to a joint
		if(obj->getType() == OBJ_PHYSICAL) {
			SoraPhysicalObject* py = static_cast<SoraPhysicalObject*>(obj);
			
			b2RevoluteJointDef jdef;
			jdef.bodyA = body;
			jdef.bodyB = py->getBody();
			jdef.localAnchorA = getLocalAnchor();
			jdef.localAnchorB = py->getLocalAnchor();

			SoraPhysicalWorld::Instance()->createJoint(jdef);
		}
        SoraObject::add(obj);
	}

/*	void SoraPhysicalObject::addlua(LuaPlus::LuaObject p) {
		if(p.IsUserData())
			add(static_cast<SoraObject*>(*(void**)p.GetUserData()));
	}
*/
	// lua wrappers
	SoraPhysicalObject::SoraPhysicalObject(float32 px, float32 py, int bDynamic) {
		body = SoraPhysicalWorld::Instance()->createBody(PHYSICAL_WORLD->pixel2b2cor(px), PHYSICAL_WORLD->pixel2b2cor(py), bDynamic>0?true:false);
		if(body) {
            localAnchor.Set(0.f, 0.f);
        } else
            DebugPtr->log("Error craeing physical body");
        setType(OBJ_PHYSICAL);
	}

	void SoraPhysicalObject::setAsBox(float32 w, float32 h, float32 density) {
		b2PolygonShape s = PHYSICAL_WORLD->generateBox(w, h);
		createFixture(s, density);
	}

	void SoraPhysicalObject::setAsCircle(float32 r, float32 density) {
		b2CircleShape s = PHYSICAL_WORLD->generateCircle(r);
		createFixture(s, density);
	}
} // namespace sora