//
//  SoraiOSWrapper.mm
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/7/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "SoraiOSWrapper.h"

#ifdef OS_IOS

#import "EAGLView.h"
#include "SoraiOSDeviceHelper.h"
#include "SoraiOSGLRenderer.h"

namespace sora {
    
    static EAGLView* g_view = 0;
    static int g_portrait = ORIENTATION_PORTRAIT;
    
    void setEAGLView(void* view) {
        g_view = (EAGLView*)view;
    }
    
    void setupEAGLFBO() {
        [g_view setFramebuffer];
    }
    
    void presentEAGLFBO() {
        [g_view presentFramebuffer];
    }
    
    void* getEAGLView() {
        return g_view;
    }
    
    void setupMultisampling(bool flag) {
        [g_view enableMultiSampling:flag];
    }
    
    int getScreenWidth() {
        if(g_portrait == ORIENTATION_LANDSCAPE_LEFT || g_portrait == ORIENTATION_LANDSCAPE_RIGHT) {
           return iOSGetScreenHeight(false);
        }
        return [g_view getScreenWidth];
    }
    
    int getScreenHeight() {
        if(g_portrait == ORIENTATION_LANDSCAPE_LEFT || g_portrait == ORIENTATION_LANDSCAPE_RIGHT) {
            return [g_view getScreenWidth];
        }
        return [g_view getScreenHeight];
    }
    
    float getContentsScale() {
        return [g_view getContentsScale];
    }
    
    void setPortrait(int flag) {
        g_portrait = flag;
    }
    
    
    void translatePointToView(float* x, float* y) {        
        float newX = iOSGetScreenWidth(false) - *x;
        float newY = iOSGetScreenHeight(false) - *y;
        switch(g_portrait) {
            case ORIENTATION_PORTRAIT_UPSIDE_DOWN:
                *x = newX;
                *y = newY;
                break;
            case ORIENTATION_LANDSCAPE_LEFT:
                *x = *y;
                *y = newX;
                break;
            case ORIENTATION_LANDSCAPE_RIGHT:
                *y = *x;
                *x = newY;
                break;
        }
    }
    
    void translatePointToGL(float* x, float *y) {
        float newX = iOSGetScreenWidth(false) - *x;
        float newY = iOSGetScreenHeight(false) - *y;
        switch(g_portrait) {
            case ORIENTATION_PORTRAIT_UPSIDE_DOWN:
                *x = newX;
                *y = newY;
                break;
            case ORIENTATION_LANDSCAPE_LEFT:
                *x = newX;
                //*x = newY;
                break;
            case ORIENTATION_LANDSCAPE_RIGHT:
                *y = newY;
                break;
        }
    }
} // namespace sora

#endif // OS_IOS