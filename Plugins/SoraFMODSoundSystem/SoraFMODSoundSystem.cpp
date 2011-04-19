//
//  SoraFMODSoundSystem.cpp
//  Sora
//
//  Created by Griffin Bu on 4/18/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraFMODSoundSystem.h"
#include "SoraFMODSoundFile.h"

#include "debug/SoraInternalLogger.h"

namespace sora {

    SoraFMODSoundSystem::SoraFMODSoundSystem() {
        pfs = SoraFMODSystem::Instance();
    }
        
    int32 SoraFMODSoundSystem::init() {
        return pfs->init(32, FMOD_INIT_NORMAL, NULL);
    }
    
    void SoraFMODSoundSystem::shutdown() {
        SoraFMODSystem::Destroy();
    }
    
    SoraMusicFile* SoraFMODSoundSystem::createMusicFile(const SoraWString& path, bool bStream) {
        return new SoraFMODMusicFile(path, bStream);
    }
    
    SoraSoundEffectFile* SoraFMODSoundSystem::createSoundEffectFile(const SoraWString& path) {
        return new SoraFMODSoundEffectFile(path);
    }
    
    SoraMusicFile* SoraFMODSoundSystem::createMusicFile(bool bStream) {
        return new SoraFMODMusicFile(bStream);
    }
    
    SoraSoundEffectFile* SoraFMODSoundSystem::createSoundEffectFile() {
        return new SoraFMODSoundEffectFile;
    }
    
    void SoraFMODSoundSystem::update() {
        pfs->update();
    }
   
} // namespace sora