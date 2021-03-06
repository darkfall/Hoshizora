//
//  SoraSystemEvents.h
//  Sora
//
//  Created by Ruiwei Bu on 8/5/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraSystemEvents_h
#define Sora_SoraSystemEvents_h

#include "SoraEvent.h"
#include "SoraPreDeclare.h"

namespace sora {
    
    enum {
        SYS_EVT_UPDATE = 0,
        SYS_EVT_ON_ENABLE,
        SYS_EVT_ON_DISABLE,
        SYS_EVT_ADD_HANDLER,
        SYS_EVT_DEL_HANDLER
    };
    
    class SORA_API SoraSystemEvent: public SoraEvent {
    public:
        SoraSystemEvent(): mType(-1) {}
        SoraSystemEvent(int32 type): mType(SYS_EVT_UPDATE) {}
        
        void setDelta(float32 dt) { 
            mDelta = dt;
        }
        float32 getDelta() const { 
            return mDelta;
        }
        
        void setType(int32 type) {
            mType = type;
        }
        int32 getType() const { 
            return mType;
        }
        
        SORA_EVENT_IDENTIFIER(18446744070299896253ULL);
        
    private:
        int32 mType;
        float32 mDelta;
    };
    
#ifdef SORA_USE_RTTI
    static bool IsSystemEvent(SoraEvent* evt) {
        return isClassClass<SoraEvent, SoraSystemEvent>(evt);
    }
#else
    static bool IsSystemEvent(SoraEvent* evt) {
        return evt->getEventIdentifier() == 18446744070299896253ULL;
    }
#endif
    
    
} // namespace sora

#endif
