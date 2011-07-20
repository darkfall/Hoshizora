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
    
    const uint32 MAX_OBJECT_SIZE = 65525;

    class SoraObjectHandle {
    public:
        explicit SoraObjectHandle(SoraObject* obj);
        ~SoraObjectHandle();
        
        SoraObject* toObject();
        
        SoraObjectHandle& operator =(const SoraObjectHandle& rhs);
        
        bool operator==(const SoraObjectHandle& rhs) const;
        
        operator SoraObject*();
        
        SoraHandle getHandleId() const;
        SoraUniqueId getUniqueId() const;
        
        static void freeObjectHandle(SoraHandle handle);
        static uint32 getGlobalObjectSize();
        
    private:
        SoraObjectHandle(SoraHandle handle, SoraUniqueId uid);
        SoraObjectHandle();

        SoraHandle m_HandleId;
        SoraUniqueId m_UniqueId;
    }; 
    
    static SoraObjectHandle toObjectHandle(SoraObject* obj) {
        return SoraObjectHandle(obj);
    }
    
} // namespace sora



#endif
