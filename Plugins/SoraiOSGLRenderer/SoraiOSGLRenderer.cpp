/*
 *  SoraiOSGLRenderer.cpp
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/23/10.
 *  Copyright 2010 Griffin Bu. All rights reserved.
 *
 */

#include "SoraiOSGLRenderer.h"

#include "SoraStringConv.h"
#include "SoraInternalLogger.h"
#include "SoraFileUtility.h"
#include "SoraMath.h"
#include "SoraColor.h"
#include "SoraiOSGLRenderTarget.h"

#include "SoraCore.h"
#include "SoraInfiniteRendererCallback.h"

#include "SoraGLSLShader.h"
#include "SoraiOSWrapper.h"
#include "SoraiOSInitializer.h"
#include "SoraiOSDeviceHelper.h"
#include "SoraSprite.h"
#include "SoraEnvValues.h"

#include "Convert.h"

#include "SOIL/SOIL.h"

const uint32 DEFAULT_VERTEX_BUFFER_SIZE = 128;

#include "util/SoraArray.h"

namespace sora{
    
    static SoraArray<SoraVertex> VertexBuffer;

	SoraiOSGLRenderer::SoraiOSGLRenderer() {
		pCurTarget = 0;
		
		mCurrTexture = -1;
		
		currShader = 0;
		iFrameStart = 1;
		CurBlendMode = 0;
	}

	SoraiOSGLRenderer::~SoraiOSGLRenderer() {
	//	shutdown();
	}

	void SoraiOSGLRenderer::shutdown() {
		std::list<SoraRenderTargetiOSGL*>::iterator itt = liTargets.begin();
		while(itt != liTargets.end()) {
			delete (*itt);
			++itt;
		}
		liTargets.clear();
		
		//delete mainWindow;
	}

	void SoraiOSGLRenderer::_glInitialize() {		
		glShadeModel(GL_SMOOTH);                    // Enables Smooth Shading
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment
        
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearStencil(0);                          // clear stencil buffer
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);                       // The Type Of Depth Test To Do
	//	glClearDepth(1.f);                          // Depth Buffer Setup
        
        //glEnable(GL_SCISSOR_TEST);
        //glDepthMask(GL_FALSE);
        
        //     glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        //     glEnable(GL_COLOR_MATERIAL);
        
        //    InitPerspective(60, (float)_oglWindowInfo.width / _oglWindowInfo.height, 0.f, 1.f);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations
        
		glDisable(GL_DITHER);
        glDisable(GL_FOG);
        
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
        
