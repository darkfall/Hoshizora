//
//  SoraiOSInitializer.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/7/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "SoraiOSInitializer.h"
#include "SoraiOSWrapper.h"

namespace sora {
    
    void SoraiOSInitializer::SoraiOSInit(bool isOGLES2API, bool multisampling) {
        if(isOGLES2API)
            SORA->registerRenderSystem(new SoraiOSGLRenderer_ES2);
        else
            SORA->registerRenderSystem(new SoraiOSGLRenderer);
        
        mVerticalSync = true;
        sora::setupMultisampling(multisampling);
    }
    
    void SoraiOSInitializer::SoraiOSStart(SoraWindowInfoBase* window, bool isOGLES2API) {
        SoraiOSInit(isOGLES2API);
        
        try {
            SORA->registerFontManager(new sora::SoraiOSFontManager);
            SORA->registerInput(new sora::SoraiOSInput);
            
            SORA->createWindow(window);
            SORA->setFPS(60);
        } catch(SoraException& e) {
            SORA->messageBox(e.what(), "Fatal error", MB_OK);
            SORA->shutDown();
        }
        SORA->start();
    }
    
    void SoraiOSInitializer::enableMultisampling(bool flag) {
        sora::setupMultisampling(flag);
    }
    
    void SoraiOSInitializer::setTimer(SoraTimer* timer) { 
        pTimer = timer;
    }
    
    void SoraiOSInitializer::setVerticalSync(bool flag) {
        mVerticalSync = flag;
    }
    
    bool SoraiOSInitializer::update() {
        if(mVerticalSync) {
            return true;
        }
        else if(pTimer)
            return pTimer->update();
        return false;
    }
    
    void SoraiOSInitializer::SoraiOSUpdateSystems() {
        SORA->update();
    }
    
    void SoraiOSInitializer::SoraiOSShutDown() {
        SORA->shutDown();
    }
    
} // namespace sora
