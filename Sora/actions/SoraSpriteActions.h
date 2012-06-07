//
//  SoraSpriteActions.h
//  SoraF
//
//  Created by Robert Bu on 2/25/12.
//  Copyright (c) 2012 Game Master Studio. All rights reserved.
//

#ifndef SoraF_SoraSpriteActions_h
#define SoraF_SoraSpriteActions_h

#include "SoraAction.h"

namespace sora {
    
    class SoraSprite;
    
    enum SoraSpriteActionDestType {
        SSADT_To,
        SSADT_By,
    };
    
    enum SoraSpriteActionType {
        SSAT_Rotate,
        SSAT_Scale,
        SSAT_Fade,
    };
    
    class SoraSpriteAction: public SoraFiniteAction {
    public:
        SoraSpriteAction(float to, float time, SoraSpriteActionType type, SoraSpriteActionDestType dest_type);
        virtual ~SoraSpriteAction();
      
        virtual void startWithTarget(SoraObject* target);
        virtual void step(float dt);
        virtual bool isDone() const;
        
        virtual SoraAction::Ptr reverse() const;
        
    private:
        float mValue;
        float mCurrent;
        float mBegin;
        
        SoraSpriteActionType mActionType;
        SoraSpriteActionDestType mDestType;
        
        SoraSprite* mSprite;
    };
    
}

#endif
