//
//  SoraRandomizer.cpp
//  SoraF
//
//  Created by Ruiwei Bu on 2/27/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "SoraRandomizer.h"
#include "SoraCore.h"

namespace sora {
    
    int32 SoraRandomizer::RandomInt(int32 s, int32 e) {
        return SoraCore::RandomInt(s, e);
    }
    
    float SoraRandomizer::RandomFloat(float s, float e) {
        return SoraCore::RandomFloat(s, e);
    }
    
    SoraVector SoraRandomizer::RandomVector(const SoraVector& s, const SoraVector& e) {
        return SoraVector(RandomFloat(s.x, e.x),
                          RandomFloat(s.y, e.y));
    }
    
    SoraVector3 SoraRandomizer::RandomVector(const SoraVector3& s, const SoraVector3& e) {
        return SoraVector3(RandomFloat(s.x, e.x),
                           RandomFloat(s.y, e.y),
                           RandomFloat(s.z, e.z));
    }
    
    SoraColorRGBA SoraRandomizer::RandomColor(const SoraColorRGBA& s, const SoraColorRGBA& e) {
        return SoraColorRGBA(RandomFloat(s.r, e.r),
                             RandomFloat(s.g, e.g),
                             RandomFloat(s.b, e.b),
                             RandomFloat(s.a, e.a));
    }
    
}
