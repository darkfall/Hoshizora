//
//  SoraObjectActions.h
//  Sora
//
//  Created by Robert Bu on 7/15/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraObjectActions_h
#define Sora_SoraObjectActions_h

#include "SoraAction.h"
#include "SoraMath.h"

namespace sora {
    
    class SORA_API SoraMoveAction: public SoraFiniteAction {
    public:
        SoraMoveAction(float startx, float starty, float endx, float endy, float inTime);
        SoraMoveAction(const SoraVector& start, const SoraVector& end, float inTime);
        
        virtual ~SoraMoveAction();
        
        virtual void startWithTarget(SoraObject* target);
        virtual void step(float dt);
        
        virtual SoraAction::Ptr reverse() const;
        
        static SoraAction::Ptr ActionWithBeginAndEnd(const SoraVector& begin,
                                                     const SoraVector& end,
                                                     float time);
        
    private:
        SoraVector mBegin;
        SoraVector mEnd;
        SoraVector mCurrValue;
    };
    
    class SORA_API SoraMoveToAction: public SoraFiniteAction {
    public:
        SoraMoveToAction(float endx, float endy, float inTime);
        SoraMoveToAction(const SoraVector& end, float inTime);
        
        virtual ~SoraMoveToAction();
        
        virtual void startWithTarget(SoraObject* target);
        virtual void step(float dt);
        
        virtual SoraAction::Ptr reverse() const;
        
        static SoraAction::Ptr ActionWithDest(const SoraVector& dest,
                                              float time);
        
    private:
        SoraVector mBegin;
        SoraVector mEnd;
        SoraVector mCurrValue;
    };
    
    class SORA_API SoraMoveByAction: public SoraFiniteAction {
    public:
        SoraMoveByAction(float x, float y, float inTime);
        SoraMoveByAction(const SoraVector& dt, float inTime);
        
        virtual ~SoraMoveByAction();
        
        virtual void startWithTarget(SoraObject* target);
        virtual void step(float dt);
        
        virtual SoraAction::Ptr reverse() const;
        
        static SoraAction::Ptr ActionWithDelta(const SoraVector& vec,
                                               float time);
        
    private:
        SoraVector mBegin;
        SoraVector mDt;
        SoraVector mCurrValue;
    };
    
    
} // namespace sora



#endif
