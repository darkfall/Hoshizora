/*
 *  Sora_iPhoneDelegate.h
 *  Sora-iPhone
 *
 *  Created by GriffinBu on 1/28/11.
 *  Copyright 2011 Project Sora. All rights reserved.
 *
 */

#ifndef SORA_IPHONE_DELEGATE_H_
#define SORA_IPHONE_DELEGATE_H_

#include "SoraCore.h"
#include "Debug/SoraInternalLogger.h"

#include "SoraiOSGLRenderer.h"
#include "SoraiOSGLES2Renderer.h"

#include "SoraiOSInput.h"

#include "SoraiOSSoundSystem/SoraOALParam.h"

#define SORA_USE_GLES2


namespace sora {
    
    class SoraiOSMainWindow;
	
	class SoraiOSInitializer: public SoraSingleton<SoraiOSInitializer> {
		friend class SoraSingleton<SoraiOSInitializer>;
		
        void SoraiOSInit(bool isOGLES2API, bool multisampling=false);
		
        SoraiOSInitializer() {}
        ~SoraiOSInitializer();
        
	public:
		
        void SoraiOSStart(SoraiOSMainWindow* window, const SoraOALParameters& soundParams=SoraOALParameters(), bool multisampling=false);
		void setTimer(SoraTimer* timer);
        
        bool update();
        void SoraiOSUpdateSystems();
        void SoraiOSShutDown();
        void enableMultisampling(bool flag);
        
        // affect gyroscope and accelemeter update
        // and must be set before you enable the acceleromter or gyroscope
        // otherwise a restart is needed
        void setDeviceUpdateInterval(float interval);
        
        void setOrientation(iOSOrientation orientation);
        iOSOrientation getOrientation() const;
        
        void enableAccelerometer(bool flag);
        void getAccelerometerAttr(float* x, float* y, float* z);
        
        void enableGyroscope(bool flag);
        void getGyroscopeAttr(float* x, float* y, float *z);
        
        void enableOrientationChange(bool flag);
        bool isOrientationChangeEnabled() const;
        
        SoraiOSMainWindow* getMainWindow() const;
        
	private:
        SoraTimer* pTimer;
		SoraiOSInput* input;
    
        SoraiOSMainWindow* mMainWindow;
#ifndef SORA_USE_GLES2
        SoraiOSGLRenderer* mRenderSystem;
#else
        SoraiOSGLES2Renderer* mRenderSystem;
#endif
        
        float mDeviceUpdateInterval;
	};
	
} // namespace sora

#endif // SORA_IPHONE_DELEGATE_H_