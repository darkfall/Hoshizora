//
//  SoraTimerManager.h
//  Sora
//
//  Created by Ruiwei Bu on 8/4/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraTimerManager_h
#define Sora_SoraTimerManager_h

#include "ITimerManager.h"
#include "SoraSimpleTimer.h"
#include <list>

namespace sora {
    
    class SoraSimpleTimerManager: public ITimerManager {
    public:
        typedef std::list<SoraSimpleTimer*> TimerList;
        
        SoraSimpleTimerManager();
        ~SoraSimpleTimerManager();
        
        bool update(TimeType dt);
        SimpleTimerPtr createTimer(const TimerFunc& func);
        void removeTimer(SoraSimpleTimer* timer);
        
    private:
        void addList();
        
        TimerList mTimerList;
        TimerList mAddList;
    };
    
    static SimpleTimerPtr CreateSimpleTimer(const ITimerManager::TimerFunc& func) {
        return SoraSimpleTimerManager::RefInstance().createTimer(func);
    }
    
} // namespace sora


#endif
