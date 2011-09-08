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

#ifdef OS_IOS

namespace sora {
    
    void setEAGLView(void* view);
    void setupEAGLFBO();
    void presentEAGLFBO();
    void* getEAGLView();
    void setupMultisampling(bool flag);
    
} // namespace sora

#endif // OS_IOS

#endif
