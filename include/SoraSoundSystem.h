/*
 *  SoraSoundSystem.h
 *  Sora
 *
 *  Created by griffin clare on 11/25/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */


#ifndef SORA_SOUND_SYSTEM_H_
#define SORA_SOUND_SYSTEM_H_

#include "SoraPlatform.h"
#include "SoraSoundFile.h"
#include "SoraMoviePlayer.h"

namespace sora {
	
	/*
		Abstract Sound Layer
		*** Due to stream concerns, memory won't be freed by CORE, so you should handle passed memories by yourself
	 */
	class SORA_API SoraSoundSystem {
	public:
		virtual ~SoraSoundSystem() {}
		
		virtual int32 init() = 0;
		virtual void shutdown() = 0;
		
        virtual SoraMusicFile* createMusicFile(const SoraWString& path, bool bStream=false) = 0;
        virtual SoraSoundEffectFile* createSoundEffectFile(const SoraWString& path) = 0;
        virtual SoraMusicFile* createMusicFile(bool bStream=false) = 0;
        virtual SoraSoundEffectFile* createSoundEffectFile() = 0;
		/*
			update sound device
		*/
		virtual void update() = 0;

	};
	
} // namespace sora


#endif