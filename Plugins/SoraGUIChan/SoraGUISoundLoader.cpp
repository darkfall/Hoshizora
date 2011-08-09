/*
 *  SoraSoundLoader.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraGUISoundLoader.h"
#include "SoraGUISound.h"
#include "SoraCore.h"

namespace gcn {
	
	SoraGUISoundLoader::SoraGUISoundLoader() {
		Sound::setSoundLoader(this);
	}
	
	SoraGUISoundLoader::~SoraGUISoundLoader() {
		Sound::setSoundLoader(NULL);
		
		for(SOUND_MAP::iterator itSound = mSoundMap.begin(); itSound != mSoundMap.end(); ++itSound) {
			if(itSound->second != NULL) {
				delete (itSound->second);
				itSound->second = NULL;
			}
		}
	}

	Sound* SoraGUISoundLoader::load(const std::string& soundName) {
		sora::stringId soundid = sora::str2id(soundName);
		SoraGUISound* psound = new SoraGUISound;
		
		SOUND_MAP::iterator itSound = mSoundMap.find(soundid);
		if(itSound != mSoundMap.end() && (itSound->second != NULL)) {
			psound->pfile = itSound->second;
			return psound;
		} else {
			sora::SoraSoundEffectFile* psefile = sora::SORA->createSoundEffectFile(sora::s2ws(soundName));
			if(psefile != NULL) {
				mSoundMap[soundid] = psefile;
				psound->pfile = psefile;
				return psound;
			}
		}
		
		return NULL;
	}

} // namespace gcn