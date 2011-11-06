//
//  SoraFMODSoundSystem.h
//  Sora
//
//  Created by Griffin Bu on 4/18/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef SORA_FMOD_SOUND_SYSTEM_H_
#define SORA_FMOD_SOUND_SYSTEM_H_

#include "SoraPlatform.h"
#include "SoraSoundSystem.h"
#include "SoraOALParam.h"

/**
 * This header is safe to be included in C++ source files
 **/

namespace sora {
    
    class SoraiOSSoundSystem: public SoraSoundSystem {
    public:
        SoraiOSSoundSystem(const SoraOALParameters& param);
        virtual ~SoraiOSSoundSystem() {}
        
        int32 init();
		void shutdown();
		
        SoraMusicFile* createMusicFile(const SoraWString& path, bool bStream=false);
        SoraSoundEffectFile* createSoundEffectFile(const SoraWString& path);
        SoraMusicFile* createMusicFile(bool bStream=false);
        SoraSoundEffectFile* createSoundEffectFile();
        
        SoraMusicFile* createMusicFileFromMemory(void* data, uint32 size);
        SoraSoundEffectFile* createSoundEffectFileFromMemory(void* data, uint32 size);
		/*
         update sound device
         */
		void update();
        
    private:
        SoraOALParameters mParams;
    };
} // namespace sora


#endif