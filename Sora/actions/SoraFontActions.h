//
//  SoraFontModifiers.h
//  Sora
//
//  Created by Robert Bu on 7/15/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraFontModifiers_h
#define Sora_SoraFontModifiers_h

#include "SoraModifier.h"
#include "SoraAction.h"

namespace sora {

    class SoraFont;
    
    class SORA_API SoraFontRotateAction: public SoraFiniteAction {
    public:
        SoraFontRotateAction(float begin, float end, float inTime, bool mLineRotation=true, bool mRotateChar=false);
        virtual ~SoraFontRotateAction();
        
        virtual void startWithTarget(SoraObject* target);
        virtual bool isDone() const;
        virtual void step(float dt);
        
        virtual SoraAction::Ptr reverse() const;
        
        static SoraAction::Ptr ActionWithRotation(float begin, float end, float time);
        
    private:
        float mBegin;
        float mEnd;
        float mCurrVal;
        bool mLineRotation;
        bool mRotateChar;
        
        SoraFont* mFont;
    };
    
    class SORA_API SoraFontScaleAction: public SoraFiniteAction {
    public:
        SoraFontScaleAction(float begin, float end, float inTime);
        virtual ~SoraFontScaleAction();
        
        virtual void startWithTarget(SoraObject* target);
        virtual bool isDone() const;
        virtual void step(float dt);
        
        virtual SoraAction::Ptr reverse() const;
        
        static SoraAction::Ptr ActionWithScale(float begin, float end, float time);
        
    private:
        float mBegin;
        float mEnd;
        float mCurrVal;
        
        SoraFont* mFont;
    };
} // namespace sora



#endif
