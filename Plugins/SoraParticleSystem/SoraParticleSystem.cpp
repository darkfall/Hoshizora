#include "SoraParticleSystem.h"
#include "SoraStringConv.h"
#include "SoraMath.h"

#include <algorithm>
#include <cmath>

namespace sora {

	int32 SoraParticleSystem::mMaxParticleNum = 2000;
	float SoraParticleSystem::mMaxParticleDistance = 500.f;

	bool isParticleDead(const SoraParticleNode& node) {
		return node.bDead;
	}

	SoraParticleSystem::SoraParticleSystem() {
		fCurrEmitTime = 0.f;
		fToEmitTime = 0.f;
		bActive = false;
		bRotate3v = false;
		fMaxDistance = mMaxParticleDistance;
		core = SoraCore::Instance();

		z = 0.f;		
		bZDepth = false;

	}

	SoraParticleSystem::SoraParticleSystem(const SoraParticleHeader& header, SoraSprite* pSpr, float x, float y, float z) {
		pheader = header;
		pSprite = pSpr;
		core = SoraCore::Instance();
		if(x != 0.f || y != 0.f) {
			pheader.emitPos.x = x;
			pheader.emitPos.y = y;
			pheader.emitPos.z = z;
		}
		
		init();
		bZDepth = false;
		bRotate3v = false;
	}

	SoraParticleSystem::SoraParticleSystem(const SoraWString& str, SoraSprite* pSpr, float x, float y, float z) {
		_parseScript(str);
		pSprite = pSpr;
		core = SoraCore::Instance();
		if(x != 0.f || y != 0.f || z != 0.f) {
			pheader.emitPos.x = x;
			pheader.emitPos.y = y;
			pheader.emitPos.z = z;
		}
		
		init();
		bZDepth = false;
		bRotate3v = false;
	}

	void SoraParticleSystem::setZDepthEnabled(bool flag) {
		bZDepth = flag;
	}

	void SoraParticleSystem::setZDepth(float depth) {
		z = depth;
	}

	bool SoraParticleSystem::isZDepthEnabled() const {
		return bZDepth;
	}

	float SoraParticleSystem::getZDepth() const {
		return z;
	}

	void SoraParticleSystem::emit(const SoraWString& str, SoraSprite* pSpr, float x, float y, float z) {
		_parseScript(str);
		pSprite = pSpr;
	
		if(x != 0.f || y != 0.f || z != 0.f) {
			pheader.emitPos.x = x;
			pheader.emitPos.y = y;
			pheader.emitPos.z = z;
		}
		
		init();
		bRotate3v = false;
		bZDepth = false;

	}

	void SoraParticleSystem::emit(const SoraParticleHeader& header, SoraSprite* pSpr, float x, float y, float z) {
		pheader = header;
		pSprite = pSpr;
		
		if(x != 0.f || y != 0.f || z != 0.f) {
			pheader.emitPos.x = x;
			pheader.emitPos.y = y;
			pheader.emitPos.z = z;
		}
		
		init();
		bRotate3v = false;
		bZDepth = false;

	}
	
	void SoraParticleSystem::setTextureRect(float x, float y, float w, float h) {
		pheader.texX = x;
		pheader.texY = y;
		pheader.texW = w;
		pheader.texH = h;
		
		if(pSprite)
			pSprite->setTextureRect(x, y, w, h);
	}
	
	void SoraParticleSystem::init() {
		if(pSprite) {
			pSprite->setCenter((float)pSprite->getSpriteWidth()/2, (float)pSprite->getSpriteHeight()/2);
			pSprite->setBlendMode(pheader.blendMode);
			pSprite->setTextureRect(pheader.texX, pheader.texY, pheader.texW, pheader.texH);
		}
		
		if(pheader.emitPos.z > mMaxParticleDistance) pheader.emitPos.z = mMaxParticleDistance;
		else if(pheader.emitPos.z < 0.f) pheader.emitPos.z = 0.f;
		
		particles.clear();
		
		hrBoundingBox.set(pheader.emitPos.x-pSprite->getTextureWidth()/2*pheader.fMaxStartScale, 
						  pheader.emitPos.y-pSprite->getTextureHeight()/2*pheader.fMaxStartScale, 
						  pheader.emitPos.x+pSprite->getTextureWidth()/2*pheader.fMaxStartScale, 
						  pheader.emitPos.y+pSprite->getTextureHeight()/2*pheader.fMaxStartScale);
		
		bActive = true;
		
		fCurrEmitTime = 0.f;
		fToEmitTime = 0.f;
		
		fMaxDistance = mMaxParticleDistance;
	}
	
