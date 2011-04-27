/*
 *  SoraSound.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraSound.h"
#include "SoraCore.h"

namespace gcn {

	SoraSound::SoraSound(const std::string& name) {
		pfile = sora::SORA->createSoundEffectFile(sora::s2ws(name));
	}
	
	SoraSound::SoraSound(const std::wstring& name) {
		pfile = sora::SORA->createSoundEffectFile(name);
	}
	
	SoraSound::SoraSound() {
		pfile = NULL;
	}
	
	SoraSound::~SoraSound() {
		if(pfile != NULL) 
			delete pfile;
	}
	
	void SoraSound::play() {
		if(pfile)
			pfile->play();
	}
	
	void SoraSound::stop() {
		if(pfile)
			pfile->stop();
	}


} // namespace gcn