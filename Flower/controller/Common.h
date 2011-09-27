//
//  Common.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_CI_Common_h
#define Sora_CI_Common_h

#include "SoraPlatform.h"
#include "SoraCore.h"

namespace flower {
    
    
    inline bool PositionCheck(float x, float y, float w, float h) {
        if(x + w/2 < 0.f ||
           x - w/2 > sora::SoraCore::Instance()->getScreenWidth())
            return false;
        
        if(y + h/2 < 0.f ||
           y - h/2 > sora::SoraCore::Instance()->getScreenHeight())
            return false;
        
        return true;
    }
    
    inline bool enclose(float& v, float threshold) {
        if(v < -threshold) {
            v = -threshold;
            return true;
        }
        else if(v > threshold) {
            v = threshold;
            return true;
        }
        return false;
    }
    
    inline bool enclose(float& v, float min, float max) {
        if(v < min) {
            v = min;
            return true;
        }
        else if(v > max) {
            v = max;
            return true;
        }
        return false;
    }
}


#endif
