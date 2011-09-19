//
//  SoraSoundSystem.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraSoundSystem.h"
#include "SoraCore.h"

namespace sora {
    
    SoraMusicFile* SoraSoundSystem::LoadMusicFromFile(const StringType& path, bool isStream) {
        return SoraCore::Instance()->createMusicFile(path, isStream);
    }
    
    SoraSoundEffectFile* LoadSoundEffectFromFile(const StringType& path) {
        return SoraCore::Instance()->createSoundEffectFile(path);
    }
    
} // namespace sora
