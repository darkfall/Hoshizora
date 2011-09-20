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
    static iOSOrientation g_orientation = ORIENTATION_PORTRAIT;
    
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
        if(g_orientation == ORIENTATION_LANDSCAPE_LEFT || g_orientation == ORIENTATION_LANDSCAPE_RIGHT) {
           return [g_view getScreenHeight];
        }
        return [g_view getScreenWidth];
    }
    
    int getScreenHeight() {
        if(g_orientation == ORIENTATION_LANDSCAPE_LEFT || g_orientation == ORIENTATION_LANDSCAPE_RIGHT) {
            return [g_view getScreenWidth];
        }
        return [g_view getScreenHeight];
    }
    
    int getViewWidth() {
        if(g_orientation == ORIENTATION_LANDSCAPE_LEFT || g_orientation == ORIENTATION_LANDSCAPE_RIGHT) {
            return [g_view getViewHeight];
        }
        return [g_view getViewWidth];
    }
    
    int getViewHeight() {
        if(g_orientation == ORIENTATION_LANDSCAPE_LEFT || g_orientation == ORIENTATION_LANDSCAPE_RIGHT) {
            return [g_view getViewWidth];
        }
        return [g_view getViewHeight];
    }
    
    float getContentsScale() {
        return [g_view getContentsScale];
    }
    
    void setOrientation(iOSOrientation flag) {
        g_orientation = flag;
    }
    
    void translatePointToView(float* x, float* y) {        
        float newX = iOSGetScreenWidth(false) - *x;
        float newY = iOSGetScreenHeight(false) - *y;
        switch(g_orientation) {
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
        switch(g_orientation) {
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