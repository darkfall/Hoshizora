//
//  SoraFontModifiers.h
//  Sora
//
//  Created by Robert Bu on 7/15/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraFontModifiers_h
#define Sora_SoraFontModifiers_h

#include "../SoraModifier.h"
#include "../SoraFont.h"

namespace sora {
    
    class SORA_API SoraFontRotationModifier: public SoraModifier<SoraFont> {
    public:
        SoraFontRotationModifier(float32 begin, float32 end, float32 inTime, bool mLineRotation=true, bool mRotateChar=false);
    
        int32   update(float32 dt);
        void    modify(SoraFont* font);
        void    reset();

        SoraModifier<SoraFont>* clone();
        
    private:
        float32 mBegin;
        float32 mEnd;
        float32 mCurrVal;
        float32 mInTime;
        float32 mCurrTime;
        bool mLineRotation;
        bool mRotateChar;
    };
    
    class SORA_API SoraFontScaleModifier: public SoraModifier<SoraFont> {
    public:
        SoraFontScaleModifier(float32 begin, float32 end, float32 inTime);
        
        int32   update(float32 dt);
        void    modify(SoraFont* font);
        void    reset();
        
        SoraModifier<SoraFont>* clone();
        
    private:
        float32 mBegin;
        float32 mEnd;
        float32 mCurrVal;
        float32 mInTime;
        float32 mCurrTime;
    };
} // namespace sora



#endif
