#ifndef SORA_PHYSICAL_WORLD_H
#define SORA_PHYSICAL_WORLD_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraSingleton.h"

#include "Box2D/Box2D.h"

/**
 * This option lets you run the physical update in a single thread
 *  So that if the physical world runs very slow
 *  It still woundn't influence the main thread of the game
 **/
//#define SORA_PHYSICAL_THREAD

namespace sora {

#define SORA_PHYSICAL_DEFAULT_GRAVITY_X 0.f
#define SORA_PHYSICAL_DEFAULT_GRAVITY_Y 1.f	

	class SoraPhysicalWorld: public SoraSingleton<SoraPhysicalWorld> {
    protected:
		friend class SoraSingleton<SoraPhysicalWorld>;
        
		SoraPhysicalWorld();
		~SoraPhysicalWorld();

	public:
		void initBox2DWorld(float32 gravityx, float32 gravityy, bool doSleep=true);

		b2Vec2 getGravity() { return worldGravity; }

		b2World* getCurrentWorld() { return physicalWorld; }
        b2Body* getBodyList();

		// frametime would get automatically form coretimer
		// iterations default is 10, 8
		void stepWorld();

		void stepWorldEx(float32 dt, int32 vIteration, int32 pIteration);
		void setFrameTime(float32 ft) { dt = ft; }

		// create a dynamic body
		b2Body* createBody(float32 px, float32 py, const b2Shape& box, bool isDynamic, float32 density=1.f, float32 friction=0.2f);
		b2Body* createBody(float32 px, float32 py, bool bDynamicBody);
		b2Body* createBody(const b2BodyDef& bodyDef, const b2FixtureDef& fixtureDef);

		void createJoint(const b2JointDef& jdef);

		void destroyBody(b2Body* body);
		void destroyJoint(b2Joint* joint);
        
        int32 getBodyCount() const;
        int32 getJointCount() const;
        
        void setScalar(int32 scalar) { coordScalar = scalar; }
        int32 getScalar() { return coordScalar; }
        
        inline float32 pixel2b2cor(float32 px) { return px / coordScalar; }
        inline float32 b2cor2pixel(float32 cor) { return cor * coordScalar; }
        inline b2Vec2 pixel2b2cor(b2Vec2& px) { return b2Vec2(px.x / coordScalar, px.y / coordScalar); }
        inline b2Vec2 b2cor2pixel(b2Vec2& cor) { return b2Vec2(cor.x * coordScalar, cor.y * coordScalar); }
        
        // wrappers to help build shapes fast
        b2PolygonShape generateBox(float32 w, float32 h, float32 cx=0.f, float32 cy=0.f, float32 rot=0.f);
        b2CircleShape generateCircle(float32 r);
        b2PolygonShape generatePolygen(b2Vec2* vertices, size_t count);
        b2PolygonShape generateEdge(float32 v1, float32 v2, float32 v3, float32 v4);

	private:
		b2World* physicalWorld;

		int32 velocityIteration;
		int32 positionIteration;

		float32 dt;

		b2Vec2 worldGravity;
        
        int32 coordScalar;

		bool bInitialized;
        bool bPluginInstalled;
    };
    
#define PHYSICAL_WORLD SoraPhysicalWorld::Instance()
    
    static float32 PixelToB2Cor(float32 px) {
        return SoraPhysicalWorld::Instance()->pixel2b2cor(px);
    }
    
    static float32 B2CorToPixel(float32 cor) {
        return SoraPhysicalWorld::Instance()->b2cor2pixel(cor);
    }
    
    static b2Vec2 PixelToB2Cor(b2Vec2& px) {
        return SoraPhysicalWorld::Instance()->pixel2b2cor(px);
    }
    
    static b2Vec2 B2CorToPixel(b2Vec2& cor) {
        return SoraPhysicalWorld::Instance()->b2cor2pixel(cor);
    }
    
    static b2PolygonShape B2CreateBox(float32 w, float32 h, float32 cx=0.f, float32 cy=0.f, float32 rot=0.f) {
        return SoraPhysicalWorld::Instance()->generateBox(w, h, cx, cy, rot);
    }
    
    static b2CircleShape B2CreateCircle(float32 r) {
        return SoraPhysicalWorld::Instance()->generateCircle(r);
    }
    
    static b2PolygonShape B2CreatePolygen(b2Vec2* vertices, size_t count) {
        return SoraPhysicalWorld::Instance()->generatePolygen(vertices, count);
    }
    
    static b2PolygonShape B2CreateEdge(float32 v1, float32 v2, float32 v3, float32 v4) {
        return SoraPhysicalWorld::Instance()->generateEdge(v1, v3, v3, v4);
    }
    

} // namespace sora

#endif