        glEnable(GL_SMOOTH);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POINT_SMOOTH);
        
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        
		SoraString info("OpenGL Version=");
		info += (char*)glGetString(GL_VERSION);
		SET_ENV_STRING("RENDERER_DRIVER", info);
    }
	
	void SoraiOSGLRenderer::_glBeginFrame() {
	}
	
	void SoraiOSGLRenderer::_glEndFrame() {
		glFlush();
	}
	
	void SoraiOSGLRenderer::beginFrame() {
		applyTransform();
		iFrameStart = 1;
	}
	
	void SoraiOSGLRenderer::endFrame() {
		_glEndFrame();
	}
	
	void SoraiOSGLRenderer::_glSetProjectionMatrix(int32 w, int32 h) {
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrthof(0.f, 320, 480, 0.f, -1.f, 1.f);
	}
	
	void SoraiOSGLRenderer::applyTransform() {
        if(!pCurTarget) {
            float w = _oglWindowInfo.width * getContentsScale();
            float h = _oglWindowInfo.height * getContentsScale();
            
            glViewport(0, 0,
                       w, h);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrthof(0.f,
                     w, h
                     , 0.f, -1.f, 1.f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
            glTranslatef(-_oglWindowInfo.x, -_oglWindowInfo.y, 0.f);
            switch(mOrientation) {
                case ORIENTATION_LANDSCAPE_LEFT:
                    glRotatef(90+_oglWindowInfo.rot,0,0,1);
                    glTranslatef(0, -w, 0);
                    break;
                case ORIENTATION_LANDSCAPE_RIGHT:
                    glRotatef(-90+_oglWindowInfo.rot,0,0,1);
                    glTranslatef(-h, 0 ,0);
                    break;
                case ORIENTATION_PORTRAIT_UPSIDE_DOWN:
                    glTranslatef(w/2,h/2,0);
                    glRotatef(180+_oglWindowInfo.rot,0,0,1);
                    glTranslatef(-w/2,-h/2,0);
                    break;
                case ORIENTATION_PORTRAIT:
                    glRotatef(_oglWindowInfo.rot, 0.f, 0.f, 1.f);
                    break;
            }
            glScalef(_oglWindowInfo.hscale, _oglWindowInfo.vscale, 1.0f);//Transformation follows order scale->rotation->displacement
            glTranslatef(_oglWindowInfo.x+_oglWindowInfo.dx, _oglWindowInfo.y+_oglWindowInfo.dy, 0.f); //Set Center Coodinates
 
            
        } else {
            glViewport(0,  0,
                       pCurTarget->getWidth(),
                       pCurTarget->getHeight());
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrthof(0,
                    pCurTarget->getWidth(),
                    0
                    , pCurTarget->getHeight(), -1.f, 1.f);
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(-_oglWindowInfo.x, -_oglWindowInfo.y, 0.f);
            glRotatef(_oglWindowInfo.rot, -0.f, 0.f, 1.f);
            glScalef(_oglWindowInfo.hscale, _oglWindowInfo.vscale, 1.0f);//Transformation follows order scale->rotation->displacement
            glTranslatef(_oglWindowInfo.x+_oglWindowInfo.dx, _oglWindowInfo.y+_oglWindowInfo.dy, 0.f); //Set Center Coodinates
        }
	}
	
	void SoraiOSGLRenderer::_glBeginScene(uint32 color, SoraHandle t, bool clear) {
		int32 width = _oglWindowInfo.width;
		int32 height = _oglWindowInfo.height;
        
		if(t) {
			pCurTarget = (SoraRenderTargetiOSGL*)t;
			width = pCurTarget->getWidth();
			height = pCurTarget->getHeight();
            
            pCurTarget->attachToRender();
            applyTransform();
			CurBlendMode = 0;
            glClearColor(CGETR(color), CGETG(color), CGETB(color), CGETA(color));
			
			if(clear)
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
        } else {
            // ios gl wrapper
            setupEAGLFBO();
            
            glClearColor(CGETR(color), CGETG(color), CGETB(color), CGETA(color));
            
            if(clear)
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        
	}
	
	void SoraiOSGLRenderer::_glEndScene() {
		flush();
		if(pCurTarget != NULL) {
            glFlush();
			pCurTarget->detachFromRender();
            pCurTarget = 0;
			
            setTransform(0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f);
            setClipping();
        } else {
            iFrameStart = 0;
            presentEAGLFBO();
        }
	}
	
	void SoraiOSGLRenderer::_glSetBlendMode(int32 blend) {
		if(blend != CurBlendMode)
			flush();
		
        //		glDisable(GL_ALPHA_TEST);
		glEnable(GL_BLEND); // Enable Blending
		
		if((blend & BLEND_SRCALPHA) != (CurBlendMode & BLEND_SRCALPHA))
			if(blend & BLEND_SRCALPHA)
				glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
        
		if((blend & BLEND_ALPHABLEND) != (CurBlendMode & BLEND_ALPHABLEND)) {
			if(blend & BLEND_ALPHABLEND) {
                if(pCurTarget)
                    // alpha trick with FBO transparent background
                    // see http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=257628
                    
                    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                
                else 
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);      //Alpha blending
            }
			else
				glBlendFunc(GL_SRC_ALPHA, GL_ONE); //Addictive
		}
        
		if((blend & BLEND_ZWRITE) != (CurBlendMode & BLEND_ZWRITE)) {
			if(blend & BLEND_ZWRITE) {
                glEnable(GL_DEPTH_TEST);
            } else {
                glDisable(GL_DEPTH_TEST);
            }
		}
        
		if((blend & BLEND_COLORADD) != (CurBlendMode & BLEND_COLORADD)) {
			if(blend & BLEND_COLORADD) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
			else glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
        
		CurBlendMode = blend;
	}
	
	bool SoraiOSGLRenderer::update() {
		//clearPoll();
		return false;
	}
	
	void SoraiOSGLRenderer::start(SoraTimer* timer) {
		SoraiOSInitializer::Instance()->setTimer(timer);
		g_timer = timer;
	}

	void SoraiOSGLRenderer::beginScene(uint32 color, SoraHandle target, bool clear) {
		_glBeginScene(color, target, clear);
	}

	void SoraiOSGLRenderer::endScene() {
		currShader = 0;
		_glEndScene();
	}

	SoraWindowHandle SoraiOSGLRenderer::createWindow(SoraWindowInfoBase* windowInfo) {
		mainWindow = windowInfo;
		_oglWindowInfo.width = windowInfo->getWindowWidth();
		_oglWindowInfo.height = windowInfo->getWindowHeight();
		
		_glInitialize();
		_glSetProjectionMatrix(_oglWindowInfo.width, _oglWindowInfo.height);
		applyTransform();
		
		return (SoraHandle)windowInfo;
		
		return 0;
	}
	
	void SoraiOSGLRenderer::setWindowSize(int32 w, int32 h) {
	}
	
	void SoraiOSGLRenderer::setWindowTitle(const StringType& title) {
	}
	
	void SoraiOSGLRenderer::setWindowPos(int32 px, int32 py) {
	}
	
	void SoraiOSGLRenderer::setFullscreen(bool flag) {

	}
	
	bool SoraiOSGLRenderer::isFullscreen() {
        return true;
	}
	
	SoraHandle SoraiOSGLRenderer::getVideoDeviceHandle() {
		return (SoraHandle)this;
	}
	
	int32 SoraiOSGLRenderer::_glTextureGetWidth(SoraHandle tex, bool bOriginal) {
		/*GLint w; 
		glBindTexture(GL_TEXTURE_2D, tex);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glBindTexture(GL_TEXTURE_2D, mCurrTexture);
		return w;*/
		return 0;
	}
	
	int32 SoraiOSGLRenderer::_glTextureGetHeight(SoraHandle tex, bool bOriginal) {
		/*GLint h; 
		glBindTexture(GL_TEXTURE_2D, tex);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
		glBindTexture(GL_TEXTURE_2D, mCurrTexture);
		return h;*/
		return 0;
	}
	
	void SoraiOSGLRenderer::bindTexture(SoraTexture* tex) {
        if(!tex) {
			flush();
			
            mCurrTexture = 0;
            glBindTexture(GL_TEXTURE_2D, 0);
        }
		else if (mCurrTexture != tex->mTextureID) {
			flush();
			
			glBindTexture(GL_TEXTURE_2D, tex->mTextureID);
			mCurrTexture = tex->mTextureID;
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
		}
	}
	
	void SoraiOSGLRenderer::flush() {
		if(VertexBuffer.size() == 0)
            return;
        
		glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        
        glVertexPointer(3, GL_FLOAT, sizeof(SoraVertex), &VertexBuffer[0].x);
        glTexCoordPointer(2, GL_FLOAT, sizeof(SoraVertex), &(VertexBuffer[0].tx));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SoraVertex), &(VertexBuffer[0].col));
        
        glDrawArrays(CurDrawMode, 0, VertexBuffer.size());
        
        VertexBuffer.reset();
	}

	SoraTexture* SoraiOSGLRenderer::createTexture(const StringType& sTexturePath, bool bMipmap) {
		//return mTextureCreate(ws2s(sTexturePath).c_str(), false);
		// to do
		return 0;
	}
	
	// to do
	SoraTexture* SoraiOSGLRenderer::createTextureFromMem(void* ptr, uint32 size, bool bMipmap) {
		/*return mTextureCreateWithData(ptr, size);*/
		SoraHandle  texid;
		int w, h, channels;
		
		texid = SOIL_load_OGL_texture_and_info_from_memory((const unsigned char* const)ptr,
														   size,
														   SOIL_LOAD_AUTO,
														   SOIL_CREATE_NEW_ID,
														   (bMipmap ? SOIL_FLAG_MIPMAPS : 0)
														   | /*SOIL_FLAG_POWER_OF_TWO |*/ SOIL_FLAG_TEXTURE_REPEATS/* | SOIL_FLAG_INVERT_Y*/,
														   &w, &h, &channels);
		
		if(!texid) {
			log_error("Error loading texture");
            return 0;
		}
		
		
		// because theres no glgettextureleveliv, so we need to handle this by ourself
		int i;
		int p2w, p2h;
		if((w != 1)) {
			i = 1;
			while(i < w)
				i *= 2;
			p2w = i;
		}
		if((h != 1)) {
			i = 1;
			while(i < h)
				i *= 2;
			p2h = i;
		}
		
		SoraTexture* tex = new SoraTexture(texid,
										   w,
										   h,
										   w,
										   h);
		return tex;
	}

	SoraTexture* SoraiOSGLRenderer::createTextureWH(int w, int h) {
		// ios max texture size = 1024
		if(w > 1024 || h > 1024)
			return NULL;
        
        int i;
		int p2w, p2h;
		if((w != 1)) {
			i = 1;
			while(i < w)
				i *= 2;
			p2w = i;
		}
		if((h != 1)) {
			i = 1;
			while(i < h)
				i *= 2;
			p2h = i;
		}
		/*size_t size = w*h*4;
		GLubyte* bitData = new GLubyte[size];
		
		SoraHandle texid = SOIL_create_OGL_texture(bitData,
												w,
												h,
												4,
												0,
												SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_TEXTURE_REPEATS);
		if(!texid) {
			throw SORA_EXCEPTION("Error creating texture");
		}*/
		GLuint texId;
			
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameterf(texId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(texId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(texId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(texId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		return new SoraTexture(texId, p2w, p2h, w, h);
	}
	
	SoraTexture* SoraiOSGLRenderer::createTextureFromRawData(unsigned int* data, int32 w, int32 h) {
		// ios max texture size = 1024
		if(w > 1024 || h > 1024)
			return NULL;
		
		SoraHandle texid = SOIL_create_OGL_texture((unsigned char*)data,
												w,
												h,
												4,
												0,
												SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_TEXTURE_REPEATS);
		if(!texid) {
			throw SORA_EXCEPTION(RuntimeException, "Error creating texture");
			return 0;
		}
		
		
		int i;
		int p2w, p2h;
		if((w != 1) && (w & (w - 1))) {
			i = 1;
			while(i < w)
				i *= 2;
			p2w = i;
		}
		if((h != 1) && (h & (h - 1))) {
			i = 1;
			while(i < h)
				i *= 2;
			p2h = i;
		}
		
		SoraTexture* tex = new SoraTexture(texid,
										   w,
										   h,
										   w,
										   h);
		return tex;
	}

	uint32* SoraiOSGLRenderer::textureLock(SoraTexture* ht) {
		ht->mTexData = sora_malloc_t(uint32, ht->mOriginalWidth * ht->mOriginalHeight);
		memset(ht->mTexData, 0, ht->mOriginalWidth * ht->mOriginalHeight);
		if(ht->mTexData) {
            glEnable(GL_TEXTURE_2D);
			
            GLint PreviousTexture;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &PreviousTexture);
			
            glBindTexture(GL_TEXTURE_2D, ht->mTextureID);
			glReadPixels(0, 0, ht->mOriginalWidth, ht->mOriginalHeight, GL_RGBA, GL_UNSIGNED_BYTE, ht->mTexData);
            if(glGetError() != GL_NO_ERROR) {
                sora_free(ht->mTexData);
                ht->mTexData = 0;
                return NULL;
            }
            
            glBindTexture(GL_TEXTURE_2D, PreviousTexture);
			return ht->mTexData;
		}
		return 0;
	}
	
	void SoraiOSGLRenderer::textureUnlock(SoraTexture* ht) {
		if(ht->mTexData != NULL) {
			glEnable(GL_TEXTURE_2D);
            
            GLint PreviousTexture;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &PreviousTexture);
			
            glBindTexture(GL_TEXTURE_2D, ht->mTextureID);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ht->mOriginalWidth, ht->mOriginalHeight, GL_RGBA, GL_UNSIGNED_BYTE, ht->mTexData);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            glBindTexture(GL_TEXTURE_2D, PreviousTexture);
			
			sora_free(ht->mTexData);
			ht->mTexData = 0;
		}
		
	}

	void SoraiOSGLRenderer::releaseTexture(SoraTexture* tex) {
		glDeleteTextures(1, (const GLuint*)&tex->mTextureID);
		delete tex;
		tex = 0;
	}
	
	int32 SoraiOSGLRenderer::_modeToGLMode(int32 mode) {
		switch (mode) {
			case Line:			return GL_LINES;
            case LineLoop:      return GL_LINE_LOOP;
			case Triangle:		return GL_TRIANGLES;
			case TriangleFan:	return GL_TRIANGLE_FAN;
			case TriangleStrip:	return GL_TRIANGLE_STRIP;
	//		case SORA_QUAD:				return GL_QUADS;
		}
		return GL_TRIANGLES;
	}
	
	void SoraiOSGLRenderer::renderTriple(SoraTriple& trip) {
		glEnable(GL_TEXTURE_2D); 
		if(trip.tex) {
			bindTexture(trip.tex);
		} else {
            flush();
            bindTexture(0);
        }
		_glSetBlendMode(trip.blend);
		CurDrawMode = GL_TRIANGLES;
        
        SoraVertex tmp = trip.v[0];
        tmp.col = CARGB(CGETA(tmp.col), CGETB(tmp.col), CGETG(tmp.col), CGETR(tmp.col));
        VertexBuffer.append(tmp);
        tmp = trip.v[1];
        tmp.col = CARGB(CGETA(tmp.col), CGETB(tmp.col), CGETG(tmp.col), CGETR(tmp.col));
        VertexBuffer.append(tmp);
        tmp = trip.v[2];
        tmp.col = CARGB(CGETA(tmp.col), CGETB(tmp.col), CGETG(tmp.col), CGETR(tmp.col));
        VertexBuffer.append(tmp);
        
        if(currShader)
			flush();
		if(!trip.tex)
			flush();
	}
	
	void SoraiOSGLRenderer::renderWithVertices(SoraTexture* tex, int32 blendMode,  SoraVertex* vertices, uint32 vsize, RenderMode mode) {		
		flush();
		
		glEnable(GL_TEXTURE_2D);
		if(tex) {
            bindTexture(tex);
		} else {
            bindTexture(0);
        }
		_glSetBlendMode(blendMode);
		CurDrawMode = RenderModeToGLMode(mode);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        
        glVertexPointer(3, GL_FLOAT, sizeof(SoraVertex), &vertices[0].x);
        glTexCoordPointer(2, GL_FLOAT, sizeof(SoraVertex), &(vertices[0].tx));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SoraVertex), &(vertices[0].col));
        
        glDrawArrays(CurDrawMode, 0, vsize);
	}
	
	void SoraiOSGLRenderer::renderQuad(SoraQuad& quad) {
		glEnable(GL_TEXTURE_2D);
		if(quad.tex) {
			bindTexture(quad.tex);
		} else {
            flush();
            bindTexture(0);
        }
		_glSetBlendMode(quad.blend);
		CurDrawMode = GL_TRIANGLES;
        
        SoraVertex tmp = quad.v[0];
        tmp.col = CARGB(CGETA(tmp.col), CGETB(tmp.col), CGETG(tmp.col), CGETR(tmp.col));
        VertexBuffer.append(tmp);
        tmp = quad.v[1];
        tmp.col = CARGB(CGETA(tmp.col), CGETB(tmp.col), CGETG(tmp.col), CGETR(tmp.col));
        VertexBuffer.append(tmp);
        tmp = quad.v[2];
        tmp.col = CARGB(CGETA(tmp.col), CGETB(tmp.col), CGETG(tmp.col), CGETR(tmp.col));
        VertexBuffer.append(tmp);
        tmp = quad.v[3];
        tmp.col = CARGB(CGETA(tmp.col), CGETB(tmp.col), CGETG(tmp.col), CGETR(tmp.col));
        VertexBuffer.append(tmp);
        tmp = quad.v[0];
        tmp.col = CARGB(CGETA(tmp.col), CGETB(tmp.col), CGETG(tmp.col), CGETR(tmp.col));
        VertexBuffer.append(tmp);
        tmp = quad.v[2];
        tmp.col = CARGB(CGETA(tmp.col), CGETB(tmp.col), CGETG(tmp.col), CGETR(tmp.col));
        VertexBuffer.append(tmp);
		
		if(currShader)
			flush();
		if(!quad.tex)
			flush();
	}

	bool SoraiOSGLRenderer::isActive() {
		return true;
	}

	void SoraiOSGLRenderer::setClipping(int32 x, int32 y, int32 w, int32 h) {
		if(w == 0 && h == 0) {
            w = _oglWindowInfo.width;
            h = _oglWindowInfo.height;
        }
        if(!pCurTarget) {
            glScissor(x, _oglWindowInfo.height-(y+h), w, h);
        }
        else {
            glScissor(x, y, w, h);
        }
	}

	void SoraiOSGLRenderer::setTransform(float x, float y, float dx, float dy, float rot, float hscale, float vscale) {
		_oglWindowInfo.x		=	x;
		_oglWindowInfo.y		=	y;
		_oglWindowInfo.dx		=	dx;
		_oglWindowInfo.dy		=	dy;
		_oglWindowInfo.rot		=	rot;
		_oglWindowInfo.hscale	=	hscale!=0.f?hscale:1.f;
		_oglWindowInfo.vscale	=	vscale!=0.f?hscale:1.f;
		flush();
		applyTransform();
	}

	SoraHandle SoraiOSGLRenderer::createTarget(int width, int height, bool zbuffer) {        
		SoraRenderTargetiOSGL* t = new SoraRenderTargetiOSGL(width, height, zbuffer);
		liTargets.push_back((SoraRenderTargetiOSGL*)t);
		return (SoraHandle)t;
	}

	void SoraiOSGLRenderer::freeTarget(SoraHandle t) {
		SoraRenderTargetiOSGL* pt = (SoraRenderTargetiOSGL*)t;
		if(!pt) return;
		std::list<SoraRenderTargetiOSGL*>::iterator itt = liTargets.begin();
		while(itt != liTargets.end()) {
			if((*itt) == pt) {
				delete pt;
				liTargets.erase(itt);
				break;
			}
			++itt;
		}
	}

	SoraHandle SoraiOSGLRenderer::getTargetTexture(SoraHandle t) {
		SoraRenderTargetiOSGL* pt = (SoraRenderTargetiOSGL*)t;
        assert(pt != NULL);
		
		return pt->getTexture();
	}
	
	bool SoraiOSGLRenderer::_glVersionCheck() {
		return true;
	}
	
	bool SoraiOSGLRenderer::_glShaderCheck() {
		// to do
		// check extensions
		// return glfwExtensionSupported("GL_ARB_fragment_shade");
		return _glVersionCheck();
	}
	
	inline bool SoraiOSGLRenderer::_glCheckError() {
		return glGetError() != GL_NO_ERROR;
	}
	
	StringType SoraiOSGLRenderer::videoInfo() {
		StringType info("Driver: OpenGL Version ");
		//int mav, miv, rev;
		info += s2ws((char*)glGetString(GL_VERSION));
		return info;
	}
	
	SoraShaderContext* SoraiOSGLRenderer::createShaderContext() {
		//return new SoraGLSLShaderContext();
        return 0;
	}
	
	void SoraiOSGLRenderer::snapshot(const StringType& file) {
	}
	
	void SoraiOSGLRenderer::setViewPoint(float, float, float) {
	}

	void SoraiOSGLRenderer::attachShaderContext(SoraShaderContext* context) {
		currShader = context;
	}
    
    void SoraiOSGLRenderer::setVerticalSync(bool flag) {
    }

	void SoraiOSGLRenderer::detachShaderContext() {
		if(!currShader->detachShaderList())
			log_error("SoraiOSGLRenderer: error detaching shader list");
		flush();
		currShader = 0;
	}

    void SoraiOSGLRenderer::onExtensionStateChanged(int32 extension, bool state, int32 param) {

    }
    
    void SoraiOSGLRenderer::renderLine(float x1, float y1, float x2, float y2, uint32 color, float width, float z) {
		sora::SoraQuad quad;
		
		quad.tex = NULL;
		
        SoraVector p1(x1, y1);
        SoraVector p2(x2, y2);
        
        // Compute the extrusion direction
        SoraVector normal = p1.normal(p2);
        normal *= width / 2;
        
        SoraVector q1 = p1 - normal;
        SoraVector q2 = p2 - normal;
        SoraVector q3 = p2 + normal;
        SoraVector q4 = p1 + normal;
        
        quad.v[0].x = q1.x; quad.v[0].y = q1.y;
        quad.v[1].x = q2.x; quad.v[1].y = q2.y;
        quad.v[2].x = q3.x; quad.v[2].y = q3.y;
        quad.v[3].x = q4.x; quad.v[3].y = q4.y;
		
		int i;
		for (i = 0; i < 4; ++i) {
			quad.v[i].z = z;
            quad.v[i].col = color;
		}
		
		quad.blend = BLEND_DEFAULT;
		
		renderQuad(quad);
	}
    
    void SoraiOSGLRenderer::fillBox(float x1, float y1, float x2, float y2, uint32 color, float z) {
        sora::SoraQuad quad;
		
		quad.tex = NULL;
		
		quad.v[0].x   = x1;
		quad.v[0].y   = y1;
		quad.v[0].col = color;
		
		quad.v[1].x   = x2;
		quad.v[1].y   = y1;
		quad.v[1].col = color;
		
		quad.v[2].x   = x2;
		quad.v[2].y   = y2;
		quad.v[2].col = color;
		
		quad.v[3].x   = x1;
		quad.v[3].y   = y2;
		quad.v[3].col = color;
		
		int i;
		for (i = 0; i < 4; ++i) {
			quad.v[i].z = z;
		}
		
		quad.blend = BLEND_DEFAULT;
		
		renderQuad(quad);
    }
	
	void SoraiOSGLRenderer::renderBox(float x1, float y1, float x2, float y2, uint32 color, float lineWidth, float z) {
		renderLine(x1, y1, x2, y1+1.f, color, lineWidth, z);
		renderLine(x2, y1, x2+1.f, y2, color, lineWidth, z);
		renderLine(x2, y2, x1, y2+1.f, color, lineWidth, z);
		renderLine(x1, y2, x1+1.f, y1, color, lineWidth, z);
	}
    
    void SoraiOSGLRenderer::setIcon(const StringType& icon) {
        
    }
    
    void SoraiOSGLRenderer::setCursor(const StringType& cursor) {
        
    }
    
    void SoraiOSGLRenderer::setOrientation(iOSOrientation por) {
        mOrientation = por;
    }
    
    iOSOrientation SoraiOSGLRenderer::getOrientation() const {
        return mOrientation;
    }
    
    void SoraiOSGLRenderer::getDesktopResolution(int* w, int* h) {
        *w = sora::getScreenWidth();
        *h = sora::getScreenHeight();
    }
    
    void SoraiOSGLRenderer::setQueryVideoModeCallback(QueryVideoMode func) {
        if(func) {
            func(getScreenWidth(), getScreenHeight());
        }
    }
    
    SoraHandle SoraiOSGLRenderer::getMainWindowHandle() { 
        return (SoraHandle)mainWindow;
    }
    
    SoraWindowInfoBase* SoraiOSGLRenderer::getMainWindow() {
        return mainWindow;
    }
    
    
    void SoraiOSGLRenderer::setTransformMatrix(const SoraMatrix4& matrix) {
        flush();
        
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(&matrix.x[0]);
    }
    
    SoraMatrix4 SoraiOSGLRenderer::getTransformMatrix() const {        
        glMatrixMode(GL_MODELVIEW);
        float matrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0]);
        return SoraMatrix4(matrix);
    }
    
    void SoraiOSGLRenderer::setProjectionMatrix(const SoraMatrix4& matrix) {
        flush();
        
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(&matrix.x[0]);
    }
    
    void SoraiOSGLRenderer::multTransformMatrix(const SoraMatrix4& matrix) {
        flush();
        
        glMatrixMode(GL_MODELVIEW);
        glMultMatrixf(&matrix.x[0]);
    }
    
    SoraMatrix4 SoraiOSGLRenderer::getProjectionMatrix() const {
        glMatrixMode(GL_PROJECTION);
        float matrix[16];
        glGetFloatv(GL_PROJECTION_MATRIX, &matrix[0]);
        return SoraMatrix4(matrix);
    }
    
    void SoraiOSGLRenderer::switchTo2D() {
        if(!pCurTarget)
            setProjectionMatrix(sora::SoraMatrix4::OrthoMat(0.f, _oglWindowInfo.width, _oglWindowInfo.height, 0.f, 0.f, 1.f));
        else 
            setProjectionMatrix(sora::SoraMatrix4::OrthoMat(0.f, pCurTarget->getWidth(), 0.f, pCurTarget->getHeight(), 0.f, 1.f));
        
        glDisable(GL_DITHER);
        glDisable(GL_FOG);
        
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
    }
    
    void SoraiOSGLRenderer::switchTo3D() {
        if(!pCurTarget)
            setProjectionMatrix(sora::SoraMatrix4::OrthoMat(0.f, _oglWindowInfo.width, _oglWindowInfo.height, 0.f, -1000.f, 1000.f));
        else 
            setProjectionMatrix(sora::SoraMatrix4::OrthoMat(0.f, pCurTarget->getWidth(), 0.f, pCurTarget->getHeight(), -1000.f, 1000.f));
    }
    
    void SoraiOSGLRenderer::setRenderState(RenderStateType type, RenderStateParam param) {
        switch(type) {
            case TextureWrap0:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, RenderParamToGLParam(param));
                break;
            case TextureWrap1:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, RenderParamToGLParam(param));
                break;
        }
    }
    
    RenderStateParam SoraiOSGLRenderer::getRenderState(RenderStateType) const {
        
    }
    
} // namespace sora