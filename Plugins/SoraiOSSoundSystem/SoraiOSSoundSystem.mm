//
//  SoraiOSSoundSystem.mm
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/15/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "SoraiOSSoundSystem.h"

#import "SoraOALController.h"
#import "SoraiOSSoundFile.h"

namespace sora {
    
    SoraiOSSoundSystem::SoraiOSSoundSystem(const SoraOALParameters& params) {
        mParams = params;
    }
    
    int32 SoraiOSSoundSystem::init() {
        return SoraOALController::init(mParams) ? 1 : 0;
    }
    
    void SoraiOSSoundSystem::shutdown() {
        SoraOALController::release();
    }
    
    SoraMusicFile* SoraiOSSoundSystem::createMusicFile(const SoraWString& path, bool bStream) {
        return new SoraiOSMusicFile(path, bStream);
    }
    
    SoraSoundEffectFile* SoraiOSSoundSystem::createSoundEffectFile(const SoraWString& path) {
        return new SoraiOSSoundEffectFile(path);
    }
    
    SoraMusicFile* SoraiOSSoundSystem::createMusicFile(bool bStream) {
        return new SoraiOSMusicFile(bStream);
    }
    
    SoraSoundEffectFile* SoraiOSSoundSystem::createSoundEffectFile() {
        return new SoraiOSSoundEffectFile();
    }
   
    void SoraiOSSoundSystem::update() {
        // do nothing
    }
    
    SoraMusicFile* SoraiOSSoundSystem::createMusicFileFromMemory(void* data, uint32 size) {
        return 0;
    }
    
    SoraSoundEffectFile* SoraiOSSoundSystem::createSoundEffectFileFromMemory(void* data, uint32 size) {
        return 0;
    }
    
} // namespace sora
