#ifndef SORA_PHYSICAL_OBJECT_H_
#define SORA_PHYSICAL_OBJECT_H_

#include "SoraObject.h"
#include "SoraPhysicalWorld.h"

namespace sora {

	// wrappers to help build shapes fast
	inline b2PolygonShape generateBox(float32 w, float32 h, float32 cx=0.f, float32 cy=0.f, float32 rot=0.f) {
		b2PolygonShape shape;
		shape.SetAsBox(pixel2b2cor(w), pixel2b2cor(h), b2Vec2(pixel2b2cor(cx), pixel2b2cor(cy)), 0.f);
		return shape;
	}

	inline b2CircleShape generateCircle(float32 r) {
		b2CircleShape shape;
		shape.m_radius = pixel2b2cor(r);
		return shape;
	}

	inline b2PolygonShape generatePolygen(b2Vec2* vertices, size_t count) {
		b2PolygonShape shape;
		shape.Set(vertices, count);
		return shape;
	}

	inline b2PolygonShape generateEdge(float32 v1, float32 v2, float32 v3, float32 v4) {
		b2PolygonShape shape;
		b2Vec2 bv1(v1, v2);
		b2Vec2 bv2(v3, v4);
		shape.SetAsEdge(pixel2b2cor(bv1), pixel2b2cor(bv2));
		return shape;
	}

	class SoraPhysicalObject: public SoraObject {
	public:
		// luaWrappers
		SoraPhysicalObject(float32 posx, float32 posy, int bDynamicBody);
		void setAsBox(float32 w, float32 h, float32 density);
		void setAsCircle(float32 r, float32 density);
	//	void addlua(LuaPlus::LuaObject p);

		SoraPhysicalObject(float32 posx, float32 posy, const b2Shape& shape, float32 density, bool dynamicBody=true);
		SoraPhysicalObject(const b2BodyDef& bodyDef, const b2FixtureDef& fixtureDef);
		virtual ~SoraPhysicalObject();
		
		void createFixture(const b2FixtureDef& def);
		void createFixture(const b2Shape& shape, float32 density=0.f);
		void destroyFixture(b2Fixture* def);

		void setSpeed(float32 x, float32 y);
		b2Vec2 getSpeed() const;
		float32 getSpeedX();
		float32 getSpeedY();

		void setAngularSpeed(float32 omega);
		float32 getAngularSpeed();
		
		void setMass(float32 mass);
		void setMassF(float32 mass, float32 massx, float32 massy);
		float32 getMass();

		void applyForce(float32 fx, float32 fy);
		void applyForceF(float32 fx, float32 fy, float32 px, float32 py);

		float32 getAngle();
		b2Vec2 getPosition() const;
		float32 getPositionX();
		float32 getPositionY();

		void setBullet(bool flag);
		bool isBullet();
		
		void setSleep(bool flag);
		bool isSleep();

		void setActive(bool flag);
		bool isActive();

		b2Body* getBody();

		void add(SoraObject* obj);

		void setLocalAnchor(float32 x, float32 y);
		b2Vec2 getLocalAnchor() const;
		float32 getLocalAnchorX();
		float32 getLocalAnchorY();

	private:
		b2Body* body;

		b2Vec2 localAnchor;
	};

} // namespace sora

#endif