	void SoraParticleSystem::setSprite(SoraSprite* spr) {
		pSprite = spr;
	}
	
	SoraSprite* SoraParticleSystem::getSprite() const {
		return pSprite;
	}

	void SoraParticleSystem::killAll() {
		bActive = false;
		particles.clear();
	}

	void SoraParticleSystem::stop() {
		bActive = false;
	}

	void SoraParticleSystem::fire() {
		bActive = true;
		init();
	}

	void SoraParticleSystem::fireAt(float x, float y, float z) {
		bActive = true;
		init();
		pheader.emitPos.set(x, y, z);
		hrBoundingBox.x1 = x;
		hrBoundingBox.y1 = y;
	}

	void SoraParticleSystem::moveTo(float x, float y, float z, bool bKill) {
		if(bKill) killAll();
		pheader.emitPos.set(x, y, z);
		hrBoundingBox.x1 = x;
		hrBoundingBox.y1 = y;
	}

	void SoraParticleSystem::restart() {
		killAll();
		if(pSprite) {	
			bActive = true;
			
			init();
		}
	}
	
	bool isParticleDied(const SoraParticleNode& node) {
		return node.bDead;
	}
	
	void SoraParticleSystem::update(float dt) {
		if(dt > 0.1f) return;
		
		particles.erase(std::remove_if(particles.begin(), particles.end(), isParticleDied), particles.end());

		if(particles.size() != 0) {
			PARTICLES::iterator p = particles.begin();
			while( p != particles.end() ) {
				if(!p->bDead) {
					p->fCurrLifetime += dt;;
					if(p->fCurrLifetime >= p->fLifetime) {
						p->bDead = true;
						p = particles.erase(p);
						continue;
					}
					if(p->position.z < 0.f || p->position.z > fMaxDistance) {
						p->bDead = true;
						p = particles.erase(p);
						continue;
					}
				
					if( p->fGravity != 0.f ) {
						p->direction.y += p->fGravity*dt;
                        p->direction.normalize();
						if(p->fSpeed == 0.f)
							p->fSpeed += p->fGravity*dt;
					}
						
					if(p->fLinearAcc != 0.f) {
						p->fSpeed += p->fLinearAcc*dt;
					}
					if(p->fTrigAcc != 0.f) {
						rotate(p->fTrigAcc*dt, 0.f, 0.f);
					}
				
					if(p->direction.length() != 0)
						p->position += p->direction*p->fSpeed*dt*((1.f - p->position.z / fMaxDistance) + 0.01f);
					else {
						p->position.x += cosf(p->fAngle) * p->fSpeed;
						p->position.y += sinf(p->fAngle) * p->fSpeed;
					}
					
					if(p->position.x < hrBoundingBox.x1)
						hrBoundingBox.x1 = p->position.x-pSprite->getTextureWidth()/2*p->fCurrScale;
					else if(p->position.x > hrBoundingBox.x2)
						hrBoundingBox.x2 = p->position.x+pSprite->getTextureWidth()/2*p->fCurrScale;
					if(p->position.y < hrBoundingBox.y1)
						hrBoundingBox.y1 = p->position.y-pSprite->getTextureHeight()/2*p->fCurrScale;
					else if(p->position.y > hrBoundingBox.y2)
						hrBoundingBox.y2 = p->position.y+pSprite->getTextureHeight()/2*p->fCurrScale;
					
					p->fAngle += p->fSpin * dt;
					p->fCurrScale += p->fScaleVar * dt;
					p->dwCurrColor += p->dwColorVar * dt;
					
					bool bdel = false;
					if(!effectors.empty()) {
						EFFECTORS::iterator iteff = effectors.begin();
						while(iteff != effectors.end() && !bdel) {
							if((*iteff)->effect((*p), dt)) {
								p->bDead = true;
						//		p = particles.erase(p);
								bdel = true;
								continue;
							}
							++iteff;
						}
					}
					if(bdel)
						continue;
				}
				++p;
			}
		}
		
		fCurrEmitTime += dt;
		if(fCurrEmitTime > pheader.fEmitLifetime && pheader.fEmitLifetime != 0.f) { 
			if(getLiveParticle() == 0) {
				bActive = false;
				fCurrEmitTime = 0.f;
			}
			return;
		}
		
		
		if(!bActive) return;
		if(particles.size() > mMaxParticleNum) {
			fToEmitTime = 0.f;
			return;
		}
		fToEmitTime += dt;
		if(pheader.fEmitDuration == 0.f) {
			if(fToEmitTime > 1.f / pheader.nEmitNum) {
				for(int i=0; i<(int)(fToEmitTime * pheader.nEmitNum + 0.5f); ++i) {
					emitParticle();
				}
				fToEmitTime = 0.f;
			}
		} else if(fToEmitTime > pheader.fEmitDuration) {
			for(int i=0; i<pheader.nEmitNum; ++i) {
				emitParticle();
			}
			fToEmitTime = 0.f;
		}
	}

