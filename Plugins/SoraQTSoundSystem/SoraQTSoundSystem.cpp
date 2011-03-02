/*
 *  SoraOSXSoundSystem.cpp
 *  Sora
 *
 *  Created by griffin clare on 11/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoraQTSoundSystem.h"

#include "audioWrapper.h"

#include "SoraCore.h"
namespace sora {
	
	ulong32 SoraQTSoundSystem::openMusic(const SoraWString& name, bool buffer) {
	/*	ulong32 length;
		void* data = SoraCore::Instance()->getResourceFile(name, length);
		if(data) {
			return openAudioFile(data, length);
		}*/
		return openAudioFile(name);

		return 0;
	}
	
	bool SoraQTSoundSystem::playMusic(ulong32 handle, bool inqueue) {
		return playAudioFile(handle);
	}
	
	bool SoraQTSoundSystem::pauseMusic(ulong32 handle) {
		return false;
	}
	
	void SoraQTSoundSystem::stopMusic(ulong32 handle, bool stopatend) {
	}
	
	ulong32 SoraQTSoundSystem::getMusicLength(ulong32 handle) {
		return 0;
	}
	
	void SoraQTSoundSystem::setMusicVolume(ulong32 handle, float32 vol) {
	}
	
	float32 SoraQTSoundSystem::getMusicVolume(ulong32 handle) {
		return 0.f;
	}
	
	ulong32 SoraQTSoundSystem::openSoundEffect(const SoraWString& name) {
		return 0;
	}
	
	bool SoraQTSoundSystem::playSoundEffect(ulong32 handle, bool loop) {
		return false;
	}
	
	void SoraQTSoundSystem::stopSoundEffect(ulong32 handle) {
	}
	
	void SoraQTSoundSystem::update() {
	}
	
	void SoraQTSoundSystem::setSoundEffectVolume(ulong32 handle, float32 vol) {
	}
	
} // namespace sora