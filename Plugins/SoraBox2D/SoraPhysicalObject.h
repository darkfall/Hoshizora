#ifndef SORA_PHYSICAL_OBJECT_H_
#define SORA_PHYSICAL_OBJECT_H_

#include "SoraObject.h"
#include "SoraDelegate.h"
#include "SoraSprite.h"

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
        
        virtual uint32 update(float32 dt);
        virtual void render();
		
		void createFixture(const b2FixtureDef& def);
		void createFixture(const b2Shape& shape, float32 density=0.f);
		void destroyFixture(b2Fixture* def);

		void    setSpeed(float32 x, float32 y);
		b2Vec2  getSpeed() const;
		float32 getSpeedX() const;
		float32 getSpeedY() const;

		void    setAngularSpeed(float32 omega);
		float32 getAngularSpeed() const;
        
        void createJoint(SoraPhysicalObject* obj);
		
		void    setMass(float32 mass);
		void    setMassF(float32 mass, float32 massx, float32 massy);
		float32 getMass() const;

		void applyForce(float32 fx, float32 fy);
		void applyForceF(float32 fx, float32 fy, float32 px, float32 py);

		float32 getAngle() const;
		void    getPosition(float32&x, float32 &y);
        float32 getPositionX();
        float32 getPositionY();

		void setBullet(bool flag);
		bool isBullet();
		
		void setSleep(bool flag);
		bool isSleep();

		void setActive(bool flag);
		bool isActive();

		b2Body* getBody() const;
        b2Fixture* getFixture() const;
        b2AABB getBoundingBox() const;

		void add(SoraObject* obj);

		void    setLocalAnchor(float32 x, float32 y);
		b2Vec2  getLocalAnchor() const;
		float32 getLocalAnchorX() const;
		float32 getLocalAnchorY() const;
        
        typedef SoraAbstractDelegate<SoraPhysicalContactInfo>* ContactDelegate;
        
        void setContactDelegate(ContactDelegate delegate);
        ContactDelegate getContactDelegate() const;
        
        SoraSprite* getRenderer() const;
        
        void enableDrawBoundingBox(bool flag);

	private:
		b2Body* body;
		b2Vec2 localAnchor;
        
        SoraSprite* mRenderer;
        ContactDelegate mContactDelegate;
        
        bool mDrawBoundingBox;
	};
    
    static SoraPhysicalObject* RenderablePhysicalOject(float32 x, float32 y, const b2Shape& shape, SoraSprite* spr, bool dynamic=true) {
        SoraPhysicalObject* obj = new SoraPhysicalObject(x, y, shape, 1.f, dynamic);
        obj->add(spr);
        spr->setCenter(spr->getSpriteWidth()/2, spr->getSpriteHeight()/2);
        return obj;
    }
} // namespace sora

#endif