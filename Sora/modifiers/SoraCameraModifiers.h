//
//  SoraCameraModifiers.h
//  Sora
//
//  Created by Robert Bu on 7/19/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraCameraModifiers_h
#define Sora_SoraCameraModifiers_h

#include "SoraPlatform.h"
#include "SoraModifier.h"
#include "SoraMath.h"
#include "SoraCamera.h"

#include "SoraObjectModifiers.h"

namespace sora {
    
    class SORA_API SoraCameraZoomModifier: public SoraModifier<SoraCamera> {
    public:
        SoraCameraZoomModifier(float starth, float startv, float hz, float hv, float inTime);
        
        int32   update(float dt);
        void    modify(SoraCamera* camera);
        void    reset();
        
        SoraModifier<SoraCamera>* clone();
        
    private:
        float mTime;
        float mCurrTime;
        SoraVector mCurrZoom;
        SoraVector mEndZoom;
        SoraVector mStartZoom;
    };
    
    class SORA_API SoraCameraRotationModifier: public SoraModifier<SoraCamera> {
    public:
        SoraCameraRotationModifier(float start, float end, float inTime);
        
        int32   update(float dt);
        void    modify(SoraCamera* camera);
        void    reset();
        
        SoraModifier<SoraCamera>* clone();
        
    private:
        float mTime;
        float mCurrTime;
        float mStart;
        float mEnd;
        float mCurrRot;
    };
    
    typedef SoraTransitionModifier<SoraCamera> SoraCameraPositionModifier;
    
} // namespace sora

#endif
