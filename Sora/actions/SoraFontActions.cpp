//
//  SoraFontModifiers.cpp
//  Sora
//
//  Created by Robert Bu on 7/15/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraFontActions.h"
#include "SoraMath.h"
#include "SoraFont.h"
#include "SoraObject.h"

namespace sora {
    
    SoraFontRotateAction::SoraFontRotateAction(float begin, float end, float inTime, bool lineRotation, bool rotateChar):
    mBegin(begin),
    mEnd(end),
    mLineRotation(lineRotation),
    mRotateChar(rotateChar) {
        mCurrVal = mBegin;
        
        this->setDuration(inTime);
    }
    
    SoraFontRotateAction::~SoraFontRotateAction() {
        
    }
    
    void SoraFontRotateAction::step(float dt) {
        SoraFiniteAction::step(dt);
        if(mFont) {
            mCurrVal = lerp(mBegin, mEnd, getCurrentTime()/getDuration());
            
            if(!mLineRotation)
                mFont->setCharRotation(mCurrVal);
            else 
                mFont->setLineRotation(mCurrVal, mRotateChar);
        }
    }
    
    void SoraFontRotateAction::startWithTarget(SoraObject* target) {
        mFont = dynamic_cast<SoraFont*>(target);
        mCurrVal = mBegin;
        
        SoraFiniteAction::startWithTarget(target);
    }
    
    SoraAction::Ptr SoraFontRotateAction::ActionWithRotation(float begin, float end, float time) {
        return new SoraFontRotateAction(begin, end, time);
    }
    
    SoraAction::Ptr SoraFontRotateAction::reverse() const {
        return new SoraFontRotateAction(mBegin, 
                                        mEnd, 
                                        getDuration(), 
                                        mLineRotation, 
                                        mRotateChar);
    }
    
    bool SoraFontRotateAction::isDone() const {
        return (!mFont ||
                getCurrentTime() > getDuration());
    }
    
    SoraFontScaleAction::SoraFontScaleAction(float begin, float end, float inTime):
    mBegin(begin),
    mEnd(end) {
        mCurrVal = begin;
        
        this->setDuration(inTime);
    }
    
    SoraFontScaleAction::~SoraFontScaleAction() {
        
    }
    
    void SoraFontScaleAction::step(float dt) {
        SoraFiniteAction::step(dt);
        if(mFont) {
            mCurrVal = lerp(mBegin, mEnd, getCurrentTime()/getDuration());
        
            mFont->setScale(mCurrVal);
        }
    }
    
    bool SoraFontScaleAction::isDone() const {
        return !mFont || SoraFiniteAction::isDone();
    }
    
    void SoraFontScaleAction::startWithTarget(SoraObject* target) {
        mFont = dynamic_cast<SoraFont*>(target);
        mCurrVal = mBegin;

        SoraFiniteAction::startWithTarget(target);
    }
    
    SoraAction::Ptr SoraFontScaleAction::ActionWithScale(float begin, float end, float time) {
        return new SoraFontScaleAction(begin, end, time);
    }
    
    SoraAction::Ptr SoraFontScaleAction::reverse() const {
        return new SoraFontScaleAction(mEnd,
                                        mBegin,
                                        getDuration());
    }
    
} // namespace sora
