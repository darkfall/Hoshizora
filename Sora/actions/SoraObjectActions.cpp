//
//  SoraObjectModifiers.cpp
//  Sora
//
//  Created by Robert Bu on 7/15/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "actions/SoraObjectActions.h"
#include "SoraObject.h"

namespace sora {
    
    SoraMoveAction::SoraMoveAction(float startx, float starty, float endx, float endy, float inTime):
    mBegin(startx, starty),
    mEnd(endx, endy) {
        mCurrValue = mBegin;
        this->setDuration(inTime);
    }
    
    SoraMoveAction::SoraMoveAction(const SoraVector& start, const SoraVector& end, float inTime):
    mBegin(start),
    mEnd(end) {
        mCurrValue = mBegin;
        this->setDuration(inTime);
    }
    
    SoraMoveAction::~SoraMoveAction() {
        
    }
    
    SoraAction::Ptr SoraMoveAction::ActionWithBeginAndEnd(const SoraVector& begin,
                                                          const SoraVector& end,
                                                          float time) {
        return new SoraMoveAction(begin, end, time);
    }
    
    void SoraMoveAction::startWithTarget(SoraObject* target) {
        mCurrValue = mBegin;
        
        SoraFiniteAction::startWithTarget(target);
    }
    
    void SoraMoveAction::step(float dt) {
        SoraFiniteAction::step(dt);
        
        mCurrValue = lerp(mBegin, mEnd, getCurrentTime()/getDuration());
        
        getTarget()->setPosition(mCurrValue.x, mCurrValue.y);
    }
    
    SoraAction::Ptr SoraMoveAction::reverse() const {
        return new SoraMoveAction(mEnd, 
                                  mBegin,
                                  getDuration());
    }
    
    SoraMoveToAction::SoraMoveToAction(float endx, float endy, float inTime):
    mEnd(endx, endy) {
        mCurrValue = mBegin;
        this->setDuration(inTime);
    }
    
    SoraMoveToAction::SoraMoveToAction(const SoraVector& end, float inTime):
    mEnd(end) {
        mCurrValue = mBegin;
        this->setDuration(inTime);
    }
    
    SoraMoveToAction::~SoraMoveToAction() {
        
    }
    
    void SoraMoveToAction::startWithTarget(SoraObject* target) {
        mCurrValue = mBegin;
        mBegin = SoraVector(target->getPositionX(),
                            target->getPositionY());
        
        SoraFiniteAction::startWithTarget(target);
        
    }
    
    void SoraMoveToAction::step(float dt) {
        mCurrValue = lerp(mBegin, mEnd, getCurrentTime()/getDuration());
        
        getTarget()->setPosition(mCurrValue.x, mCurrValue.y);
    }
    
    SoraAction::Ptr SoraMoveToAction::reverse() const {
        return new SoraMoveToAction(mEnd, 
                                    getDuration());
    }
   
    SoraAction::Ptr SoraMoveToAction::ActionWithDest(const SoraVector& dest,
                                                     float time) {
        return new SoraMoveToAction(dest, time);
    }
    
    SoraMoveByAction::SoraMoveByAction(float endx, float endy, float inTime):
    mDt(endx, endy) {
        mCurrValue = mBegin;
        this->setDuration(inTime);
    }
    
    SoraMoveByAction::SoraMoveByAction(const SoraVector& end, float inTime):
    mDt(end) {
        mCurrValue = mBegin;
        this->setDuration(inTime);
    }
    
    SoraMoveByAction::~SoraMoveByAction() {
        
    }
    
    void SoraMoveByAction::startWithTarget(SoraObject* target) {
        mCurrValue = mBegin;
        mBegin = SoraVector(target->getPositionX(),
                            target->getPositionY());
        
        SoraFiniteAction::startWithTarget(target);
        
    }
    
    SoraAction::Ptr SoraMoveByAction::ActionWithDelta(const SoraVector& vec, float time) {
        return new SoraMoveByAction(vec, time);
    }
    
    void SoraMoveByAction::step(float dt) {
        mCurrValue = lerp(mBegin, mBegin + mDt, getCurrentTime()/getDuration());
        
        getTarget()->setPosition(mBegin.x + mCurrValue.x, 
                                 mBegin.y + mCurrValue.y);
    }
    
    SoraAction::Ptr SoraMoveByAction::reverse() const {
        return new SoraMoveByAction(-mDt,
                                    getDuration());
    }
    
} // namespace sora
