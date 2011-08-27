//
//  SoraHandleManager.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/13/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraHandleManager.h"
#include "SoraObjectHandle.h"

namespace sora {
    
    SoraHandleManager::SoraHandleManager():
    m_NextUniqueId(0),
    m_NextUniqueHandle(0) {
    }
    
    SoraHandle SoraHandleManager::getUniqueHandle() {
        if(m_FreeHandles.empty()) {
            ++m_NextUniqueHandle;
            if(m_NextUniqueHandle > MAX_OBJECT_SIZE)
                throw SORA_EXCEPTION(PoolOverflowException, "MAX OBJECT SIZE REACHED");
            return m_NextUniqueHandle;
        } else {
            int32 slot = m_FreeHandles.back();
            m_FreeHandles.pop_back();
            return slot;
        }
    }
    
    SoraUniqueId SoraHandleManager::getNextUniqieId() {
        return m_NextUniqueId++;
    }
    
    void SoraHandleManager::freeHandle(SoraHandle handle) {
        m_FreeHandles.push_back(handle);
        SoraObjectHandle::freeObjectHandle(handle);
    }
    
} // namespace sora
