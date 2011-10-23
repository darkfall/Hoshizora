//
//  SoraDeltaWatch.h
//  Sora
//
//  Created by Robert Bu on 7/31/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraDeltaWatch_h
#define Sora_SoraDeltaWatch_h

#include "SoraPlatform.h"
#include <deque>

namespace sora {
    
    /**
     *  A special watch for smooth delta 
     *  It will calculate the delta based on the averge of FRAME_COUNT - 1's delta
     *  For smoother animation update
     **/
    template<int FRAME_COUNT = 10>
    class SORA_API SoraDeltaWatch {
    public:
        SoraDeltaWatch(): 
        mCurrDelta(0.f) {
        
        }
        
        void update();
        
        float getDelta() const;
        
    private:
        typedef std::deque<float> FrameQueue;
        FrameQueue mFrames;
        
        float mCurrDelta;
    };
    
    
} // namespace sora

#endif