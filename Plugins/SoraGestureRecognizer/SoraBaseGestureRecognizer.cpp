//
//  SoraBaseGestureRecognizer.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/1/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraBaseGestureTypeRecognizer.h"

namespace sora {
    
    SoraGestureType SoraBaseGestureTypeRecognizer::recognizeGesture(const SoraGesture::GestureSigPointList& sigPoints) {
        SoraGestureType type = GESTURE_UNRECOGNIZED;
        
        if(sigPoints.size() <= 1) {
            type = GESTURE_UNRECOGNIZED;
        } else if(sigPoints.size() == 2) {
            switch(sigPoints[0].mDirection) {
                case GESTURE_DIR_UP: type = GESTURE_SLIDE_UP; break;
                case GESTURE_DIR_DOWN: type = GESTURE_SLIDE_DOWN; break;
                case GESTURE_DIR_LEFT: type = GESTURE_SLIDE_LEFT; break;
                case GESTURE_DIR_RIGHT: type = GESTURE_SLIDE_RIGHT; break;
                default:
                    type = GESTURE_UNRECOGNIZED;
                    break;
            }
        } else if(sigPoints.size() == 3) {
            switch(sigPoints[0].mDirection) {
                case GESTURE_DIR_UP:
                    switch(sigPoints[1].mDirection) {
                        case GESTURE_DIR_LEFT: type = GESTURE_SLIDE_UP_LEFT; break;
                        case GESTURE_DIR_RIGHT: type = GESTURE_SLIDE_UP_RIGHT; break;
                        default:
                            type = GESTURE_UNRECOGNIZED;
                            break;
                    }
                    break;
                    
                case GESTURE_DIR_DOWN:
                    switch(sigPoints[1].mDirection) {
                        case GESTURE_DIR_LEFT: type = GESTURE_SLIDE_DOWN_LEFT; break;
                        case GESTURE_DIR_RIGHT: type = GESTURE_SLIDE_DOWN_RIGHT; break;
                        default:
                            type = GESTURE_UNRECOGNIZED;
                            break;
                    }
                    break;
                    
                case GESTURE_DIR_LEFT:
                    switch(sigPoints[1].mDirection) {
                        case GESTURE_DIR_UP: type = GESTURE_SLIDE_LEFT_UP; break;
                        case GESTURE_DIR_DOWN: type = GESTURE_SLIDE_LEFT_DOWN; break;
                        default:
                            type = GESTURE_UNRECOGNIZED;
                            break;
                    }
                    break;
                    
                case GESTURE_DIR_RIGHT:
                    switch(sigPoints[1].mDirection) {
                        case GESTURE_DIR_UP: type = GESTURE_SLIDE_RIGHT_UP; break;
                        case GESTURE_DIR_DOWN: type = GESTURE_SLIDE_RIGHT_DOWN; break;
                        default:
                            type = GESTURE_UNRECOGNIZED;
                            break;
                    }
                    break;
                    
                default:
                    type = GESTURE_UNRECOGNIZED;
                    break;
            }
        }
        return type;
    }
    
}