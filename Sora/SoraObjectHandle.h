//
//  SoraObjectHandle.h
//  Sora
//
//  Created by Ruiwei Bu on 7/13/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraObjectHandle_h
#define Sora_SoraObjectHandle_h

#include "SoraObject.h"

namespace sora {
    
    const uint32 MAX_OBJECT_SIZE = 65535;
    static SoraObject* g_ObjectMap[MAX_OBJECT_SIZE];

    class SoraObjectHandle {
    public:
        SoraObjectHandle();
        explicit SoraObjectHandle(SoraObject* obj);
        SoraObjectHandle(SoraHandle handle, SoraUniqueId uid);
        
        SoraObject* toObject();
        
        SoraObjectHandle& operator =(const SoraObjectHandle& rhs);
        
        bool operator==(const SoraObjectHandle& rhs) const;
        
        operator SoraObject*();
        
        SoraHandle getHandleId() const;
        SoraUniqueId getUniqueId() const;
        
        static void freeObjectHandle(SoraHandle handle);
        
    private:
        SoraHandle m_HandleId;
        SoraUniqueId m_UniqueId;
    }; 
    
    static SoraObjectHandle toObjectHandle(SoraObject* obj) {
        return SoraObjectHandle(obj);
    }
    
} // namespace sora



#endif
