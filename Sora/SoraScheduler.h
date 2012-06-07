//
//  SoraScheduler.h
//  SoraF
//
//  Created by Robert Bu on 2/26/12.
//  Copyright (c) 2012 Game Master Studio. All rights reserved.
//

#ifndef SoraF_SoraScheduler_h
#define SoraF_SoraScheduler_h

#include "SoraPlatform.h"
#include "SoraFunction.h"
#include <list>

namespace sora {
    
    class SoraObject;
    

    class SoraScheduler {
    public:
        typedef SoraBind<SoraObject, void(float)> BindType;
        typedef SoraMemfun<void(float), SoraObject> MemfunType;
        
        struct Schedule;
        
        SoraScheduler(SoraObject* owner);
        ~SoraScheduler();
        
        void schedule(MemfunType::Memfun, float interval);
        void unschedule(MemfunType::Memfun);
        
        void update(float dt);
        
    private:
        typedef std::list<Schedule> ScheduleList;
        ScheduleList mScheduled;
        
        SoraObject* mOwner;
    };
    
    
    template<typename T>
    inline SoraScheduler::MemfunType::Memfun ScheduleFunc(void (T::*Func)(float)) {
        return (SoraScheduler::MemfunType::Memfun)Func;
    }
    
}


#endif
