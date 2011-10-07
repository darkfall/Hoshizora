//
//  SoraDeltaWatch.cpp
//  Sora
//
//  Created by Robert Bu on 7/31/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraDeltaWatch.h"
#include "SoraCore.h"

namespace sora {
    
    template<int FRAME_COUNT>
    void SoraDeltaWatch<FRAME_COUNT>::update() {
        float delta = SoraCore::Ptr->getDelta();
        
        mFrames.push_back(delta);
        if(mFrames.size() >= FRAME_COUNT)
            mFrames.pop_front();
        
        if(mCurrDelta == 0.f)
            mCurrDelta = delta;
        else {
            mCurrDelta = (mCurrDelta * (mFrames.size() - 1) + delta) / (mFrames.size() + 1);
        }
    }
    
    template<int FRAME_COUNT>
    float SoraDeltaWatch<FRAME_COUNT>::getDelta() const {
        return mCurrDelta;
    }
    
} // namespace sora