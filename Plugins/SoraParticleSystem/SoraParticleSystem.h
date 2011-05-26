#ifndef SORA_EFFECT_SYSTEM
#define SORA_EFFECT_SYSTEM

#include "SoraCore.h"
#include "SoraSprite.h"

#include "SoraException.h"
#include "SoraPlatform.h"

#include "SoraColor.h"

#include "hgerect.h"
#include "hgevector.h"
#include "vector3.h"

/* custom container for faster traversal*/
#include "QuickList.h"

/* quaternion for rotation 3v */
#include "quaternion.h"

#include <vector>
#include <map>

#define MAX_PARTICLE_NUM 20000
#define MAX_DISTANCE 1000.f

namespace sora {

	struct SoraParticleHeader {
		SoraParticleHeader(): nEmitNum(0), fEmitLifetime(0), fEmitDuration(0), blendMode(BLEND_DEFAULT) {}
		
		int32 nEmitNum;
		float32 fEmitLifetime;

		float32 fMinLifetime;
		float32 fMaxLifetime;

		float32 fMinSpeed;
		float32 fMaxSpeed;

		float32 fMinGravity;
		float32 fMaxGravity;

		float32 fMinStartScale;
		float32 fMaxStartScale;

		float32 fMinEndScale;
		float32 fMaxEndScale;

		float32 fMinLinearAcc;
		float32 fMaxLinearAcc;

		float32 fMinTrigAcc;
		float32 fMaxTrigAcc;

		float32 fMinAngle;
		float32 fMaxAngle;
		
		float32 fMinSpin;
		float32 fMaxSpin;
		
		vector3 emitPos;
		vector3 emitRange;
		vector3 minDirection;
		vector3 maxDirection;

		SoraColorRGBA dwMinStartColor;
		SoraColorRGBA dwMaxStartColor;

		SoraColorRGBA dwMinEndColor;
		SoraColorRGBA dwMaxEndColor;
		// texture rect
		float32 texX;
		float32 texY;
		float32 texW;
		float32 texH;
		
		float32 fEmitDuration;

		int32 blendMode;
	};

	struct SoraParticleNode {
		float32 fCurrLifetime;
		float32 fLifetime;
		
		float32 fLinearAcc;
		
		float32 fSpeedX;
		float32 fSpeedY;
	
		float32 fAngle;
		float32 fTrigAcc;

		float32 fGravity;
		
		float32 fCurrScale;
		float32 fScaleVar;

		float32 fCurrSpin;
		float32 fSpin;
		
		vector3 position;
		vector3 direction;
		float32 fSpeed;

		SoraColorRGBA dwCurrColor;
		SoraColorRGBA dwColorVar;

		bool bDead;
	};
	
	/*
	 return true = erase particle after return
	 */
	class SoraParticleEffector {
	public:
		virtual bool effect(SoraParticleNode& node, float32 dt) = 0;
	};

	typedef ulong32 HSORAPARTICLE;

	class SoraParticleSystem {
	public:
		SoraParticleSystem();
		SoraParticleSystem(const SoraParticleHeader& header, SoraSprite* pSpr, float32 x=0.f, float32 y=0.f, float32 z=0.f);
		SoraParticleSystem(const SoraWString& script, SoraSprite* pSpr, float32 x=0.f, float32 y=0.f, float32 z=0.f);
		 
		// enable z depth buffer when rendering particle
		void setZDepthEnabled(bool flag);
		void setZDepth(float32 z);
		bool isZDepthEnabled() const;
		float32 getZDepth() const;

		void emit(const SoraParticleHeader& header, SoraSprite* pSpr, float32 x=0.f, float32 y=0.f, float32 z=0.f);
		void emit(const SoraWString& script, SoraSprite* pSpr, float32 x=0.f, float32 y=0.f, float32 z=0.f);

		void render();
		void update(float32 dt);

		void restart();
		void stop();
		void fire();
		void fireAt(float32 x, float32 y, float32 z);
		void moveTo(float32 x, float32 y, float32 z, bool bKill=false);

		void killAll();

		bool isActive() const { return bActive; }

		void saveScript(const SoraWString& script);
		int32 getLiveParticle() const { return particles.size(); }
		
		void setBlendMode(int32 mode);
		int32 getBlendMode() const;

		void rotate(float32 roll, float32 pitch, float32 yaw);
				
		hgeRect getBoundingBox() const { return hrBoundingBox; }
		
		/* effector */
		/*
		 Effectors would NOT be deleted after particlesystem destruct
		 */
		void addEffector(SoraParticleEffector* effector);
		void removeEffector(SoraParticleEffector* effector);
		
		SoraParticleHeader pheader;

	private:
		inline void emitParticle();
		inline void init();
		
		SoraParticleSystem(const SoraParticleSystem&);
		SoraParticleSystem operator=(const SoraParticleSystem&);

		void _parseScript(const SoraWString& script);
		
		typedef std::vector<SoraParticleNode> PARTICLES;
		PARTICLES particles;
		
		typedef std::list<SoraParticleEffector*> EFFECTORS;
		EFFECTORS effectors;

		SoraSprite* pSprite;

		float32 fCurrEmitTime;
		float32 fToEmitTime;
		float32 fMaxDistance;

		bool bActive;
		bool bZDepth;
		float32 z;
		
		bool bRotate3v;
		quaternion quatRotation;
		
		hgeRect hrBoundingBox;
		
		SoraCore* core;
	};

	class SoraParticleManager {
		friend class SoraParticleSystem;
		typedef map<HSORAPARTICLE, SoraParticleSystem*> PARTICLE_MAP;

	public:
		SoraParticleManager();
		SoraParticleManager(HSORASPRITE globalSprite);
		virtual ~SoraParticleManager();

		void setGlobalSprite(HSORASPRITE globalSprite);

		HSORAPARTICLE emit(const SoraParticleHeader& header, float32 x=0.f, float32 y=0.f, float32 z=0.f);
		HSORAPARTICLE emitS(const SoraWString& par, float32 x=0.f, float32 y=0.f, float32 z=0.f);

		bool isActive(HSORAPARTICLE h);
		
		void kill(HSORAPARTICLE h);
		void stop(HSORAPARTICLE h);
		void restart(HSORAPARTICLE h);

		void fire(HSORAPARTICLE h);
		void fireAt(HSORAPARTICLE h, float32 x, float32 y, float32 z);
		
		void killAll();

		int32 getParticleAlive(HSORAPARTICLE h);
		int32 getTotalParticleAlive();
		
		int32 size();

		void render();
		void update(float32 dt);

		void save(HSORAPARTICLE h, const SoraWString& path);
		
	private:
		SoraParticleManager(const SoraParticleManager&) {}

		PARTICLE_MAP particles;

		SoraSprite* pGlobalSprite;
	};

} // namespace sora

#endif