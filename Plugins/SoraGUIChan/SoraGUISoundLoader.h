/*
 *  SoraSoundLoader.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_GCN_SOUND_LOADER_H_
#define SORA_GCN_SOUND_LOADER_H_

#include "guichan.hpp"

#include "SoraSingleton.h"
#include "SoraPlatform.h"
#include "stringId.h"

#include "SoraSoundFile.h"

namespace gcn {
	
	/*
	 SoraSound loader for GUIChan with cache support
	 */
	class SoraGUISoundLoader: public gcn::SoundLoader {
	public:
		SoraGUISoundLoader();
		virtual ~SoraGUISoundLoader();
		
		gcn::Sound* load(const std::string& soundName);
		gcn::Sound* load(const std::wstring& soundName);
		
	private:
		typedef sora::hash_map<sora::stringId, sora::SoraSoundEffectFile*> SOUND_MAP;
		SOUND_MAP mSoundMap;
	};
	
} // namespace sora


#endif // SORA_SOUND_LOADER_H_