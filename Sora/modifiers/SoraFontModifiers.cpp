//
//  SoraFontModifiers.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/15/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraFontModifiers.h"

namespace sora {
    
    SoraFontRotationModifier::SoraFontRotationModifier(float32 begin, float32 end, float32 inTime, bool lineRotation, bool rotateChar):
    mBegin(begin),
    mEnd(end),
    mInTime(inTime),
    mCurrTime(0.f),
    mLineRotation(lineRotation),
    mRotateChar(rotateChar) {
        mCurrVal = mBegin;
    }
    
    int32 SoraFontRotationModifier::update(float32 dt) {
        mCurrTime += dt;
        mCurrVal = (mEnd - mBegin) * (mCurrTime / mInTime) + mBegin;
        if(mCurrTime >= mInTime) {
            return ModifierUpdateEnd;
        }
        return 0;
    }
    
    void SoraFontRotationModifier::reset() {
        mCurrTime = 0.f;
        mCurrVal = mBegin;
    }
    
    void SoraFontRotationModifier::modify(SoraFont* font) {
        if(!mLineRotation)
            font->setCharRotation(mCurrVal);
        else 
            font->setLineRotation(mCurrVal, mRotateChar);
    }
    
    SoraModifier<SoraFont>* SoraFontRotationModifier::clone() {
        return new SoraFontRotationModifier(mBegin, mEnd, mInTime, mLineRotation, mRotateChar);
    }
    
    SoraFontScaleModifier::SoraFontScaleModifier(float32 begin, float32 end, float32 inTime):
    mBegin(begin),
    mEnd(end),
    mInTime(inTime) {
        mCurrTime = 0.f;
        mCurrVal = begin;
    }
    
    int32 SoraFontScaleModifier::update(float32 dt) {
        mCurrTime += dt;
        mCurrVal = (mEnd - mBegin) * (mCurrTime / mInTime) + mBegin;
        if(mCurrTime >= mInTime) {
            return ModifierUpdateEnd;
        }
        return 0;
    }
    
    void SoraFontScaleModifier::reset() {
        mCurrTime = 0.f;
        mCurrVal = mBegin;
    }
    
    void SoraFontScaleModifier::modify(SoraFont* font) {
        font->setScale(mCurrVal);
    }
    
    SoraModifier<SoraFont>* SoraFontScaleModifier::clone() {
        return new SoraFontScaleModifier(mBegin, mEnd, mInTime);
    }
    
} // namespace sora
