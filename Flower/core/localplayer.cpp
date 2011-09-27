//
//  localplayer.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "localplayer.h"

#include "SoraiOSGLRenderer/SoraiOSInput.h"
#include "SoraiOSGLRenderer/SoraiOSInitializer.h"

#include "../controller/defines.h"
#include "../controller/ConfigController.h"

namespace flower {
    
    LocalPlayer::LocalPlayer() {
        mControlMode = ConfigController::CurrentControlMode;
    }
    
    LocalPlayer::~LocalPlayer() {

    }
    
    uint32 LocalPlayer::update(float dt) {
        switch(mControlMode) {
            case CM_ACCELEROMETER:
                accelerometerUpdate();
                break;
                
            case CM_TOUCH:
                touchUpdate(); 
                break;
        }
        
        Player::update(dt);
        
        return 0;
    }
    
    void LocalPlayer::setControllMode(ControlMode mode) {
        mControlMode = mode;
    }
    
    ControlMode LocalPlayer::getControlMode() const {
        return mControlMode;
    }
    
    
    void LocalPlayer::accelerometerUpdate() {
        float x, y, z;
        sora::SoraiOSInitializer::Instance()->getAccelerometerAttr(&x, &y, &z);
        
        float speed = - mMaxSpeed * (y + ConfigController::AccelerometerSensetivity);
        moveTo(getPositionX()+y*speed, getPositionY(), speed);
    }
    
    void LocalPlayer::touchUpdate() {
        
    }

    
} // namespace flower