	void SoraParticleSystem::render() {
		if(!pSprite)
			return;
		if(particles.size() == 0)
			return;
		
		pSprite->setTextureRect(pheader.texX, pheader.texY, pheader.texW, pheader.texH);
		pSprite->setCenter(pheader.texW/2, pheader.texH/2);

		if(bZDepth) {
			pSprite->setBlendMode(BLEND_DEFAULT);
			pSprite->setZ( z );
		} else {
			pSprite->setBlendMode(BLEND_DEFAULT_Z);
		}
		
		for(PARTICLES::iterator p=particles.begin(); p!=particles.end(); ++p) {
			if(!p->bDead) {
				float scale = (1.f - p->position.z / fMaxDistance) + 0.01f;
				if(scale > 1.f) scale = 1.f;
				else if(scale < 0.f) scale = 0.f;
				pSprite->setColor( CSETA(p->dwCurrColor, p->dwCurrColor.a * 255 * scale ) );
				pSprite->setRotation( p->fAngle );
				pSprite->setScale( p->fCurrScale * scale, p->fCurrScale * scale );
				pSprite->render( p->position.x, p->position.y );
			}
		}
	//	pSprite->setZ(1.f);
	}
	
	void SoraParticleSystem::emitParticle() {
		SoraParticleNode node;
		node.bDead = false;
		node.fLifetime = SoraCore::RandomFloat(pheader.fMinLifetime, pheader.fMaxLifetime);
		
		node.dwCurrColor.a = SoraCore::RandomFloat(pheader.dwMinStartColor.a, pheader.dwMaxStartColor.a);
		node.dwCurrColor.r = SoraCore::RandomFloat(pheader.dwMinStartColor.r, pheader.dwMaxStartColor.r);
		node.dwCurrColor.g = SoraCore::RandomFloat(pheader.dwMinStartColor.g, pheader.dwMaxStartColor.g);
		node.dwCurrColor.b = SoraCore::RandomFloat(pheader.dwMinStartColor.b, pheader.dwMaxStartColor.b);
		SoraColorRGBA dwStart = node.dwCurrColor;
		SoraColorRGBA dwEnd;
		dwEnd.a = SoraCore::RandomFloat(pheader.dwMinEndColor.a, pheader.dwMaxEndColor.a);
		dwEnd.r = SoraCore::RandomFloat(pheader.dwMinEndColor.r, pheader.dwMaxEndColor.r);
		dwEnd.g = SoraCore::RandomFloat(pheader.dwMinEndColor.g, pheader.dwMaxEndColor.g);
		dwEnd.b = SoraCore::RandomFloat(pheader.dwMinEndColor.b, pheader.dwMaxEndColor.b);
		node.dwColorVar = (dwEnd - dwStart) / node.fLifetime;
		
		node.fCurrScale = SoraCore::RandomFloat(pheader.fMinStartScale, pheader.fMaxStartScale);
		float fStart = node.fCurrScale;
		float fEnd = SoraCore::RandomFloat(pheader.fMinEndScale, pheader.fMaxEndScale);
		node.fScaleVar = (fEnd - fStart) / node.fLifetime;
		
		node.fCurrSpin = 0.f;
		node.fSpin = SoraCore::RandomFloat(pheader.fMinSpin, pheader.fMaxSpin);
		
		node.fSpeed = SoraCore::RandomFloat(pheader.fMinSpeed, pheader.fMaxSpeed);
		node.fAngle = DGR_RAD(SoraCore::RandomFloat(pheader.fMinAngle, pheader.fMaxAngle));
		node.fGravity = SoraCore::RandomFloat(pheader.fMinGravity, pheader.fMaxGravity);
		node.fTrigAcc = DGR_RAD(SoraCore::RandomFloat(pheader.fMinTrigAcc, pheader.fMaxTrigAcc));
		node.fLinearAcc = SoraCore::RandomFloat(pheader.fMinLinearAcc, pheader.fMaxLinearAcc);
		
		node.position.set(pheader.emitPos.x+SoraCore::RandomFloat(-pheader.emitRange.x, pheader.emitRange.x),
						  pheader.emitPos.y+SoraCore::RandomFloat(-pheader.emitRange.y, pheader.emitRange.y),
						  pheader.emitPos.z+SoraCore::RandomFloat(-pheader.emitRange.z, pheader.emitRange.z));
		node.direction = SoraVector3(SoraCore::RandomFloat(pheader.minDirection.x, pheader.maxDirection.x),
								 SoraCore::RandomFloat(pheader.minDirection.y, pheader.maxDirection.y),
								 SoraCore::RandomFloat(pheader.minDirection.z, pheader.maxDirection.z)).normalize();
		
		if(bRotate3v)
			node.direction = quatRotation.rotate(node.direction);
		
		node.fCurrLifetime = 0.f;
		particles.push_back(node);
	}

