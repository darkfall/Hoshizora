//
//  SoraCameraModifiers.cpp
//  Sora
//
//  Created by Robert Bu on 7/19/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraCameraModifiers.h"

namespace sora {

    SoraCameraZoomModifier::SoraCameraZoomModifier(float starth, float startv, float hz, float hv, float inTime):
    mTime(inTime),
    mCurrTime(0.f),
    mEndZoom(hz, hv),
    mStartZoom(starth, startv) {
        mCurrZoom = mStartZoom;
    }
    
    int32 SoraCameraZoomModifier::update(float dt) {
        mCurrTime += dt;
        mCurrZoom = slerp(mStartZoom, mEndZoom, mCurrTime/mTime);

        if(mCurrTime >= mTime) {
            return ModifierUpdateEnd;
        }
        return 0;
    }
    
    void SoraCameraZoomModifier::modify(SoraCamera* camera) {
        camera->setZoom(mCurrZoom.x, mCurrZoom.y);
    }
    
    void SoraCameraZoomModifier::reset() {
        mCurrZoom = mStartZoom;
        mCurrTime = 0.f;
    }
  
    SoraModifier<SoraCamera>* SoraCameraZoomModifier::clone() {
        return new SoraCameraZoomModifier(mStartZoom.x, 
                                          mStartZoom.y,
                                          mEndZoom.x, 
                                          mEndZoom.y,
                                          mTime);
    }
    
    SoraCameraRotationModifier::SoraCameraRotationModifier(float start, float end, float inTime):
    mTime(inTime),
    mCurrTime(0.f),
    mStart(start),
    mEnd(end) {
        mCurrRot = mStart;
    }
    
    int32 SoraCameraRotationModifier::update(float dt) {
        mCurrTime += dt;
        mCurrRot = slerp(mStart, mEnd, mCurrTime/mTime);
        
        if(mCurrTime >= mTime) {
            return ModifierUpdateEnd;
        }
        return 0;
    }
    
    void SoraCameraRotationModifier::modify(SoraCamera* camera) {
        camera->setRotation(mCurrRot);
    }
    
    void SoraCameraRotationModifier::reset() {
        mCurrRot = mStart;
        mCurrTime = 0.f;
    }
    
    SoraModifier<SoraCamera>* SoraCameraRotationModifier::clone() {
        return new SoraCameraRotationModifier(mStart,
                                              mEnd,
                                              mTime);
    }
    
} // namespace sora
