//
//  ITimerManager.h
//  Sora
//
//  Created by Ruiwei Bu on 8/4/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_ITimerManager_h
#define Sora_ITimerManager_h

#include "../SoraSingleton.h"
#include "../SoraPlatform.h"
#include "../SoraDelegate.h"

namespace sora {
    
    /**
     * TimerManager Interface
     **/
    class SoraSimpleTimer;
    
    typedef SoraAutoPtr<SoraSimpleTimer> SimpleTimerPtr;
    
    class ITimerManager: public SoraDirectSingleton<ITimerManager> {
    public:
        typedef float32 TimeType;
        typedef SoraAbstractDelegate< /* Interval */TimeType, bool> TimerFunc;
        
        virtual bool update(TimeType dt) = 0;
        virtual SimpleTimerPtr createTimer(const TimerFunc& func) = 0;
        virtual void removeTimer(SoraSimpleTimer* timer) = 0;
        
    };
    
} // namespace sora


#endif
