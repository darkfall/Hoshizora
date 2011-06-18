/*
 *  SoraOGLRenderTarget.cpp
 *  SoraPureCore
 *
 *  Created by griffin clare on 11/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "SoraiOSGLRenderTarget.h"
#include "SoraTexture.h"
#include "SoraCore.h"

namespace sora {

SoraRenderTargetiOSGL::SoraRenderTargetiOSGL(int32 _w, int32 _h, bool _zbuffer):
    w(_w), h(_h), zbuffer(_zbuffer), err(0), frameBuffer(0), depthBuffer(0) {
	GLuint gl_error;

	glEnable(GL_TEXTURE_2D);

#ifndef WIN32
    GLuint glTex;
	glGenTextures(1, &glTex);					
	glBindTexture(GL_TEXTURE_2D, glTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _w, _h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	if((gl_error = glGetError()) != GL_NO_ERROR) {
		err = 1;
	}
	// create framebuffer
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//create depth buffer
	if (zbuffer) {
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	// attach texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glTex, 0);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        err = 1;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if(!glTex || !frameBuffer)
		err = 2;
	err = 0;

	SoraTexture* ptex = new SoraTexture;
	ptex->mTextureID = glTex;
	ptex->mTextureWidth = ptex->mOriginalWidth = w;
	ptex->mTextureHeight = ptex->mOriginalHeight = h;
	tex = (ulong32)ptex;
        
    glDisable(GL_TEXTURE_2D);
#endif
}

SoraRenderTargetiOSGL::~SoraRenderTargetiOSGL() {
#ifndef WIN32
	if (tex) {
		glDeleteTextures(1, (GLuint*)(&((SoraTexture*)tex)->mTextureID));
        delete (SoraTexture*)tex;
    }

	if (frameBuffer)
		glDeleteFramebuffers(1, (uint32*)(&frameBuffer));

	if (depthBuffer)
		glDeleteRenderbuffers(1, (uint32*)(&depthBuffer));
#endif
}

void SoraRenderTargetiOSGL::attachToRender() {
#ifndef WIN32
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
#endif
}

void SoraRenderTargetiOSGL::detachFromRender() {
#ifndef WIN32
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
}

} // namespace sora
