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
#include "SoraiOSMainWindow.h"

#import "AccelerometerController.h"
#import "AppDelegate.h"
#import "GyroScopeController.h"

#include "SoraiOSFontManager/SoraiOSFontManager.h"
#include "SoraiOSSoundSystem/SoraiOSSoundSystem.h"


namespace sora {
    
    static SoraiOSAppDelegate* g_app_delegate = 0;

    void SoraiOSInitializer::SoraiOSInit(bool isOGLES2API, bool multisampling) {
#ifndef SORA_USE_GLES2
        mRenderSystem = new SoraiOSGLRenderer;
        SoraCore::Instance()->registerRenderSystem(mRenderSystem);
#else
        mRenderSystem = new SoraiOSGLES2Renderer;
        SoraCore::Instance()->registerRenderSystem(mRenderSystem);
#endif
        
        sora::setupMultisampling(multisampling);
        g_app_delegate = [[SoraiOSAppDelegate alloc] init];
            
        mDeviceUpdateInterval = 1.0f/30.f;
    }
    
    void SoraiOSInitializer::SoraiOSStart(SoraiOSMainWindow* window, const SoraOALParameters& soundParams, bool multisampling) {
        SoraiOSInit(false, multisampling);
        
        try {
            SoraCore::Ptr->registerFontManager(new sora::SoraiOSFontManager);
            SoraCore::Ptr->registerSoundSystem(new sora::SoraiOSSoundSystem(soundParams));
            SoraCore::Ptr->registerInput(new sora::SoraiOSInput);
            SoraCore::Ptr->registerTimer(new sora::SoraiOSTimer);
            
        } catch(SoraException& e) {
            SoraCore::Ptr->messageBox(e.what(), "Fatal error", MB_OK);
            SoraCore::Ptr->shutDown();
        }
                
        mMainWindow = window;
        SoraCore::Ptr->createWindow(window);
        SoraCore::Ptr->start();
    }
    
    SoraiOSMainWindow* SoraiOSInitializer::getMainWindow() const {
        return mMainWindow;
    }
    
    SoraiOSInitializer::~SoraiOSInitializer() {
        if(g_app_delegate)
            [g_app_delegate release];
        enableGyroscope(false);
        enableAccelerometer(false);
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
            SoraiOSUpdateSystems();
        }
        return false;
    }
    
    void SoraiOSInitializer::SoraiOSUpdateSystems() {
        SoraCore::Ptr->update();
    }
    
    void SoraiOSInitializer::SoraiOSShutDown() {
        SoraCore::Ptr->shutDown();
    }
    
    void SoraiOSInitializer::setDeviceUpdateInterval(float interval) {
        mDeviceUpdateInterval = interval;
        
        sora_assert(mDeviceUpdateInterval >= 0.f && mDeviceUpdateInterval <= 1.0f);
    }
    
    void SoraiOSInitializer::setOrientation(iOSOrientation orientation) {
        iOSOrientation prev = getOrientation();
        
        mRenderSystem->setOrientation(orientation);
        
        switch(orientation) {
            case ORIENTATION_PORTRAIT:
                sora::setOrientation(orientation);
				[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationPortrait animated:YES];
				break;
			case ORIENTATION_PORTRAIT_UPSIDE_DOWN:
                sora::setOrientation(orientation);
				[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationPortraitUpsideDown animated:YES];
				break;
			case ORIENTATION_LANDSCAPE_LEFT:
                sora::setOrientation(orientation);
				[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationLandscapeRight animated:YES];
				break;
			case ORIENTATION_LANDSCAPE_RIGHT:
                sora::setOrientation(orientation);
				[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationLandscapeLeft animated:YES];
				break;
        }
        
        mMainWindow->didChangeStatusBarOrientation(orientation, prev);
    }
    
    static SoraiOSAccelerometerController* g_accl_controler = 0;
    static SoraiOSGyroScopeController* g_gyro_controller = 0;
    
    iOSOrientation SoraiOSInitializer::getOrientation() const {
        return mRenderSystem->getOrientation();
    }
    
    void SoraiOSInitializer::enableAccelerometer(bool flag) {
        if(flag) {
            g_accl_controler = [SoraiOSAccelerometerController alloc];
            [g_accl_controler setInterval:mDeviceUpdateInterval];
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
    
    void SoraiOSInitializer::enableGyroscope(bool flag) {
        if(flag) {
            g_gyro_controller = [SoraiOSGyroScopeController alloc];
            [g_gyro_controller setInterval:mDeviceUpdateInterval];
            [g_gyro_controller prepare];
            
        } else {
            if(g_gyro_controller) {
                [g_gyro_controller stop];
                [g_gyro_controller release];
                g_gyro_controller = 0;
            }
        }
    }
    
    void SoraiOSInitializer::enableOrientationChange(bool flag) {
        sora::enableOrientation(flag);
    }
    
    bool SoraiOSInitializer::isOrientationChangeEnabled() const {
        return sora::isOrientationEnabled();
    }
    
    void SoraiOSInitializer::getGyroscopeAttr(float* x, float* y, float* z) {
        if(g_gyro_controller) {
            *x = g_gyro_controller.x;
            *y = g_gyro_controller.y;
            *z = g_gyro_controller.z;
        } else {
            *x = 0.f;
            *y = 0.f;
            *z = 0.f;
        }
    }


} // namespace sora
