//
//  SoraFMODSoundSystem.h
//  Sora
//
//  Created by Griffin Bu on 4/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef SORA_FMOD_SOUND_SYSTEM_H_
#define SORA_FMOD_SOUND_SYSTEM_H_

#include "SoraPlatform.h"
#include "SoraSoundSystem.h"

#include "SoraFMODSystem.h"

namespace sora {
    
    class SoraFMODSoundSystem: public SoraSoundSystem {
    public:
        SoraFMODSoundSystem();
        
        int32 init();
		void shutdown();
		
        SoraMusicFile* createMusicFile(const util::String& path, bool bStream=false);
        SoraSoundEffectFile* createSoundEffectFile(const util::String& path);
        SoraMusicFile* createMusicFile(bool bStream=false);
        SoraSoundEffectFile* createSoundEffectFile();
		/*
         update sound device
         */
		void update();
        
    private:
        SoraFMODSystem* pfs;
    };
} // namespace sora


#endif