	void SoraParticleSystem::setBlendMode(int32 mode) {
		pheader.blendMode = mode;
		if(pSprite) pSprite->setBlendMode(mode);
	}

	int32 SoraParticleSystem::getBlendMode() const {
		return pheader.blendMode;
	}
	
	void SoraParticleSystem::rotate(float roll, float pitch, float yaw) {
		quatRotation.makeRotate(roll, pitch, yaw);
		bRotate3v = true;
		
	/*	PARTICLES::iterator p = particles.begin();
		while( p != particles.end() ) {
			++p;
			
			p->direction = quatRotation.rotate(p->direction);
		}*/
	}

	void SoraParticleSystem::saveScript(const SoraWString& script) {
		FILE* file = fopen(ws2s(script).c_str(), "wb");
		if(file) {
		//	fwrite("SRPS", 4, 1, file);
			fwrite(&pheader, sizeof(SoraParticleHeader), 1, file);
			fclose(file);
		}
	}

	void SoraParticleSystem::_parseScript(const SoraWString& script) {
		uint32 size;
		void* buffer = SoraCore::Instance()->getResourceFile(script, size);
		if(buffer) {
			memcpy(&pheader, buffer, size);
            SoraCore::Ptr->freeResourceFile(buffer);
		}
	}
	
	void SoraParticleSystem::addEffector(SoraParticleEffector* effector) {
		effectors.push_back(effector);
	}
	
	void SoraParticleSystem::removeEffector(SoraParticleEffector* effector) {
		effectors.remove(effector);
	}

	SoraParticleManager::SoraParticleManager() {
		pGlobalSprite = 0;
	}

	SoraParticleManager::SoraParticleManager(SoraSprite* globalSprite) {
		pGlobalSprite = (SoraSprite*)globalSprite;
	}

	SoraParticleManager::~SoraParticleManager() {
		killAll();
	}

