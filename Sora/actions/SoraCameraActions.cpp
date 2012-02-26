//
//  SoraCameraModifiers.cpp
//  Sora
//
//  Created by Robert Bu on 7/19/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraCameraActions.h"
#include "SoraCamera.h"

namespace sora {

    SoraCameraZoomAction::SoraCameraZoomAction(float starth, float startv, float hz, float hv, float inTime):
    mEndZoom(hz, hv),
    mStartZoom(starth, startv) {
        mCurrZoom = mStartZoom;
        
        this->setDuration(inTime);
    }
    
    SoraCameraZoomAction::~SoraCameraZoomAction() {
        
    }
    
    void SoraCameraZoomAction::step(float dt) {
        SoraFiniteAction::step(dt);
        if(mCamera) {
            mCurrZoom = lerp(mStartZoom, mEndZoom, getCurrentTime()/getDuration());
        
            mCamera->setZoom(mCurrZoom.x, mCurrZoom.y);
        }
    }
    
    SoraAction::Ptr SoraCameraZoomAction::ActionWithZoom(float sh, float sv, float hz, float hv, float time) {
        return new SoraCameraZoomAction(sh, sv, hz, hv, time);
    }
    
    bool SoraCameraZoomAction::isDone() const {
        return (!mCamera ||
                getCurrentTime() > getDuration());
    }
    
    void SoraCameraZoomAction::startWithTarget(SoraObject* target) {
        mCamera = dynamic_cast<Sora2DCamera*>(target);
        mCurrZoom = mStartZoom;
        
        SoraFiniteAction::startWithTarget(target);
    }
    
    SoraAction::Ptr SoraCameraZoomAction::reverse() const {
        return new SoraCameraZoomAction(mEndZoom.x,
                                        mEndZoom.y,
                                        mStartZoom.x,
                                        mStartZoom.y,
                                        getDuration());
    }

    
    SoraCameraRotationAction::SoraCameraRotationAction(float start, float end, float inTime):
    mStart(start),
    mEnd(end) {
        mCurrRot = mStart;
        
        this->setDuration(inTime);
    }
    
    SoraCameraRotationAction::~SoraCameraRotationAction() {
        
    }
    
    void SoraCameraRotationAction::step(float dt) {
        SoraFiniteAction::step(dt);
        if(mCamera) {
            mCurrRot = lerp(mStart, mEnd, getCurrentTime()/getDuration());
            
            mCamera->setRotation(mCurrRot);
        }
    }
    
    bool SoraCameraRotationAction::isDone() const {
        return (!mCamera ||
                getCurrentTime() > getDuration());
    }
    
    SoraAction::Ptr SoraCameraRotationAction::ActionWithRotation(float start, float end, float inTime) {
        return new SoraCameraRotationAction(start, end, inTime);
    }
    
    void SoraCameraRotationAction::startWithTarget(SoraObject* target) {
        mCamera = dynamic_cast<Sora2DCamera*>(target);
        mCurrRot = mStart;
        
        SoraFiniteAction::startWithTarget(target);
    }
    
    SoraAction::Ptr SoraCameraRotationAction::reverse() const {
        return new SoraCameraRotationAction(mEnd,
                                            mStart,
                                            getDuration());
    }

    
} // namespace sora
