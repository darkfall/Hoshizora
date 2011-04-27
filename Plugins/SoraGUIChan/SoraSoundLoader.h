/*
 *  SoraSoundLoader.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_SOUND_LOADER_H_
#define SORA_SOUND_LOADER_H_

#include "SoundLoader.h"
#include "Sound.h"

#include "SoraSingleton.h"
#include "SoraPlatform.h"
#include "stringId.h"

#include "SoraSoundFile.h"

namespace gcn {
	
	/*
	 SoraSound loader for GUIChan with cache support
	 */
	class SoraSoundLoader: public gcn::SoundLoader, public sora::SoraSingleton<SoraSoundLoader> {
		friend class sora::SoraSingleton<SoraSoundLoader>;
	
	public:
		SoraSoundLoader();
		virtual ~SoraSoundLoader();
		
		Sound* load(const std::string& soundName);
		Sound* load(const std::wstring& soundName);
		
	private:
		typedef sora::hash_map<sora::stringId, sora::SoraSoundEffectFile*> SOUND_MAP;
		SOUND_MAP mSoundMap;
	};
	
} // namespace sora


#endif // SORA_SOUND_LOADER_H_