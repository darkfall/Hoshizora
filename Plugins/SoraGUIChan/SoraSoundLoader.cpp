/*
 *  SoraSoundLoader.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraSoundLoader.h"
#include "SoraSound.h"
#include "SoraCore.h"

namespace gcn {
	
	SoraSoundLoader::SoraSoundLoader() {
		Sound::setSoundLoader(this);
	}
	
	SoraSoundLoader::~SoraSoundLoader() {
		Sound::setSoundLoader(NULL);
		
		for(SOUND_MAP::iterator itSound = mSoundMap.begin(); itSound != mSoundMap.end(); ++itSound) {
			if(itSound->second != NULL) {
				delete (itSound->second);
				itSound->second = NULL;
			}
		}
	}

	Sound* SoraSoundLoader::load(const std::string& soundName) {
		return load(sora::s2ws(soundName));
	}
	
	Sound* SoraSoundLoader::load(const std::wstring& soundName) {
		sora::stringId soundid = sora::str2id(soundName);
		SoraSound* psound = new SoraSound;
		
		SOUND_MAP::iterator itSound = mSoundMap.find(soundid);
		if(itSound != mSoundMap.end() && (itSound->second != NULL)) {
			psound->pfile = itSound->second;
			return psound;
		} else {
			sora::SoraSoundEffectFile* psefile = sora::SORA->createSoundEffectFile(soundName);
			if(psefile) {
				mSoundMap[soundid] = psefile;
				psound->pfile = psefile;
				return psound;
			}
		}
		
		return NULL;
	}

} // namespace gcn