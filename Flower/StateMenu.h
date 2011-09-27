//
//  StateMenu.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_StateMenu_h
#define Sora_iPhone_StateMenu_h

#include "app/SoraGameState.h"
#include "SoraSprite.h"
#include "SoraCore.h"

#include "timer/SoraSimpleTimer.h"

namespace flower {
    
    class StateMenu: public sora::SoraGameState {
    public:
        void onEnter();
        void onLeave();
        void onUpdate(float dt);
       
        void onRender();
        
    private:
        sora::SimpleTimerPtr mTimer;
        sora::SoraSprite* mBg;
    };
    
} // namespace flower

#endif
