//
//  SoraiOSInitializer.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/7/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "SoraiOSInitializer.h"
#include "SoraiOSWrapper.h"
#include "SoraiOSGLRenderer.h"
#include "SoraiOSTimer.h"

#import "AccelerometerController.h"

namespace sora {
    
    void SoraiOSInitializer::SoraiOSInit(bool isOGLES2API, bool multisampling) {
       // if(isOGLES2API)
       //     SORA->registerRenderSystem(new SoraiOSGLRenderer_ES2);
       // else
            SORA->registerRenderSystem(new SoraiOSGLRenderer);
        
        sora::setupMultisampling(multisampling);
    }
    
    void SoraiOSInitializer::SoraiOSStart(SoraWindowInfoBase* window, bool isOGLES2API) {
        SoraiOSInit(isOGLES2API);
        
        try {
            SORA->registerFontManager(new sora::SoraiOSFontManager);
            SORA->registerInput(new sora::SoraiOSInput);
            SORA->registerTimer(new sora::SoraiOSTimer);
            
            SORA->createWindow(window);
            SORA->setFPS(120);
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
    
    bool SoraiOSInitializer::update() {
        if(pTimer->update()) {
            sora::SoraTimestamp time;
            sora::SORA_IOS->SoraiOSUpdateSystems();
        //    printf("%u\n", time.elapsed());

        }
        return false;
    }
    
    void SoraiOSInitializer::SoraiOSUpdateSystems() {
        SORA->update();
    }
    
    void SoraiOSInitializer::SoraiOSShutDown() {
        SORA->shutDown();
    }
    
    void SoraiOSInitializer::setRenderSystem(SoraiOSGLRenderer* r) {
        mRenderSystem = r;
    }
    
    void SoraiOSInitializer::setOrientation(int portrait) {
        mRenderSystem->setOrientation(portrait);
        
        switch(portrait) {
            case ORIENTATION_PORTRAIT:
                sora::setPortrait(portrait);
				[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationPortrait animated:YES];
				break;
			case ORIENTATION_PORTRAIT_UPSIDE_DOWN:
                sora::setPortrait(portrait);
				[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationPortraitUpsideDown animated:YES];
				break;
			case ORIENTATION_LANDSCAPE_LEFT:
                sora::setPortrait(portrait);
				[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationLandscapeRight animated:YES];
				break;
			case ORIENTATION_LANDSCAPE_RIGHT:
                sora::setPortrait(portrait);
				[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationLandscapeLeft animated:YES];
				break;
        }
    }
    
    static AccelerometerController* g_accl_controler = 0;
    
    int SoraiOSInitializer::getOrientation() const {
        return mRenderSystem->getOrientation();
    }
    
    void SoraiOSInitializer::enableAccelerometer(bool flag) {
        if(flag) {
            g_accl_controler = [AccelerometerController alloc];
            [g_accl_controler prepare];
        } else {
            if(g_accl_controler) {
                [g_accl_controler stop];
                [g_accl_controler release];
                g_accl_controler = 0;
            }
        }
    }
    
    void SoraiOSInitializer::getAccelerometerAttr(float* x, float* y, float* z) {
        if(g_accl_controler) {
            *x = g_accl_controler.x;
            *y = g_accl_controler.y;
            *z = g_accl_controler.z;
        } else {
            *x = 0.f;
            *y = 0.f;
            *z = 0.f;
        }
    }
    

} // namespace sora
