//
//  SoraCameraModifiers.h
//  Sora
//
//  Created by Robert Bu on 7/19/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraCameraModifiers_h
#define Sora_SoraCameraModifiers_h

#include "../SoraPlatform.h"
#include "../SoraModifier.h"
#include "../SoraMath.h"
#include "../SoraCamera.h"

#include "SoraObjectModifiers.h"

namespace sora {
    
    class SORA_API SoraCameraZoomModifier: public SoraModifier<SoraCamera> {
    public:
        SoraCameraZoomModifier(float32 starth, float32 startv, float32 hz, float32 hv, float32 inTime);
        
        int32   update(float32 dt);
        void    modify(SoraCamera* camera);
        void    reset();
        
        SoraModifier<SoraCamera>* clone();
        
    private:
        float32 mTime;
        float32 mCurrTime;
        SoraVector mCurrZoom;
        SoraVector mEndZoom;
        SoraVector mStartZoom;
    };
    
    class SORA_API SoraCameraRotationModifier: public SoraModifier<SoraCamera> {
    public:
        SoraCameraRotationModifier(float32 start, float32 end, float32 inTime);
        
        int32   update(float32 dt);
        void    modify(SoraCamera* camera);
        void    reset();
        
        SoraModifier<SoraCamera>* clone();
        
    private:
        float32 mTime;
        float32 mCurrTime;
        float32 mStart;
        float32 mEnd;
        float32 mCurrRot;
    };
    
    typedef SoraTransitionModifier<SoraCamera> SoraCameraPositionModifier;
    
} // namespace sora

#endif
