/*
 *  SoraSound.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_SOUND_H_
#define SORA_SOUND_H_

#include "Sound.h"
#include "SoraSoundFile.h"

namespace gcn {
	
	class SoraSound: public Sound {
		friend class SoraSoundLoader;
		
		SoraSound(const std::string& name);
		SoraSound(const std::wstring& name);
		SoraSound();
		
	public:
		virtual ~SoraSound();
		
		void play();
		void stop();
		
	private:
		sora::SoraSoundEffectFile* pfile;
	};
	
} // namespace sora

#endif // SORA_SOUND_H_