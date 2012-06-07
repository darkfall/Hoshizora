//
//  SoraScheduler.cpp
//  SoraF
//
//  Created by Robert Bu on 2/26/12.
//  Copyright (c) 2012 Game Master Studio. All rights reserved.
//

#include "SoraScheduler.h"
#include "SoraObject.h"
#include "SoraForEach.h"

namespace sora {
    
    struct SoraScheduler::Schedule {
        float mCurrentTime;
        float mInterval;
        
        SoraScheduler::BindType mFunc;
        
        bool update(float dt) {
            mCurrentTime += dt;
            if(mCurrentTime >= mInterval) {
                mCurrentTime = 0.f;
                return true;
            }
            return false;
        }
        
        Schedule(const SoraScheduler::BindType& func, float interval):
        mFunc(func),
        mInterval(interval),
        mCurrentTime(0) {
        }
        
        bool operator==(const SoraScheduler::BindType& func) const {
            return mFunc.getMemfun() == func.getMemfun();
        }
        
        bool operator==(SoraScheduler::MemfunType::Memfun func) const {
            return mFunc.getMemfun() == func;
        }
    };
    
    SoraScheduler::SoraScheduler(SoraObject* owner):
    mOwner(owner) {
        
    }
    
    SoraScheduler::~SoraScheduler() {
        
    }
    
    void SoraScheduler::schedule(MemfunType::Memfun func, float interval) {
        mScheduled.push_back(Schedule(Bind(mOwner, func), interval));
    }
    
    void SoraScheduler::unschedule(MemfunType::Memfun func) {
        ScheduleList::iterator it = std::find(mScheduled.begin(),
                                              mScheduled.end(),
                                              func);
        if(it != mScheduled.end())
            mScheduled.erase(it);
    }
    
    void SoraScheduler::update(float dt) {
        SORA_FOR_EACH(Schedule& s, mScheduled) {
            if(s.update(dt)) {
                float ct = s.mCurrentTime;
                s.mFunc(ct + dt);
            }
        }
    }
    
}
