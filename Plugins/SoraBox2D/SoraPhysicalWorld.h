#ifndef SORA_PHYSICAL_WORLD_H
#define SORA_PHYSICAL_WORLD_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraSingleton.h"

#include "Box2D/Box2D.h"

namespace sora {

#define SORA_PHYSICAL_DEFAULT_GRAVITY_X 0.f
#define SORA_PHYSICAL_DEFAULT_GRAVITY_Y 1.f

#define SORA_BOX2D_SCALAR 100

	inline float32 pixel2b2cor(float32 px) {
		return px / SORA_BOX2D_SCALAR;
	}

	inline float32 b2cor2pixel(float32 cor) {
		return cor * SORA_BOX2D_SCALAR;
	}

	inline b2Vec2 pixel2b2cor(b2Vec2& px) {
		return b2Vec2(px.x / SORA_BOX2D_SCALAR, px.y / SORA_BOX2D_SCALAR);
	}

	inline b2Vec2 b2cor2pixel(b2Vec2& cor) {
		return b2Vec2(cor.x * SORA_BOX2D_SCALAR, cor.y * SORA_BOX2D_SCALAR);
	}

	class SoraPhysicalWorld: public SoraSingleton<SoraPhysicalWorld> {
		friend class SoraSingleton<SoraPhysicalWorld>;
		
		SoraPhysicalWorld(): velocityIteration(10), positionIteration(8), bInitialized(false) {}
		~SoraPhysicalWorld() {}

	public:
		void InitBox2DWorld(b2Vec2 gravity, bool doSleep=true);

		b2Vec2 GetGravity() { return worldGravity; }

		b2World* GetCurrentWorld() { return physicalWorld; }

		// frametime would get automatically form coretimer
		// iterations default is 10, 8
		void StepWorld();

		void StepWorldEx(float32 dt, int32 vIteration, int32 pIteration);
		void SetFrameTime(float32 ft) { dt = ft; }

		// create a dynamic body
		b2Body* CreateBody(float32 px, float32 py, const b2Shape& box, bool isDynamic, float density=1.f);
		b2Body* CreateBody(float32 px, float32 py, bool bDynamicBody);
		b2Body* CreateBody(const b2BodyDef& bodyDef, const b2FixtureDef& fixtureDef);

		void CreateJoint(const b2JointDef& jdef);

		void destroyBody(b2Body* body);
		void destroyJoint(b2Joint* joint);

	private:
		b2World* physicalWorld;

		int32 velocityIteration;
		int32 positionIteration;

		float32 dt;

		b2Vec2 worldGravity;

		bool bInitialized;
	};

} // namespace sora

#endif