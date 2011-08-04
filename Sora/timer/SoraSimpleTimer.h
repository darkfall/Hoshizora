//
//  SoraTimer.h
//  Sora
//
//  Created by Ruiwei Bu on 8/4/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_Timer_SoraTimer_h
#define Sora_Timer_SoraTimer_h

#include "../SoraPlatform.h"
#include "../SoraTimestamp.h"
#include "../SoraDelegate.h"
#include "../AutoContainer.h"
#include "../uncopyable.h"

namespace sora {
    
    class ITimerManager;
    
    class SoraSimpleTimer {
    public:
        typedef float32 TimeType;
        typedef SoraAbstractDelegate< /* Delta */TimeType, bool> TimerFunc;
        
        SoraSimpleTimer(const TimerFunc& func, ITimerManager* owner);
        virtual ~SoraSimpleTimer();
    
        // minimum delta depends on current framerate
        // for example minimun delta = 1
        virtual bool start(TimeType interval, uint32 times, TimeType start);
        virtual void stop();
        virtual void pause(bool flag);
        virtual TimeType play();
        
        virtual bool update(TimeType dt);
        
        bool isPaused() const;
        bool isEnabled() const;
        void setEnable(bool flag);
        
        // time since the timer has started
        TimeType getStartedTime() const;
        // previous delta, maybe larger than defined because of frame update
        TimeType getPrevDelta() const;
                
    private:
        uint32 mTimes;
        bool mEnabled;
        bool mPaused;
        TimerFunc* mTimerFunc;
        
        TimeType mInterval;
        TimeType mStartedTime;
        TimeType mPrevDelta;
        TimeType mCurrDelta;
        
        ITimerManager* mOwner;
    };

    
} // namespace

#endif
