/*
 *  SoraOGLRenderTarget.h
 *  SoraPureCore
 *
 *  Created by griffin clare on 11/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORA_PSPGL_RENDER_TARGET_H_
#define SORA_PSPGL_RENDER_TARGET_H_

#include "SoraPlatform.h"
#include "SoraRenderTarget.h"

#ifdef OS_PSP
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

namespace sora {

class SoraRenderTargetPSPGL {
	GLuint frameBuffer;
	GLuint depthBuffer;

    int32 w, h;
	ulong32 tex;
	bool zbuffer;
	int32 err;
    
public:
	SoraRenderTargetPSPGL(int32 _w, int32 _h, bool _zbuffer);
	~SoraRenderTargetPSPGL();

	virtual void attachToRender();
	virtual void detachFromRender();
    
    int32 getWidth() const { return w; }
    int32 getHeight() const { return h; }
    
    ulong32 getTexture() const { return tex; }
};

} // namespace sora

#endif
