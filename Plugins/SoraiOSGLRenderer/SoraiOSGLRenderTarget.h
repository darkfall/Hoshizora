/*
 *  SoraOGLRenderTarget.h
 *  SoraPureCore
 *
 *  Created by griffin clare on 11/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORA_IOSGL_RENDER_TARGET_H_
#define SORA_IOSGL_RENDER_TARGET_H_

#include "SoraPlatform.h"

#ifdef OS_IOS
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#endif

namespace sora {

    class SoraRenderTargetiOSGL {
        GLuint frameBuffer;
        GLuint depthBuffer;
        GLuint glTex;
        GLuint oldbuffer;
        
        int32 w, h;
        ulong32 tex;
        bool zbuffer;
        
    public:
        SoraRenderTargetiOSGL(int32 _w, int32 _h, bool _zbuffer);
        ~SoraRenderTargetiOSGL();
        
        virtual void attachToRender();
        virtual void detachFromRender();
        
        int32 getWidth() const { return w; }
        int32 getHeight() const { return h; }
        
        ulong32 getTexture() const { return tex; }
    };

} // namespace sora

#endif
