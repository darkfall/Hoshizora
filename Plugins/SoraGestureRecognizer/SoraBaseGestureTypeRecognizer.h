//
//  SoraBaseGestureTypeRecognizer.h
//  Sora
//
//  Created by Ruiwei Bu on 7/1/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraBaseGestureTypeRecognizer_h
#define Sora_SoraBaseGestureTypeRecognizer_h

#include "SoraGesture.h"

namespace sora {
        
    class SoraBaseGestureTypeRecognizer: public SoraGestureTypeRecognizer {
    public:
        SoraGestureType recognizeGesture(const SoraGesture::GestureSigPointList& sigPoints);
    };
    
}

#endif
