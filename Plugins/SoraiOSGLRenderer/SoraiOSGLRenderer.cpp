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
#include "Rect4V.h"
#include "SoraColor.h"

#include "SoraCore.h"
#include "SoraInfiniteRendererCallback.h"

#include "SoraGLSLShader.h"

#include "SoraiOSInitializer.h"
#include "SOIL/SOIL.h"

const uint32 MAX_VERTEX_BUFFER = 2048;

static int mVertexCount = 0;
static GLfloat mVertices[MAX_VERTEX_BUFFER*3];
static GLfloat mUVs[MAX_VERTEX_BUFFER<<1];
static GLubyte mColors[MAX_VERTEX_BUFFER<<2];

namespace sora{

	SoraiOSGLRenderer::SoraiOSGLRenderer() {
		pCurTarget = 0;
		uGLShaderProgram = 0;
		
		bShaderAvailable = _glShaderCheck();
		
		mCurrTexture = -1;
		
		currShader = 0;
		iFrameStart = 1;
		CurBlendMode = 0;
	}

	SoraiOSGLRenderer::~SoraiOSGLRenderer() {
	//	shutdown();
	}

	void SoraiOSGLRenderer::shutdown() {
		std::list<SoraRenderTarget*>::iterator itt = liTargets.begin();
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
		glClearDepthf(1.f);                          // Depth Buffer Setup
        
        //glDepthMask(GL_FALSE);
        
		glEnable(GL_CULL_FACE);
		glEnable(GL_COLOR_MATERIAL);
		
		//   InitPerspective(60, (float)_oglWindowInfo.width / _oglWindowInfo.height, 0.f, 1.f);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations
		
		glDisable(GL_DITHER);
		glDisable(GL_FOG);
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		glDisable(GL_TEXTURE_2D);
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
		
		//Clearing the projection matrix...
		glLoadIdentity();
		//Creating an orthoscopic view matrix going from -1 -> 1 in each
		//dimension on the screen (x, y, z).
		glOrthof(0.f, w, h, 0.f, -1.f, 1.f);
	}
	
