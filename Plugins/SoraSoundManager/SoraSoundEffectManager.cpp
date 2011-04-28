/*
 *  SoraSoundEffectManager.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraSoundEffectManager.h"
#include "SoraCore.h"

namespace sora {
	
	SoraSoundEffectManager::SoraSoundEffectManager(): effectVolume(70) {
	}
	
	SoraSoundEffectManager::~SoraSoundEffectManager() {
		SOUND_EFFECT_MAP::iterator itEffect = mSoundEffects.begin();
		for(itEffect = mSoundEffects.begin(); itEffect != mSoundEffects.end(); ++itEffect) {
			if(itEffect->second != NULL) {
				delete itEffect->second;
				itEffect->second = NULL;
			}
		}
		mSoundEffects.clear();
	}

	SoundEffectId SoraSoundEffectManager::load(const std::wstring& effectName) {
		SoundEffectId eid = str2id(effectName);
		SOUND_EFFECT_MAP::iterator itEffect = mSoundEffects.find(eid);
		if(itEffect != mSoundEffects.end() && (itEffect->second != NULL)) {
			return eid;
		} 
		
		SoraSoundEffectFile* pefile = SORA->createSoundEffectFile(effectName);
		if(pefile == NULL)
			return 0;
		
		mSoundEffects[eid] = pefile;
		return eid;
	}
	
	void SoraSoundEffectManager::unload(SoundEffectId eid) {
		SOUND_EFFECT_MAP::iterator itEffect = mSoundEffects.find(eid);
		if(itEffect != mSoundEffects.end() && (itEffect->second != NULL)) {
			delete itEffect->second;
			itEffect->second = NULL;
			
			mSoundEffects.erase(itEffect);
		}
	}
	
	SoraSoundEffectFile* SoraSoundEffectManager::get(SoundEffectId eid) const {
		SOUND_EFFECT_MAP::const_iterator itEffect = mSoundEffects.find(eid);
		if(itEffect != mSoundEffects.end() && (itEffect->second != NULL)) {
			return itEffect->second;
		}
		return NULL;
	}
	
	bool SoraSoundEffectManager::play(SoundEffectId eid) {
		SOUND_EFFECT_MAP::iterator itEffect = mSoundEffects.find(eid);
		if(itEffect != mSoundEffects.end() && (itEffect->second != NULL)) {
			itEffect->second->setVolume(effectVolume);
			itEffect->second->play();
			return true;
		}
		return false;
	}
	
	void SoraSoundEffectManager::stop(SoundEffectId eid) {
		SOUND_EFFECT_MAP::iterator itEffect = mSoundEffects.find(eid);
		if(itEffect != mSoundEffects.end() && (itEffect->second != NULL)) {
			itEffect->second->stop();
		}
	}
	
	void SoraSoundEffectManager::setVolume(float32 vol) {
		effectVolume = vol;
	}
	
	float32 SoraSoundEffectManager::getVolume() const {
		return effectVolume;
	}
	
	bool SoraSoundEffectManager::playDirect(const std::wstring& effectName) {
		SoundEffectId eid = str2id(effectName);
		if(!play(eid)) {
			SoraSoundEffectFile* pefile = SORA->createSoundEffectFile(effectName);
			if(pefile == NULL)
				return false;
			
			mSoundEffects[eid] = pefile;
			pefile->setVolume(effectVolume);
			pefile->play();
			return true;
		}
		return false;
	}

} // namespace sora