/*
 *  SoraOGLRenderTarget.cpp
 *  SoraPureCore
 *
 *  Created by griffin clare on 11/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "SoraOGLRenderTarget.h"
#include "SoraTexture.h"

#ifdef WIN32
#include <GL/GL.h>
#include <GL/GLU.h>
#endif

namespace sora {

SoraRenderTargetOG::SoraRenderTargetOG(ulong32 _w, ulong32 _h, bool _zbuffer):
SoraRenderTarget(_w, _h, _zbuffer) {
	long32 gl_error;
	
	frameBuffer = depthBuffer = 0;
	
	glEnable(GL_TEXTURE_2D);
	
#ifndef WIN32
	glGenTextures(1, (unsigned int*)(&tex));					// Create 1 Texture
	glBindTexture(GL_TEXTURE_2D, tex);							// Bind The Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);							// Bind The Texture
	
	if((gl_error = glGetError()) != GL_NO_ERROR) {
		err = 1;
	}
	
	glDisable(GL_TEXTURE_2D);
	
	// create framebuffer
	glGenFramebuffersEXT(1, (unsigned int*)(&frameBuffer));
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
	
	//create render buffer
	if (zbuffer) {
		glGenRenderbuffersEXT(1, (unsigned int*)(&depthBuffer));
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	}
	
	// attach texture
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);
	// Attach to the FBO for depth
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer); 
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	if(!tex || !frameBuffer)
		err = 2;
	err = 0;
	
	SoraTexture* ptex = new SoraTexture;
	ptex->mTextureID = tex;
	ptex->mTextureWidth = ptex->mOriginalWidth = w;
	ptex->mTextureHeight = ptex->mOriginalHeight = h;
	stex = (ulong32)ptex;
#endif
}

SoraRenderTargetOG::~SoraRenderTargetOG() {
#ifndef WIN32
	if (tex)
		glDeleteTextures(1, (uint32*)(&tex));
	
	if (frameBuffer)
		glDeleteFramebuffersEXT(1, (uint32*)(&frameBuffer));
	
	if (depthBuffer)
		glDeleteRenderbuffersEXT(1, (uint32*)(&depthBuffer));
#endif
}

void SoraRenderTargetOG::attachToRender() {
#ifndef WIN32
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
#endif
}

void SoraRenderTargetOG::detachFromRender() {
#ifndef WIN32
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#endif
}

} // namespace sora