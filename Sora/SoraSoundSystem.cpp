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
    
    SoraMusicFile* LoadMusicFromMemory(void* pdata, ulong32 size) {
        return SoraCore::Instance()->createMusicFileFromMemory(pdata, size);
    }
    
    SoraSoundEffectFile* LoaSoundEffectFromMemory(void* pdata, ulong32 size) {
        return SoraCore::Instance()->createSoundEffectFileFromMemory(pdata, size);
    }
} // namespace sora
