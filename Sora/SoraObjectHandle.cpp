//
//  SoraObjectHandle.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/13/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraObjectHandle.h"

namespace sora {
            

    SoraObjectHandle::SoraObjectHandle(SoraObject* obj):
    m_HandleId(obj->getHandleId()),
    m_UniqueId(obj->getUniqueId()) {
        g_ObjectMap[m_HandleId] = obj;
    }
    
    SoraObjectHandle::SoraObjectHandle(SoraHandle handle, SoraUniqueId uid):
    m_HandleId(handle),
    m_UniqueId(uid) {
        
    }
        
    SoraObject* SoraObjectHandle::toObject() {
        assert(m_HandleId < MAX_OBJECT_SIZE);
        SoraObject* object = g_ObjectMap[m_HandleId];
        if(object != NULL &&
           object->getUniqueId() == m_UniqueId)
            return object;
        return NULL;
    }
    
    SoraObjectHandle& SoraObjectHandle::operator =(const SoraObjectHandle& rhs) {
        if(&rhs != this) {
            m_HandleId = rhs.m_HandleId;
            m_UniqueId = rhs.m_UniqueId;
        }
    }
    
    bool SoraObjectHandle::operator==(const SoraObjectHandle& rhs) const {
        return m_HandleId == rhs.m_HandleId && m_UniqueId == rhs.m_UniqueId;
    }
    
    SoraObjectHandle::operator SoraObject*() {
        return toObject();
    }
    
    SoraHandle SoraObjectHandle::getHandleId() const {
        return m_HandleId;
    }
    
    SoraUniqueId SoraObjectHandle::getUniqueId() const {
        return m_UniqueId;
    }

    void SoraObjectHandle::freeObjectHandle(SoraHandle handle) {
        assert(handle < MAX_OBJECT_SIZE);
        g_ObjectMap[handle] = NULL;
    }
} // namespace sora