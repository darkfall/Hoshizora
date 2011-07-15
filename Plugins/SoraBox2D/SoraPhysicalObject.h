#ifndef SORA_PHYSICAL_OBJECT_H_
#define SORA_PHYSICAL_OBJECT_H_

#include "SoraObject.h"
#include "SoraDelegate.h"

#include "Box2D/Box2D.h"

namespace sora {
    
#define OBJ_PHYSICAL 0x0000300
    
    
    class SoraPhysicalObject;
    
    enum ContactPhase {
        CONTACT_BEGIN,
        CONTACT_END,
    };
    
    struct SoraPhysicalContactInfo {
        SoraPhysicalObject* contactObject1;
        SoraPhysicalObject* contactObject2;
        b2Vec2 normal;
        b2Vec2 position;
        bool isTouching;
        
        ContactPhase phase;
    };

	class SoraPhysicalObject: public SoraObject {
	public:
		// luaWrappers
		SoraPhysicalObject(float32 posx, float32 posy, int bDynamicBody);
		void setAsBox(float32 w, float32 h, float32 density);
		void setAsCircle(float32 r, float32 density);

		SoraPhysicalObject(float32 posx, float32 posy, const b2Shape& shape, float32 density, bool dynamicBody=true);
		SoraPhysicalObject(const b2BodyDef& bodyDef, const b2FixtureDef& fixtureDef);
		virtual ~SoraPhysicalObject();
		
		void createFixture(const b2FixtureDef& def);
		void createFixture(const b2Shape& shape, float32 density=0.f);
		void destroyFixture(b2Fixture* def);

		void setSpeed(float32 x, float32 y);
		b2Vec2 getSpeed() const;
		float32 getSpeedX() const;
		float32 getSpeedY() const;

		void setAngularSpeed(float32 omega);
		float32 getAngularSpeed() const;
        
        void createJoint(SoraPhysicalObject* obj);
		
		void setMass(float32 mass);
		void setMassF(float32 mass, float32 massx, float32 massy);
		float32 getMass() const;

		void applyForce(float32 fx, float32 fy);
		void applyForceF(float32 fx, float32 fy, float32 px, float32 py);

		float32 getAngle() const;
		b2Vec2 getPosition() const;
		float32 getPositionX() const;
		float32 getPositionY() const;

		void setBullet(bool flag);
		bool isBullet();
		
		void setSleep(bool flag);
		bool isSleep();

		void setActive(bool flag);
		bool isActive();

		b2Body* getBody() const;

		void add(SoraObject* obj);

		void setLocalAnchor(float32 x, float32 y);
		b2Vec2 getLocalAnchor() const;
		float32 getLocalAnchorX() const;
		float32 getLocalAnchorY() const;
        
        typedef SoraAbstractDelegate<SoraPhysicalContactInfo>* ContactDelegate;
        
        void setContactDelegate(ContactDelegate delegate);
        ContactDelegate getContactDelegate() const;

	private:
		b2Body* body;
		b2Vec2 localAnchor;
        
        ContactDelegate mContactDelegate;
	};

} // namespace sora

#endif