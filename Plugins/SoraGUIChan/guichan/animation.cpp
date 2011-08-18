//
//  animation.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/10/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "guichan/animation.hpp"

namespace gcn {
    
    Animation::Animation(int time):
    Modifier(true),
    mTotalTime(time),
    mCurrTime(0) {
        
    }
    
    void Animation::update(Widget* widget) {
        if(!isFinished()) {
            ++mCurrTime;
            if(mCurrTime >= mTotalTime)
                setFinish(true);
        }
    }
    
    void Animation::reset(int time) {
        setFinish(false);
        
        mCurrTime = 0;
        mTotalTime = time;
    }
    
    void Animation::setFinish(bool flag) {
        if(flag) 
            reset(mTotalTime);
        Modifier::setFinish(flag);
    }
    
    int Animation::getTotalTime() const {
        return mTotalTime;
    }
    
    int Animation::getCurrTime() const {
        return mCurrTime;
    }
    
    float Animation::getCurrTimeRatio() const {
        return (float)mCurrTime / mTotalTime;
    }
    
} // namespace gcn