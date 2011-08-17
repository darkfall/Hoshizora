//
//  SoraHandleManager.h
//  Sora
//
//  Created by Ruiwei Bu on 7/13/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraHandleManager_h
#define Sora_SoraHandleManager_h

#include "SoraPlatform.h"
#include "SoraSingleton.h"

#include <deque>
#include <map>

namespace sora {
    
    class SORA_API SoraHandleManager: public SoraSingleton<SoraHandleManager> {
    protected:
        friend class SoraSingleton<SoraHandleManager>;
        SoraHandleManager();
        
    public:
        SoraHandle getUniqueHandle();
        SoraUniqueId getNextUniqieId();
        
        void freeHandle(SoraHandle handle);
                
    private:
        SoraHandle m_NextUniqueId;
        SoraUniqueId m_NextUniqueHandle;
        
        typedef std::deque<SoraUniqueId> FreeUniqieHandles;
        FreeUniqieHandles m_FreeHandles;
    };
    
    static SoraUniqueId GetNextUniqueId() {
        return SoraHandleManager::Instance()->getNextUniqieId();
    }
    
    static SoraHandle FindFreeHandleSlot() {
        return SoraHandleManager::Instance()->getUniqueHandle();
    }
    
    static void FreeHandleSlot(SoraHandle handle) {
        SoraHandleManager::Instance()->freeHandle(handle);
    }
    
} // namespace sora


#endif
