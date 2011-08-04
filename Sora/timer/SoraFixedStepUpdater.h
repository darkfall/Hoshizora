//
//  SoraFixedStepUpdater.h
//  Sora
//
//  Created by Ruiwei Bu on 8/4/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraFixedStepUpdater_h
#define Sora_SoraFixedStepUpdater_h

#include "../SoraPlatform.h"

namespace sora {
    
    /**
     * Object Update in a specific steptime
     **/
    
    template<typename T>
    class SoraFixedStepUpdater {
    public:
        inline SoraFixedStepUpdater(float32 stepTime):
        mUpdateObj(NULL),
        mStepTime(stepTime),
        mAccumulator(0.f) {
    
        }
        
        inline ~SoraFixedStepUpdater() {}
        
        inline void enable(T* obj) {
            mUpdateObj = obj;
            mAccumulator = 0.f;
        }
        
        inline void disable(T* obj) {
            if(mUpdateObj == obj)
                mUpdateObj = NULL;
        }
        
        inline uint32 update(float32 interval) {
            uint32 result = 0;
            if(mUpdateObj) {
                mAccumulator += interval;
                
                uint32 step = uint32(mAccumulator / mStepTime);
                if(step > 0) {
                    result = mUpdateObj->update(mAccumulator);
                    mAccumulator -= step*stepTime;
                }
            }
            return result;
        }
        
    private:
        T* mUpdateObj;
        const float32 mStepTime;
        float32 mAccumulator;
    };
    
} // namespace sora

#endif
