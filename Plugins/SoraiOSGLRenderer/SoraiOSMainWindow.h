/*
 *  SoraiOSMainWindow.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */
#ifndef SORA_IOS_MAIN_WINDOW_H_
#define SORA_IOS_MAIN_WINDOW_H_

#include "SoraCore.h"
#include "SoraiOSDeviceHelper.h"

#include "SoraZipResourceManager/SoraZipResourceManager.h"
#include "SoraiOSFontManager/SoraiOSFontManager.h"
#include "SoraiOSWrapper.h"
#include "SoraFont.h"
#include "SoraFileUtility.h"

namespace sora {
    
    /**
     * Inheritate your window from this window
     * Only updateFunc, renderFunc, init is supported in inheritance here
     * Cause iOS won't let you control window size and position or title etc
     **/
    
    class SoraiOSMainWindow: public sora::SoraWindowInfoBase {
    public:
        SoraiOSMainWindow() { }
        
        int32 getWindowWidth() { 
            return sora::getScreenWidth();
        }
        int32 getWindowHeight() { 
            return sora::getScreenHeight();
        }
        
        int32 getViewWidth() {
            return sora::getViewWidth();
        }
        int32 getViewHeight() {
            return sora::getViewHeight();
        }
        
        int32 getWindowPosX() { return 0; }
        int32 getWindowPosY() { return 0; }
        
        SoraString getWindowName() { return "Hoshizora"; }
        SoraString getWindowId() { return "MainWindow"; }
        
        bool isWindowed() { return false; }
        bool hideMouse() { return false; }
        
        virtual bool updateFunc() {
            return false;
        }
        
        virtual bool renderFunc() {
            return false;
        }
        
        virtual void init() { }
        
        // ios delegates
        virtual void applicationWillResignActive() {}
        virtual void applicationDidBecomeActive() {}
        
        virtual void didChangeStatusBarOrientation(iOSOrientation newOrientation, iOSOrientation oldOrientation) {
        
        }
        
        virtual void didReceiveMemoryWarning() {}
        
    private:
    };
    

    
} // namespace sora

#endif
