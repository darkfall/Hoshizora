//
//  MainWindow.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_MainWindow_h
#define Sora_iPhone_MainWindow_h

#include "SoraiOSGLRenderer/SoraiOSMainWindow.h"

#include "SoraFSM.h"
#include "SoraGameApp.h"

#include "SoraSprite.h"

namespace flower {
    
    class MainWindow: public sora::SoraiOSMainWindow {
    public:
        void init();
        
        bool updateFunc();
        bool renderFunc();
        
        void applicationWillResignActive();
        void applicationDidBecomeActive();
        
    private:
        sora::SoraSprite* mBg;
        sora::SoraFSMManager mFSManager;
    };
    
} // namespace flower



#endif
