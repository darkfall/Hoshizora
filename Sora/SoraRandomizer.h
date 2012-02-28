//
//  SoraRandomizer.h
//  SoraF
//
//  Created by Ruiwei Bu on 2/27/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef SoraF_SoraRandomizer_h
#define SoraF_SoraRandomizer_h

#include "SoraPlatform.h"
#include "SoraVector.h"
#include "SoraVector3.h"
#include "SoraColor.h"

namespace sora {
    
    class SoraRandomizer {
    public:
        
        static int32 RandomInt(int32 s, int32 e);
        static float RandomFloat(float s, float e);
        
        static SoraVector       RandomVector(const SoraVector& s, const SoraVector& e);
        static SoraVector3      RandomVector(const SoraVector3& s, const SoraVector3& e);
        static SoraColorRGBA    RandomColor(const SoraColorRGBA& s, const SoraColorRGBA& e);
    };
    
}

#endif
