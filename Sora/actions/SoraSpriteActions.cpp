//
//  SoraSpriteActions.cpp
//  SoraF
//
//  Created by Robert Bu on 2/25/12.
//  Copyright (c) 2012 Game Master Studio. All rights reserved.
//

#include "SoraSpriteActions.h"
#include "SoraMath.h"
#include "SoraObject.h"
#include "SoraSprite.h"

namespace sora {

    SoraSpriteAction::SoraSpriteAction(float to, float time, SoraSpriteActionType type, SoraSpriteActionDestType dest_type):
    mValue(to),
    mActionType(type),
    mDestType(dest_type) {
        this->setDuration(time);
    }
    
    SoraSpriteAction::~SoraSpriteAction() {
        
    }
    
    void SoraSpriteAction::startWithTarget(SoraObject* target) {
        mSprite = dynamic_cast<SoraSprite*>(target);
        if(mSprite) {
            switch(mActionType) {
                case SSAT_Fade:
                    mBegin = CGETA(mSprite->getColor());
                    break;
                case SSAT_Rotate:
                    mBegin = mSprite->getRotation();
                    break;
                case SSAT_Scale:
                    mBegin = mSprite->getVScale();
                    break;
            }
        }
        mCurrent = 0.f;
        
        SoraFiniteAction::startWithTarget(target);
    }
    
    void SoraSpriteAction::step(float dt) {
        SoraFiniteAction::step(dt);
        
        if(mSprite) {
            if(mDestType == SSADT_By)
                mCurrent = lerp(mBegin, mBegin + mValue, getCurrentTime() / getDuration());
            else {
                mCurrent = lerp(mBegin, mValue, getCurrentTime() / getDuration());
            }
            switch(mActionType) {
                case SSAT_Fade:
                    mSprite->setColor(CSETA(mSprite->getColor(), mCurrent));
                    break;
                case SSAT_Rotate:
                    mSprite->setRotation(mCurrent);
                    break;
                case SSAT_Scale:
                    mSprite->setScale(mCurrent, mCurrent);
                    break;
            }
        }
    }
    
    bool SoraSpriteAction::isDone() const {
        return !mSprite || SoraFiniteAction::isDone();
    }
    
    SoraAction::Ptr SoraSpriteAction::reverse() const {
        if(mDestType == SSADT_To) {
            return new SoraSpriteAction(mBegin, getDuration(), mActionType, mDestType);
        } else 
            return new SoraSpriteAction(-mValue, getDuration(), mActionType, mDestType);
    }
    
}
