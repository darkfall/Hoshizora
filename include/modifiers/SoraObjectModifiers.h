//
//  SoraObjectModifiers.h
//  Sora
//
//  Created by Ruiwei Bu on 7/15/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraObjectModifiers_h
#define Sora_SoraObjectModifiers_h

#include "SoraObject.h"
#include "SoraModifier.h"
#include "SoraMath.h"

namespace sora {
    
    class SoraObjectTransitionModifier: public SoraModifier<SoraObject> {
    public:
        SoraObjectTransitionModifier(float32 startx, float32 starty, float32 endx, float32 endy, float32 inTime);
        SoraObjectTransitionModifier(const SoraVector& start, const SoraVector& end, float32 inTime);
        
        int32   update(float32 dt);
        void    modify(SoraObject* obj);
        void    reset();
        
        SoraModifier<SoraObject>* clone();
        
    private:
        SoraVector mBegin;
        SoraVector mEnd;
        SoraVector mCurrValue;
        float32 mInTime;
        float32 mCurrTime;
    };
    
    
    
} // namespace sora



#endif