	void SoraParticleManager::setGlobalSprite(SoraSprite* globalSprite) {
		pGlobalSprite = globalSprite;
	}

	void SoraParticleManager::killAll() {
		PARTICLE_MAP::iterator p = particles.begin();
		while( p != particles.end() ) {
			if( p->second ) {
				delete p->second;
				p->second = 0;
			}
			++p;
		}
		particles.clear();
	}

	void SoraParticleManager::kill(SoraHandle h) {
		PARTICLE_MAP::iterator p = particles.find(h);
		if( p != particles.end() ) {
			delete p->second;
			particles.erase( p );
		}
	}

	bool SoraParticleManager::isActive(SoraHandle h) {
		PARTICLE_MAP::iterator p = particles.find(h);
		if( p != particles.end() ) {
			return p->second->isActive();
		}
		return false;
	}

	void SoraParticleManager::stop(SoraHandle h) {
		PARTICLE_MAP::iterator p = particles.find(h);
		if( p != particles.end() ) {
			p->second->stop();
		}
	}

	void SoraParticleManager::restart(SoraHandle h) {
		PARTICLE_MAP::iterator p = particles.find(h);
		if( p != particles.end() ) {
			p->second->restart();
		}
	}

	void SoraParticleManager::fire(SoraHandle h) {
		PARTICLE_MAP::iterator p = particles.find(h);
		if( p != particles.end() ) {
			p->second->fire();
		}
	}

	void SoraParticleManager::fireAt(SoraHandle h, float x, float y, float z) {
		PARTICLE_MAP::iterator p = particles.find(h);
		if( p != particles.end() ) {
			p->second->fireAt(x, y, z);
		}
	}

	int32 SoraParticleManager::getParticleAlive(SoraHandle h) {
		PARTICLE_MAP::iterator p = particles.find(h);
		if( p != particles.end() ) {
			return p->second->getLiveParticle();
		}
		return 0;
	}

	int32 SoraParticleManager::getTotalParticleAlive() {
		PARTICLE_MAP::iterator p = particles.begin();
		int32 num = 0;
		while( p != particles.end() ) {
			num += p->second->getLiveParticle();
			++p;
		}
		return num;
	}

	int32 SoraParticleManager::size() {
		return particles.size();
	}

	void SoraParticleManager::save(SoraHandle h, const SoraWString& path) {
		PARTICLE_MAP::iterator p = particles.find(h);
		if( p != particles.end() ) {
			p->second->saveScript(path);
		}
	}

	void SoraParticleManager::render() {
		PARTICLE_MAP::iterator p = particles.begin();
		while( p != particles.end() ) {
			p->second->render();
			++p;
		}
	}

	void SoraParticleManager::update(float dt) {
		PARTICLE_MAP::iterator p = particles.begin();
		while( p != particles.end() ) {
			p->second->update(dt);
			if(!p->second->isActive()) {
				delete p->second;
				particles.erase(p);
			}
			++p;
		}
	}

	SoraHandle SoraParticleManager::emit(const SoraParticleHeader& header, float x, float y, float z) {
		if(pGlobalSprite == 0)
			THROW_SORA_EXCEPTION(IllegalStateException, "Error emit Particle, no global particle sprites set");
		
		SoraParticleSystem* par = new SoraParticleSystem(header, pGlobalSprite, x, y, z);
		if(!par)
			THROW_SORA_EXCEPTION(RuntimeException, "Failed Alloc ParticleSystem");
		par->fire();
		particles[(SoraHandle)par] = par;
		return (SoraHandle)par;
	}

	SoraHandle SoraParticleManager::emitS(const SoraWString& s, float x, float y, float z) {
		if(pGlobalSprite == 0)
			THROW_SORA_EXCEPTION(IllegalStateException,"Error emit Particle, no global particle sprites set");
		
		SoraParticleSystem* par = new SoraParticleSystem(s, pGlobalSprite, x, y, z);
		if(!par)
			THROW_SORA_EXCEPTION(RuntimeException, "Failed Alloc ParticleSystem");
		par->fire();
		particles[(SoraHandle)par] = par;
		return (SoraHandle)par;
	}
} // namespace sora