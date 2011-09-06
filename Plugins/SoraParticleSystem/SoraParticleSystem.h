#ifndef SORA_EFFECT_SYSTEM
#define SORA_EFFECT_SYSTEM

#include "SoraCore.h"
#include "SoraSprite.h"
#include "SoraException.h"
#include "SoraPlatform.h"
#include "SoraColor.h"
#include "SoraMath.h"

#include <vector>
#include <map>

namespace sora {

	struct SoraParticleHeader {
		SoraParticleHeader(): nEmitNum(0), fEmitLifetime(0), fEmitDuration(0), blendMode(BLEND_DEFAULT) {}
		
		int32 nEmitNum;
		float fEmitLifetime;

		float fMinLifetime;
		float fMaxLifetime;

		float fMinSpeed;
		float fMaxSpeed;

		float fMinGravity;
		float fMaxGravity;

		float fMinStartScale;
		float fMaxStartScale;

		float fMinEndScale;
		float fMaxEndScale;

		float fMinLinearAcc;
		float fMaxLinearAcc;

		float fMinTrigAcc;
		float fMaxTrigAcc;

		float fMinAngle;
		float fMaxAngle;
		
		float fMinSpin;
		float fMaxSpin;
		
		SoraVector3 emitPos;
		SoraVector3 emitRange;
		SoraVector3 minDirection;
		SoraVector3 maxDirection;

		SoraColorRGBA dwMinStartColor;
		SoraColorRGBA dwMaxStartColor;

		SoraColorRGBA dwMinEndColor;
		SoraColorRGBA dwMaxEndColor;
		// texture rect
		float texX;
		float texY;
		float texW;
		float texH;
		
		float fEmitDuration;

		int32 blendMode;
	};

	struct SoraParticleNode {
		float fCurrLifetime;
		float fLifetime;
		
		float fLinearAcc;
		
		float fSpeedX;
		float fSpeedY;
	
		float fAngle;
		float fTrigAcc;

		float fGravity;
		
		float fCurrScale;
		float fScaleVar;

		float fCurrSpin;
		float fSpin;
		
		SoraVector3 position;
		SoraVector3 direction;
		float fSpeed;

		SoraColorRGBA dwCurrColor;
		SoraColorRGBA dwColorVar;

		bool bDead;
	};
	
	/*
	 return true = erase particle after return
	 */
	class SoraParticleEffector {
	public:
		virtual bool effect(SoraParticleNode& node, float dt) = 0;
	};

	typedef ulong32 HSORAPARTICLE;

	class SoraParticleSystem {
	public:
		SoraParticleSystem();
		SoraParticleSystem(const SoraParticleHeader& header, SoraSprite* pSpr, float x=0.f, float y=0.f, float z=0.f);
		SoraParticleSystem(const util::String& script, SoraSprite* pSpr, float x=0.f, float y=0.f, float z=0.f);
		 
		// enable z depth buffer when rendering particle
		void setZDepthEnabled(bool flag);
		void setZDepth(float z);
		bool isZDepthEnabled() const;
		float getZDepth() const;
		
		void setSprite(SoraSprite* spr);
		SoraSprite* getSprite() const;
		
		void setTextureRect(float x, float y, float w, float h);

		void emit(const SoraParticleHeader& header, SoraSprite* pSpr, float x=0.f, float y=0.f, float z=0.f);
		void emit(const util::String& script, SoraSprite* pSpr, float x=0.f, float y=0.f, float z=0.f);

		void render();
		void update(float dt);

		void restart();
		void stop();
		void fire();
		void fireAt(float x, float y, float z);
		void moveTo(float x, float y, float z, bool bKill=false);
		
		static void setMaxParticleNum(int32 max) { mMaxParticleNum = max; }
		static void setMaxParticleDistance(float max) { mMaxParticleDistance = max; }

		void killAll();

		bool isActive() const { return bActive; }

		void saveScript(const util::String& script);
		int32 getLiveParticle() const { return particles.size(); }
		
		void setBlendMode(int32 mode);
		int32 getBlendMode() const;

		void rotate(float roll, float pitch, float yaw);
				
		SoraRect getBoundingBox() const { return hrBoundingBox; }
		
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
		
		static int32 mMaxParticleNum;
		static float mMaxParticleDistance;
		
		SoraParticleSystem(const SoraParticleSystem&);
		SoraParticleSystem operator=(const SoraParticleSystem&);

		void _parseScript(const util::String& script);
		
		typedef std::vector<SoraParticleNode> PARTICLES;
		PARTICLES particles;
		
		typedef std::list<SoraParticleEffector*> EFFECTORS;
		EFFECTORS effectors;

		SoraSprite* pSprite;

		float fCurrEmitTime;
		float fToEmitTime;
		float fMaxDistance;

		bool bActive;
		bool bZDepth;
		float z;
		
		bool bRotate3v;
		SoraQuaternion quatRotation;
		
		SoraRect hrBoundingBox;
		
		SoraCore* core;
	};

	class SoraParticleManager {
		friend class SoraParticleSystem;
		typedef std::map<HSORAPARTICLE, SoraParticleSystem*> PARTICLE_MAP;

	public:
		SoraParticleManager();
		SoraParticleManager(SoraSprite* globalSprite);
		virtual ~SoraParticleManager();

		void setGlobalSprite(SoraSprite* globalSprite);

		HSORAPARTICLE emit(const SoraParticleHeader& header, float x=0.f, float y=0.f, float z=0.f);
		HSORAPARTICLE emitS(const util::String& par, float x=0.f, float y=0.f, float z=0.f);

		bool isActive(HSORAPARTICLE h);
		
		void kill(HSORAPARTICLE h);
		void stop(HSORAPARTICLE h);
		void restart(HSORAPARTICLE h);

		void fire(HSORAPARTICLE h);
		void fireAt(HSORAPARTICLE h, float x, float y, float z);
		
		void killAll();

		int32 getParticleAlive(HSORAPARTICLE h);
		int32 getTotalParticleAlive();
		
		int32 size();

		void render();
		void update(float dt);

		void save(HSORAPARTICLE h, const util::String& path);
		
	private:
		SoraParticleManager(const SoraParticleManager&) {}

		PARTICLE_MAP particles;

		SoraSprite* pGlobalSprite;
	};

} // namespace sora

#endif