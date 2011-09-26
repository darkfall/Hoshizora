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

namespace ci {
    
    class MainWindow: public sora::SoraiOSMainWindow {
    public:
        void init();
        
        bool updateFunc();
        bool renderFunc();
        
        void applicationWillResignActive();
        void applicationDidBecomeActive();
        
    private:
        
    };
    
} // namespace ci



#endif
