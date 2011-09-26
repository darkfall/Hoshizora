//
//  deltaupdater.h
//  Sora
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_deltaupdater_h
#define Sora_deltaupdater_h

namespace atd {
    
    struct DeltaUpdater {
        DeltaUpdater():
        mCurrentTime(0.f),
        mInterval(0.f) {
            
        }
        
        DeltaUpdater(float interval):
        mCurrentTime(0.f),
        mInterval(interval) {
            
        }
        
        void setInterval(float dt) {
            mInterval = dt;
            mCurrentTime = 0.f;
        }
        
        float getInterval() const {
            return mInterval;
        }
        
        bool update(float dt) {
            if(mInterval != 0.f) {
                mCurrentTime += dt;
                if(mCurrentTime >= mInterval) {
                    mCurrentTime = 0.f;
                    return true;
                }
            }
            return false;
        }
        
        float mCurrentTime;
        float mInterval;
    };
    
} // namespace atd


#endif
