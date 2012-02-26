//
//  SoraCameraActions.h
//  Sora
//
//  Created by Robert Bu on 7/19/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraCameraActions_h
#define Sora_SoraCameraActions_h

#include "SoraPlatform.h"
#include "SoraMath.h"
#include "SoraAction.h"

namespace sora {
    
    class Sora2DCamera;
    
    class SORA_API SoraCameraZoomAction: public SoraFiniteAction {
    public:
        SoraCameraZoomAction(float starth, float startv, float hz, float hv, float time);
        virtual ~SoraCameraZoomAction();
        
        virtual void startWithTarget(SoraObject* target);
        virtual bool isDone() const;
        virtual void step(float dt);
        
        virtual SoraAction::Ptr reverse() const;
        
        static SoraAction::Ptr ActionWithZoom(float sh, float sv, float hz, float hv, float time);
        
    private:
        SoraVector mCurrZoom;
        SoraVector mEndZoom;
        SoraVector mStartZoom;
        
        Sora2DCamera* mCamera;
    };
    
    class SORA_API SoraCameraRotationAction: public SoraFiniteAction {
    public:
        SoraCameraRotationAction(float start, float end, float inTime);
        virtual ~SoraCameraRotationAction();
        
        virtual void startWithTarget(SoraObject* target);
        virtual bool isDone() const;
        virtual void step(float dt);
        
        virtual SoraAction::Ptr reverse() const;
        
        static SoraAction::Ptr ActionWithRotation(float start, float end, float inTime);

    private:
        float mStart;
        float mEnd;
        float mCurrRot;
        
        Sora2DCamera* mCamera;
    };
    
} // namespace sora

#endif
