//
//  SoraObjectModifiers.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/15/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraObjectModifiers.h"
 
namespace sora {
    
    SoraObjectTransitionModifier::SoraObjectTransitionModifier(float32 startx, float32 starty, float32 endx, float32 endy, float32 inTime):
    mBegin(startx, starty),
    mEnd(endx, endy),
    mInTime(inTime) {
        mCurrValue = mBegin;
        mCurrTime = 0.f;
    }
    
    SoraObjectTransitionModifier::SoraObjectTransitionModifier(const SoraVector& start, const SoraVector& end, float32 inTime):
    mBegin(start),
    mEnd(end),
    mInTime(inTime) {
        mCurrValue = mBegin;
        mCurrTime = 0.f;
    }
    
    int32   SoraObjectTransitionModifier::update(float32 dt) {
        mCurrTime += dt;
        mCurrValue = (mEnd - mBegin) * (mCurrTime / mInTime) + mBegin;
        if(mCurrTime >= mInTime) {
            return ModifierUpdateEnd;
        }
        return 0;

    }
    void SoraObjectTransitionModifier::modify(SoraObject* obj) {
        obj->setPosition(mCurrValue.x, mCurrValue.y);
    }
    
    void SoraObjectTransitionModifier::reset() {
        mCurrValue = mBegin;
        mCurrTime = 0.f;
    }
    
    SoraModifier<SoraObject>* SoraObjectTransitionModifier::clone() {
        return new SoraObjectTransitionModifier(mBegin, mEnd, mInTime);
    }
    
} // namespace sora