	void SoraiOSGLRenderer::applyTransform() {
        if(!pCurTarget) {
            glViewport(0, 0,
                       _oglWindowInfo.width,
                       _oglWindowInfo.height);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrthof(0.f,
                    _oglWindowInfo.width,
                    _oglWindowInfo.height
                    , 0.f, -1.f, 1.f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
            glTranslatef(_oglWindowInfo.x-_oglWindowInfo.dx, _oglWindowInfo.y-_oglWindowInfo.dy, 0.f); //Set Center Coodinates
            glRotatef(_oglWindowInfo.rot, -0.f, 0.f, 1.f);
            glScalef(_oglWindowInfo.hscale, _oglWindowInfo.vscale, 1.0f);//Transformation follows order scale->rotation->displacement
			
            glTranslatef(-_oglWindowInfo.x, -_oglWindowInfo.y, 0.f);
        } else {
            glViewport(0, 0,
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
            glTranslatef(0.f, 0.f, 0.f); //Set Center Coodinates
            glRotatef(0.f, -0.f, 0.f, 1.f);
            glScalef(1.f, 1.f, 1.0f);//
			
            glTranslatef(0.f, 0.f, 0.f);
        }
	}
	
	void SoraiOSGLRenderer::_glBeginScene(ulong32 color, ulong32 t, bool clear) {
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
			else
				glClear(GL_DEPTH_BUFFER_BIT);
			
        } else {
            glClearColor(CGETR(color), CGETG(color), CGETB(color), CGETA(color));
            
            if(clear)
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            else
                glClear(GL_DEPTH_BUFFER_BIT);
        }
        
	}
	
	void SoraiOSGLRenderer::_glEndScene() {
		flush();
		if(pCurTarget != NULL) {
            glFlush();
			pCurTarget->detachFromRender();
            pCurTarget = 0;
			
            applyTransform();
        } else 
            iFrameStart = 0;
		//else
		//	glfwSwapBuffers();
		
	}
	
	void SoraiOSGLRenderer::_glSetBlendMode(int32 blend) {
		if(blend != CurBlendMode)
			flush();
		
		//		glDisable(GL_ALPHA_TEST);
		glEnable(GL_BLEND); // Enable Blending
		
		if((blend & BLEND_ALPHABLEND) != (CurBlendMode & BLEND_ALPHABLEND)) {
			if(blend & BLEND_ALPHABLEND)
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);      //Alpha blending
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
		SORA_IOS->setTimer(timer);
		g_timer = timer;
	}

	void SoraiOSGLRenderer::beginScene(ulong32 color, ulong32 target, bool clear) {
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
		
		bFullscreen = /*windowInfo->isWindowed()*/ true;
		windowInfo->init();		

		return (ulong32)windowInfo;
		
		return 0;
	}
	
	void SoraiOSGLRenderer::setWindowSize(int32 w, int32 h) {
	//	glfwSetWindowSize(w, h);
	}
	
	void SoraiOSGLRenderer::setWindowTitle(const SoraWString& title) {
	//	glfwSetWindowTitle(ws2s(title).c_str());
	}
	
	void SoraiOSGLRenderer::setWindowPos(int32 px, int32 py) {
	//	glfwSetWindowPos(px, py);
	}
	
	void SoraiOSGLRenderer::setFullscreen(bool flag) {
		/*if(bFullscreen == flag)
			return;
		
		endScene();
		
		bFullscreen = flag;
		glfwCloseWindow();
		glfwOpenWindow(mainWindow->getWindowWidth(), mainWindow->getWindowHeight(),
					   0, 0, 0, 0,
					   32,
					   0,
					   bFullscreen==true?GLFW_FULLSCREEN:GLFW_WINDOW);
		glfwSetWindowTitle(mainWindow->getWindowName().c_str());
		glfwSetWindowCloseCallback(int_exitFunc);

		_glInitialize();
		_glSetProjectionMatrix(_oglWindowInfo.width, _oglWindowInfo.height);
		
		beginScene();*/
	}
	
	bool SoraiOSGLRenderer::isFullscreen() {
		return bFullscreen;
	}
	
	ulong32 SoraiOSGLRenderer::getVideoDeviceHandle() {
		return (ulong32)this;
	}
	
	int32 SoraiOSGLRenderer::_glTextureGetWidth(ulong32 tex, bool bOriginal) {
		/*GLint w; 
		glBindTexture(GL_TEXTURE_2D, tex);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glBindTexture(GL_TEXTURE_2D, mCurrTexture);
		return w;*/
		return 0;
	}
	
	int32 SoraiOSGLRenderer::_glTextureGetHeight(ulong32 tex, bool bOriginal) {
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
		if (mVertexCount > 0) {
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			
			glVertexPointer(3, GL_FLOAT, 0, mVertices);
			glTexCoordPointer(2, GL_FLOAT, 0, mUVs);
			
			glColorPointer (4, GL_UNSIGNED_BYTE, 0, mColors);
			glDrawArrays(CurDrawMode, 0, mVertexCount);
			mVertexCount = 0;
		}
	}

	SoraTexture* SoraiOSGLRenderer::createTexture(const SoraWString& sTexturePath, bool bMipmap) {
		//return mTextureCreate(ws2s(sTexturePath).c_str(), false);
		// to do
		return 0;
	}
	
	// to do
	SoraTexture* SoraiOSGLRenderer::createTextureFromMem(void* ptr, ulong32 size, bool bMipmap) {
		/*return mTextureCreateWithData(ptr, size);*/
		ulong32  texid;
		int w, h, channels;
		
		texid = SOIL_load_OGL_texture_and_info_from_memory((const unsigned char* const)ptr,
														   size,
														   SOIL_LOAD_AUTO,
														   SOIL_CREATE_NEW_ID,
														   (bMipmap ? SOIL_FLAG_MIPMAPS : 0)
														   | /*SOIL_FLAG_POWER_OF_TWO |*/ SOIL_FLAG_TEXTURE_REPEATS/* | SOIL_FLAG_INVERT_Y*/,
														   &w, &h, &channels);
		
		if(!texid) {
			throw SORA_EXCEPTION("Error loading texture");
		}
		
		// ios max texture size = 1024
		if(w > 1024 || h > 1024)
			return NULL;
		
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
		
		printf("%d %d %d, %d, %d\n", w, h, channels, p2w, p2h);

		
		SoraTexture* tex = new SoraTexture(texid,
										   p2w,
										   p2h,
										   w,
										   h);
		return tex;
	}

	SoraTexture* SoraiOSGLRenderer::createTextureWH(int w, int h) {
		// ios max texture size = 1024
		if(w > 1024 || h > 1024)
			return NULL;
		/*size_t size = w*h*4;
		GLubyte* bitData = new GLubyte[size];
		
		ulong32 texid = SOIL_create_OGL_texture(bitData,
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_INT, 0);
			
		return new SoraTexture(texId, w, h, w, h);
	}
	
	SoraTexture* SoraiOSGLRenderer::createTextureFromRawData(unsigned int* data, int32 w, int32 h) {
		// ios max texture size = 1024
		if(w > 1024 || h > 1024)
			return NULL;
		
		ulong32 texid = SOIL_create_OGL_texture((unsigned char*)data,
												w,
												h,
												4,
												0,
												SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_TEXTURE_REPEATS);
		if(!texid) {
			throw SORA_EXCEPTION("Error creating texture");
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
										   p2w,
										   p2h,
										   w,
										   h);
		return tex;
	}

	uint32* SoraiOSGLRenderer::textureLock(SoraTexture* ht) {
		ht->dataRef.texData = new uint32[ht->mOriginalWidth * ht->mOriginalHeight];
		memset(ht->dataRef.texData, 0, ht->mOriginalWidth * ht->mOriginalHeight);
		if(ht->dataRef.texData) {
            glEnable(GL_TEXTURE_2D);
			
            GLint PreviousTexture;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &PreviousTexture);
			
            glBindTexture(GL_TEXTURE_2D, ht->mTextureID);
			glReadPixels(0, 0, ht->mOriginalWidth, ht->mOriginalHeight, GL_RGBA, GL_UNSIGNED_BYTE, ht->dataRef.texData);
            if(glGetError() != GL_NO_ERROR) {
                delete ht->dataRef.texData;
                ht->dataRef.texData = 0;
                return NULL;
            }
            
            glBindTexture(GL_TEXTURE_2D, PreviousTexture);
			return ht->dataRef.texData;
		}
		return 0;
	}
	
	void SoraiOSGLRenderer::textureUnlock(SoraTexture* ht) {
		if(ht->dataRef.texData != NULL) {
			glEnable(GL_TEXTURE_2D);
            
            GLint PreviousTexture;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &PreviousTexture);
			
            glBindTexture(GL_TEXTURE_2D, ht->mTextureID);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ht->mOriginalWidth, ht->mOriginalHeight, GL_RGBA, GL_UNSIGNED_BYTE, ht->dataRef.texData);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            glBindTexture(GL_TEXTURE_2D, PreviousTexture);
			
			delete ht->dataRef.texData;
			ht->dataRef.texData = 0;
		}
		
	}

	void SoraiOSGLRenderer::releaseTexture(SoraTexture* tex) {
		glDeleteTextures(1, (const GLuint*)&tex->mTextureID);
		delete tex;
		tex = 0;
	}
	
	int32 SoraiOSGLRenderer::_modeToGLMode(int32 mode) {
		switch (mode) {
	//		case SORA_LINE:				return GL_LINE;
			case SORA_TRIANGLES:		return GL_TRIANGLES;
			case SORA_TRIANGLES_FAN:	return GL_TRIANGLE_FAN;
			case SORA_TRIANGLES_STRIP:	return GL_TRIANGLE_STRIP;
	//		case SORA_QUAD:				return GL_QUADS;
		}
		return GL_TRIANGLES;
	}
	
	void SoraiOSGLRenderer::renderTriple(SoraTriple& trip) {
		if(currShader != NULL) {
			flush();
		}
		
		glEnable(GL_TEXTURE_2D); // Enable Texture Mapping
		if(trip.tex) {
			
			//glBindTexture(GL_TEXTURE_2D, quad.tex->mTextureID);
			bindTexture(trip.tex);
		} else {
            flush();
            bindTexture(0);
        }
		_glSetBlendMode(trip.blend);
		CurDrawMode = GL_TRIANGLES;
		
		//TODO: insert code here
		GLfloat verteces[9] = {
			trip.v[2].x, trip.v[2].y, trip.v[2].z,
			trip.v[1].x, trip.v[1].y, trip.v[1].z,
			trip.v[0].x, trip.v[0].y, trip.v[0].z,
		};
		
		GLfloat texCoords[8] = {
			trip.v[2].tx, trip.v[2].ty,
			trip.v[1].tx, trip.v[1].ty,
			trip.v[0].tx, trip.v[0].ty,
		};
		GLubyte colors[16] = {
			(GLubyte)CGETR(trip.v[2].col), (GLubyte)CGETG(trip.v[2].col), (GLubyte)CGETB(trip.v[2].col), (GLubyte)CGETA(trip.v[2].col),
			(GLubyte)CGETR(trip.v[1].col), (GLubyte)CGETG(trip.v[1].col), (GLubyte)CGETB(trip.v[1].col), (GLubyte)CGETA(trip.v[1].col),
			(GLubyte)CGETR(trip.v[0].col), (GLubyte)CGETG(trip.v[0].col), (GLubyte)CGETB(trip.v[0].col), (GLubyte)CGETA(trip.v[0].col),
		};
		
		if (mVertexCount >= MAX_VERTEX_BUFFER-3)
			flush();
		
		int u = 0;
		int idx = 0;
		int cdx = 0;
		for (int i=0;i<3;i++) {
			mVertices[(mVertexCount*3)] = verteces[idx++];
			mVertices[(mVertexCount*3)+1] = verteces[idx++];
			mVertices[(mVertexCount*3)+2] = verteces[idx++];
			
			mUVs[(mVertexCount<<1)] = texCoords[u++];
			mUVs[(mVertexCount<<1)+1] = texCoords[u++];
			
			mColors[(mVertexCount<<2)] = colors[cdx++];
			mColors[(mVertexCount<<2)+1] = colors[cdx++];
			mColors[(mVertexCount<<2)+2] = colors[cdx++];
			mColors[(mVertexCount<<2)+3] = colors[cdx++];
			mVertexCount++;
		}
		
		if(currShader) {
			if(!currShader->attachShaderList())
				SoraCore::Instance()->log("SoraiOSGLRenderer: error attaching shader list");
			flush();
		}

		if(!trip.tex)
			flush();	
	}
	
	void SoraiOSGLRenderer::renderWithVertices(SoraTexture* tex, int32 blendMode,  SoraVertex* vertices, uint32 vsize, int32 mode) {		
		flush();
		
		glEnable(GL_TEXTURE_2D); // Enable Texture Mapping
		if(tex) {
			
			//glBindTexture(GL_TEXTURE_2D, quad.tex->mTextureID);
			bindTexture(tex);
		} else {
            bindTexture(0);
        }
		_glSetBlendMode(blendMode);
		CurDrawMode = _modeToGLMode(mode);
		
		for(int i=0; i<vsize; ++i) {
			int ti = i;
			
			mVertices[(mVertexCount*3)] = vertices[ti].x;
			mVertices[(mVertexCount*3)+1] = vertices[ti].y;
			mVertices[(mVertexCount*3)+2] = vertices[ti].z;
			
			mUVs[(mVertexCount<<1)] = vertices[ti].tx;
			mUVs[(mVertexCount<<1)+1] = vertices[ti].ty;
			
			mColors[(mVertexCount<<2)] = (GLubyte)CGETR(vertices[ti].col);
			mColors[(mVertexCount<<2)+1] = (GLubyte)CGETG(vertices[ti].col);
			mColors[(mVertexCount<<2)+2] = (GLubyte)CGETB(vertices[ti].col);
			mColors[(mVertexCount<<2)+3] = (GLubyte)CGETA(vertices[ti].col);
			mVertexCount++;
		}
		
		if(currShader) {
			if(!currShader->attachShaderList())
				SoraCore::Instance()->log("SoraOGLRenderer: error attaching shader list");
		}
		
		flush();
	}
	
	void SoraiOSGLRenderer::renderQuad(SoraQuad& quad) {
		if(currShader != NULL) {
			flush();
		}
		
		glEnable(GL_TEXTURE_2D); // Enable Texture Mapping
		if(quad.tex) {
			
			//glBindTexture(GL_TEXTURE_2D, quad.tex->mTextureID);
			bindTexture(quad.tex);
		} else {
            flush();
            bindTexture(0);
        }
		_glSetBlendMode(quad.blend);
		CurDrawMode = GL_TRIANGLES;
		
		//TODO: insert code here
		GLfloat verteces[18] = {
			quad.v[0].x, quad.v[0].y, quad.v[0].z,
			quad.v[1].x, quad.v[1].y, quad.v[1].z,
			quad.v[2].x, quad.v[2].y, quad.v[2].z,
			
			quad.v[3].x, quad.v[3].y, quad.v[3].z,
			quad.v[0].x, quad.v[0].y, quad.v[0].z,
			quad.v[2].x, quad.v[2].y, quad.v[2].z,
		};
		
		GLfloat texCoords[12] = {
			quad.v[0].tx, quad.v[0].ty,
			quad.v[1].tx, quad.v[1].ty,
			quad.v[2].tx, quad.v[2].ty,
			
			quad.v[3].tx, quad.v[3].ty,
			quad.v[0].tx, quad.v[0].ty,
			quad.v[2].tx, quad.v[2].ty,
		};
		GLubyte colors[24] = {
			(GLubyte)CGETR(quad.v[0].col), (GLubyte)CGETG(quad.v[0].col), (GLubyte)CGETB(quad.v[0].col), (GLubyte)CGETA(quad.v[0].col),
			(GLubyte)CGETR(quad.v[1].col), (GLubyte)CGETG(quad.v[1].col), (GLubyte)CGETB(quad.v[1].col), (GLubyte)CGETA(quad.v[1].col),
			(GLubyte)CGETR(quad.v[2].col), (GLubyte)CGETG(quad.v[2].col), (GLubyte)CGETB(quad.v[2].col), (GLubyte)CGETA(quad.v[2].col),
			
			(GLubyte)CGETR(quad.v[3].col), (GLubyte)CGETG(quad.v[3].col), (GLubyte)CGETB(quad.v[3].col), (GLubyte)CGETA(quad.v[3].col),
			(GLubyte)CGETR(quad.v[0].col), (GLubyte)CGETG(quad.v[0].col), (GLubyte)CGETB(quad.v[0].col), (GLubyte)CGETA(quad.v[0].col),
			(GLubyte)CGETR(quad.v[2].col), (GLubyte)CGETG(quad.v[2].col), (GLubyte)CGETB(quad.v[2].col), (GLubyte)CGETA(quad.v[2].col),
		};
		
		if (mVertexCount >= MAX_VERTEX_BUFFER-6)
			flush();
		
		int u = 0;
		int idx = 0;
		int cdx = 0;
		for (int i=0;i<6;i++) {
			mVertices[(mVertexCount*3)] = verteces[idx++];
			mVertices[(mVertexCount*3)+1] = verteces[idx++];
			mVertices[(mVertexCount*3)+2] = verteces[idx++];
			
			mUVs[(mVertexCount<<1)] = texCoords[u++];
			mUVs[(mVertexCount<<1)+1] = texCoords[u++];
			
			mColors[(mVertexCount<<2)] = colors[cdx++];
			mColors[(mVertexCount<<2)+1] = colors[cdx++];
			mColors[(mVertexCount<<2)+2] = colors[cdx++];
			mColors[(mVertexCount<<2)+3] = colors[cdx++];
			mVertexCount++;
		}
		if(currShader) {
			if(!currShader->attachShaderList())
				SoraCore::Instance()->log("SoraiOSGLRenderer: error attaching shader list");
			flush();
		}

		if(!quad.tex)
			flush();
	}

	bool SoraiOSGLRenderer::isActive() {
		return true;
	}

	void SoraiOSGLRenderer::setClipping(int32 x, int32 y, int32 w, int32 h) {
		glScissor(x, y, w, h);
	}

	void SoraiOSGLRenderer::setTransform(float32 x, float32 y, float32 dx, float32 dy, float32 rot, float32 hscale, float32 vscale) {
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

	ulong32 SoraiOSGLRenderer::createTarget(int width, int height, bool zbuffer) {
        return 0;
        
		SoraRenderTargetiOSGL* t = new SoraRenderTargetiOSGL(width, height, zbuffer);
		liTargets.push_back((SoraRenderTarget*)t);
		return (ulong32)t;
	}

	void SoraiOSGLRenderer::freeTarget(ulong32 t) {
		SoraRenderTarget* pt = (SoraRenderTarget*)t;
		if(!pt) return;
		std::list<SoraRenderTarget*>::iterator itt = liTargets.begin();
		while(itt != liTargets.end()) {
			if((*itt) == pt) {
				delete pt;
				liTargets.erase(itt);
				break;
			}
			++itt;
		}
	}

	ulong32 SoraiOSGLRenderer::getTargetTexture(ulong32 t) {
		SoraRenderTargetiOSGL* pt = (SoraRenderTargetiOSGL*)t;
        assert(pt != NULL);
		
		return pt->getTexture();
	}
	
	bool SoraiOSGLRenderer::_glVersionCheck() {
	/*	int majorVersion, minorVersion, rev;
		glfwGetVersion(&majorVersion, &minorVersion, &rev);
		return majorVersion >= 2;*/
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
	
	SoraWString SoraiOSGLRenderer::videoInfo() {
		SoraWString info(L"Driver=OpenGL Version=");
		//int mav, miv, rev;
		info += s2ws((char*)glGetString(GL_VERSION));

		//glfwGetVersion(&mav, &miv, &rev);
		//info += int_to_str(mav); info+=".";
		//info += int_to_str(miv); info+=".";
		//info += int_to_str(rev); info+="\n";
		return info;
	}
	
	SoraShaderContext* SoraiOSGLRenderer::createShaderContext() {
		return new SoraGLSLShaderContext();
	}
	
	void SoraiOSGLRenderer::snapshot(const SoraString& file) {
	}
	
	void SoraiOSGLRenderer::setViewPoint(float, float, float) {
	}

	void SoraiOSGLRenderer::attachShaderContext(SoraShaderContext* context) {
		currShader = context;
	}
    
    void SoraiOSGLRenderer::setVerticalSync(bool flag) {
        SoraiOSInitializer::Instance()->setVerticalSync(flag);
    }

	void SoraiOSGLRenderer::detachShaderContext() {
		if(!currShader->detachShaderList())
			SoraCore::Instance()->log("SoraiOSGLRenderer: error detaching shader list");
		flush();
		currShader = 0;
	}

    void SoraiOSGLRenderer::onExtensionStateChanged(int32 extension, bool state, int32 param) {

    }
    
    void SoraiOSGLRenderer::renderRect(float32 x1, float32 y1, float32 x2, float32 y2, float32 fWidth, uint32 color, float32 z) {
		Rect4V rect;
		
		if(fWidth != y2-y1 && fWidth != x2-x1) {
			float rotAng = atan2f(y2-y1, x2-x1)-F_PI_4;
			
			rect.x1 = x1; rect.y1 = y1;
			rect.x2 = x1+fWidth*cosf(rotAng); rect.y2 = y1+fWidth*sinf(rotAng);
			rect.x4 = x2; rect.y4 = y2;
			rect.x3 = x2+fWidth*cosf(rotAng); rect.y3 = y2+fWidth*sinf(rotAng);
		} else {
			rect.x1 = x1; rect.y1 = y1;
			rect.x2 = x2; rect.y2 = y1;
			rect.x3 = x2; rect.y3 = y2;
			rect.x4 = x1; rect.y4 = y2;
		}
		sora::SoraQuad quad;
		
		quad.tex = NULL;
		
		quad.v[0].x   = rect.x1;
		quad.v[0].y   = rect.y1;
		quad.v[0].col = color;
		
		quad.v[1].x   = rect.x2;
		quad.v[1].y   = rect.y2;
		quad.v[1].col = color;
		
		quad.v[2].x   = rect.x3;
		quad.v[2].y   = rect.y3;
		quad.v[2].col = color;
		
		quad.v[3].x   = rect.x4;
		quad.v[3].y   = rect.y4;
		quad.v[3].col = color;
		
		int i;
		for (i = 0; i < 4; ++i) {
			quad.v[i].z = z;
		}
		
		quad.blend = BLEND_DEFAULT;
		
		renderQuad(quad);
	}
	
	void SoraiOSGLRenderer::renderBox(float32 x1, float32 y1, float32 x2, float32 y2, uint32 color, float32 z) {
		renderRect(x1, y1, x2, y1+1.f, 1.f, color, z);
		renderRect(x2, y1, x2+1.f, y2, 1.f, color, z);
		renderRect(x2, y2, x1, y2+1.f, 1.f, color, z);
		renderRect(x1, y2, x1+1.f, y1, 1.f, color, z);
	}
    
    void SoraiOSGLRenderer::setIcon(const SoraString& icon) {
        
    }
    
    void SoraiOSGLRenderer::setCursor(const SoraString& cursor) {
        
    }
} // namespace sora