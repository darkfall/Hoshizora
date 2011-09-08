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
#include "SoraiOSWrapper.h"
#include "SoraCore.h"
#include "EAGLView.h"

namespace sora {

SoraRenderTargetiOSGL::SoraRenderTargetiOSGL(int32 _w, int32 _h, bool _zbuffer):
    w(_w), h(_h), zbuffer(_zbuffer), frameBuffer(0), depthBuffer(0) {
        
        SoraTexture* objtex = (SoraTexture*)SoraCore::Instance()->createTextureWH(_w, _h);
        w = objtex->mTextureWidth;
        h = objtex->mTextureHeight;
        glTex = objtex->mTextureID;
        tex = (HSORATEXTURE)objtex;
			
		// create framebuffer
        glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, (GLint *) &oldbuffer);

		glGenFramebuffersOES(1, &frameBuffer);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, frameBuffer);
		
		glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, glTex, NULL);
        
		//create depth buffer
		glGenRenderbuffersOES(1, &depthBuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthBuffer);
        glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, w, h);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, 0);
        
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_COMPONENT16_OES, GL_RENDERBUFFER_OES, depthBuffer);
        
		GLenum status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
		if(status != GL_FRAMEBUFFER_COMPLETE_OES) {
            THROW_SORA_EXCEPTION(RuntimeException, "Erroring creating fbo object");
		}
        
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, oldbuffer);
}

SoraRenderTargetiOSGL::~SoraRenderTargetiOSGL() {
	if (tex) {
		glDeleteTextures(1, (GLuint*)(&((SoraTexture*)tex)->mTextureID));
        delete (SoraTexture*)tex;
    }

	if (frameBuffer)
		glDeleteFramebuffersOES(1, (uint32*)(&frameBuffer));

	if (depthBuffer)
		glDeleteRenderbuffersOES(1, (uint32*)(&depthBuffer));
}

void SoraRenderTargetiOSGL::attachToRender() {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, (GLint *) &oldbuffer);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, frameBuffer);
}

void SoraRenderTargetiOSGL::detachFromRender() {
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, oldbuffer);
}

} // namespace sora
