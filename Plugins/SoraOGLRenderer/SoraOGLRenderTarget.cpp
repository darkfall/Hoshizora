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
#include "SoraCore.h"

#ifdef WIN32
#include <GL/GL.h>
#include <GL/GLU.h>
#endif


namespace sora {

SoraRenderTargetOG::SoraRenderTargetOG(int32 _w, int32 _h, bool _zbuffer):
    w(_w), h(_h), zbuffer(_zbuffer), err(0), frameBuffer(0), depthBuffer(0) {
	GLuint gl_error;

	glEnable(GL_TEXTURE_2D);

#ifndef WIN32
    GLuint glTex;
	glGenTextures(1, &glTex);					
	glBindTexture(GL_TEXTURE_2D, glTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _w, _h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	if((gl_error = glGetError()) != GL_NO_ERROR) {
		err = 1;
	}
	// create framebuffer
	glGenFramebuffersEXT(1, &frameBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);

	//create depth buffer
	if (zbuffer) {
		glGenRenderbuffersEXT(1, &depthBuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	}

	// attach texture
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, glTex, 0);
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if(status != GL_FRAMEBUFFER_COMPLETE_EXT) {
        err = 1;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	if(!glTex || !frameBuffer)
		err = 2;
	else err = 0;

	SoraTexture* ptex = new SoraTexture;
	ptex->mTextureID = glTex;
	ptex->mTextureWidth = ptex->mOriginalWidth = w;
	ptex->mTextureHeight = ptex->mOriginalHeight = h;
	tex = (ulong32)ptex;
        
    glDisable(GL_TEXTURE_2D);
#endif
}

SoraRenderTargetOG::~SoraRenderTargetOG() {
#ifndef WIN32
	if (tex) {
		glDeleteTextures(1, (GLuint*)(&((SoraTexture*)tex)->mTextureID));
        delete (SoraTexture*)tex;
    }

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
