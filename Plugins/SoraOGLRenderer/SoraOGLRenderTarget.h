/*
 *  SoraOGLRenderTarget.h
 *  SoraPureCore
 *
 *  Created by griffin clare on 11/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORA_OGL_RENDER_TARGET_H_
#define SORA_OGL_RENDER_TARGET_H_

#include "SoraRenderTarget.h"
#if defined(OS_LINUX)
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif
#include "glfw/GL/glfw.h"

namespace sora {

class SoraRenderTargetOG: public SoraRenderTarget {
	ulong32 frameBuffer;
	ulong32 depthBuffer;

public:
	SoraRenderTargetOG(ulong32 w, ulong32 h, bool zbuffer);
	~SoraRenderTargetOG();

	virtual void attachToRender();
	virtual void detachFromRender();
};

} // namespace sora

#endif
