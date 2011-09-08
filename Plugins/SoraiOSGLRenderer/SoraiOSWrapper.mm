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

namespace sora {
    
    static EAGLView* g_view = 0;
    
    void setEAGLView(void* view) {
        g_view = (EAGLView*)view;
    }
    
    void setupEAGLFBO() {
        if(g_view)
            [g_view setFramebuffer];
    }
    
    void presentEAGLFBO() {
        if(g_view)
            [g_view presentFramebuffer];
    }
    
    void* getEAGLView() {
        return g_view;
    }
    
    void setupMultisampling(bool flag) {
        if(g_view)
            [g_view enableMultiSampling:flag];
    }
    
} // namespace sora

#endif // OS_IOS