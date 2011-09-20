//
//  SoraiOSWrapper.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/7/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_SoraiOSWrapper_h
#define Sora_iPhone_SoraiOSWrapper_h

#include "SoraPlatform.h"
#include "SoraiOSDeviceHelper.h"

#ifdef OS_IOS

namespace sora {
    
    void setEAGLView(void* view);
    void setupEAGLFBO();
    void presentEAGLFBO();
    void* getEAGLView();
    void setupMultisampling(bool flag);
    
    int getScreenWidth();
    int getScreenHeight();
    int getViewWidth();
    int getViewHeight();
    
    float getContentsScale();
    void setOrientation(iOSOrientation p);
    
    // tranlate a point into corresponding orientation view
    void translatePointToView(float* x, float *y);
    // tranlsate a point from view into gl point 
    void translatePointToGL(float* x, float* y);
    
} // namespace sora

#endif // OS_IOS

#endif
