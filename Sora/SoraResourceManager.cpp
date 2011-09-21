//
//  SoraResourceManager.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/20/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraResourceManager.h"
#include "SoraCore.h"

namespace sora {
    
    SoraResourceHandle SoraResourceManager::LoadAndAttachResourcePack(const StringType& file) {
        return SoraCore::Instance()->loadResourcePack(file);
    }
    
    void SoraResourceManager::DetachResourcePack(SoraResourceHandle handle) {
        SoraCore::Instance()->detachResourcePack(handle);
    }
    
    void* SoraResourceManager::LoadResourceFile(const StringType& file, ulong32* size) {
        return SoraCore::Instance()->getResourceFile(file, *size);
    }
    
    ulong32 SoraResourceManager::GetResourceFileSize(const StringType& file) {
        return SoraCore::Instance()->getResourceFileSize(file);
    }
   
    void SoraResourceManager::FreeResourceFile(void* p) {
        SoraCore::Instance()->freeResourceFile(p);
    }
    
#ifdef SORA_ENABLE_MULTI_THREAD
    
    void SoraResourceManager::LoadResourceFileAsync(const StringType& file, const AsyncNotification& notification, void* puserdata) {
        SoraCore::Instance()->loadResourceFileAsync(file, notification, puserdata);
    }
    
#endif
    
} // namespace sora
