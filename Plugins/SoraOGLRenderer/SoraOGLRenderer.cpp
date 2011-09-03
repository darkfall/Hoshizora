/*
 *  SoraOGLRenderer.cpp
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/23/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#include "SoraOGLRenderer.h"

#include "SoraLogger.h"

#include "SoraStringConv.h"
#include "SoraFileUtility.h"
#include "SoraMath.h"
#include "SoraRect4v.h"
#include "SoraColor.h"

#include "SoraCore.h"
#include "SoraInfiniteRendererCallback.h"
#include "SoraRenderSystemExtension.h"
#include "SoraOGLKeyPoll.h"

#undef SORA_USE_SHADER
#ifdef SORA_USE_SHADER
#include "SoraShader/SoraCGGLShader.h"
#endif

#include "glfw/GL/glfw.h"
#include "soil/SOIL.h"

#ifdef OS_OSX
#include "OSXIconWrapper.h"
#endif

#ifdef WIN32
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib, "glfw.lib")
#endif

const uint32 MAX_VERTEX_BUFFER = 9192;

static int mVertexCount = 0;
static GLfloat mVertices[MAX_VERTEX_BUFFER*3];
static GLfloat mUVs[MAX_VERTEX_BUFFER<<1];
static GLubyte mColors[MAX_VERTEX_BUFFER<<2];

namespace sora{
    
	SoraOGLRenderer::SoraOGLRenderer() {
		pCurTarget = 0;
		uGLShaderProgram = 0;

		bShaderAvailable = _glShaderCheck();

		mCurrTexture = -1;

		currShader = 0;
		iFrameStart = 1;
		CurBlendMode = 0;
        
        // FSAA params must be set by users before creating the main window
        SoraRenderSystemExtension::Instance()->registerExtension(SORA_EXTENSION_FSAA);
	}

	SoraOGLRenderer::~SoraOGLRenderer() {
	}

	void SoraOGLRenderer::shutdown() {
		std::list<SoraRenderTargetOG*>::iterator itt = liTargets.begin();
		while(itt != liTargets.end()) {
			delete (*itt);
			++itt;
		}
		liTargets.clear();
	}

    static void InitPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
    {
        GLfloat xmin, xmax, ymin, ymax;

        ymax = zNear * (GLfloat)tan(fovy * 3.1415962f / 360.0);
        ymin = -ymax;
        xmin = ymin * aspect;
        xmax = ymax * aspect;

        glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
    }

	void SoraOGLRenderer::_glInitialize() {
		glShadeModel(GL_SMOOTH);                    // Enables Smooth Shading
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearStencil(0);                          // clear stencil buffer

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);                       // The Type Of Depth Test To Do
		glClearDepth(1.f);                          // Depth Buffer Setup

        glEnable(GL_SCISSOR_TEST);
        //glDepthMask(GL_FALSE);

 //       glEnable(GL_CULL_FACE);
//		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//		glEnable(GL_COLOR_MATERIAL);

     //   InitPerspective(60, (float)_oglWindowInfo.width / _oglWindowInfo.height, 0.f, 1.f);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations

		glDisable(GL_DITHER);
		glDisable(GL_FOG);
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		glDisable(GL_TEXTURE_2D); 
        
		SoraString info("OpenGL Version=");
		info += (char*)glGetString(GL_VERSION);
		SET_ENV_STRING("RENDERER_DRIVER", info);
    }

	void SoraOGLRenderer::_glBeginFrame() {
	}

	void SoraOGLRenderer::_glEndFrame() {
		glFlush();
		glfwSwapBuffers();
	}

	void SoraOGLRenderer::beginFrame() {
		iFrameStart = 1;
	}

	void SoraOGLRenderer::endFrame() {
		_glEndFrame();
	}

	void SoraOGLRenderer::_glSetProjectionMatrix(int32 w, int32 h) {
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);

		//Clearing the projection matrix...
		glLoadIdentity();
		//Creating an orthoscopic view matrix going from -1 -> 1 in each
		//dimension on the screen (x, y, z).
		glOrtho(0.f, w, h, 0.f, -1.f, 1.f);
	}

	void SoraOGLRenderer::applyTransform() {
        if(!pCurTarget) {
            glViewport(0, 0,
                       _oglWindowInfo.width,
                       _oglWindowInfo.height);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.f,
                    _oglWindowInfo.width,
                    _oglWindowInfo.height
                    , 0.f, 1.f, -1.f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(_oglWindowInfo.x+_oglWindowInfo.dx, _oglWindowInfo.y+_oglWindowInfo.dy, 0.f); //Set Center Coodinates

            glRotatef(_oglWindowInfo.rot, -0.f, 0.f, 1.f);
            glScalef(_oglWindowInfo.hscale, _oglWindowInfo.vscale, 1.0f);//Transformation follows order scale->rotation->displacement
            glTranslatef(-_oglWindowInfo.x, -_oglWindowInfo.y, 0.f);


        } else {
            glViewport(0,  0,
                       pCurTarget->getWidth(),
                       pCurTarget->getHeight());
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0,
                    pCurTarget->getWidth(),
                    0
                    , pCurTarget->getHeight(), 1.f, -1.f);
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(_oglWindowInfo.x+_oglWindowInfo.dx, _oglWindowInfo.y+_oglWindowInfo.dy, 0.f); //Set Center Coodinates

            glRotatef(_oglWindowInfo.rot, -0.f, 0.f, 1.f);
            glScalef(_oglWindowInfo.hscale, _oglWindowInfo.vscale, 1.0f);//Transformation follows order scale->rotation->displacement
            glTranslatef(-_oglWindowInfo.x, -_oglWindowInfo.y, 0.f);

        }
	}

	void SoraOGLRenderer::_glBeginScene(uint32 color, ulong32 t, bool clear) {
		int32 width = _oglWindowInfo.width;
		int32 height = _oglWindowInfo.height;

		if(t) {
			pCurTarget = (SoraRenderTargetOG*)t;
			width = pCurTarget->getWidth();
			height = pCurTarget->getHeight();

            pCurTarget->attachToRender();
            setTransform(0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f);
			CurBlendMode = 0;
            glClearColor(CGETR(color)/255.0f, CGETG(color)/255.0f, CGETB(color)/255.0f, CGETA(color)/255.0f);
			
			if(clear)
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			else
				glClear(GL_DEPTH_BUFFER_BIT);
			
        } else {
            glClearColor(CGETR(color)/255.0f, CGETG(color)/255.0f, CGETB(color)/255.0f, CGETA(color)/255.0f);
            
            if(clear)
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            else
                glClear(GL_DEPTH_BUFFER_BIT);
        }

	}

	void SoraOGLRenderer::_glEndScene() {
		flush();
		if(pCurTarget != NULL) {
            glFlush();

            pCurTarget->detachFromRender();
            pCurTarget = 0;
            
            setTransform(0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f);
            setClipping();
        } else
            iFrameStart = 0;
		//else
		//	glfwSwapBuffers();

	}

	void SoraOGLRenderer::_glSetBlendMode(int32 blend) {
		if(blend != CurBlendMode)
			flush();
		
//		glDisable(GL_ALPHA_TEST);
		glEnable(GL_BLEND); // Enable Blending
		
		if((blend & BLEND_SRCALPHA) != (CurBlendMode & BLEND_SRCALPHA))
			if(blend & BLEND_SRCALPHA)
				glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);

		if((blend & BLEND_ALPHABLEND) != (CurBlendMode & BLEND_ALPHABLEND)) {
			if(blend & BLEND_ALPHABLEND) {
#ifndef OS_WIN32
                if(pCurTarget)
                    // alpha trick with FBO transparent background
                    // see http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=257628

                    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

                else 
#endif
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

	bool SoraOGLRenderer::update() {
#ifdef OS_OSX
        osx_activeCurrentCursor();
#endif
		return false;
	}

	void SoraOGLRenderer::start(SoraTimer* timer) {
		sora::g_timer = timer;
		_glInitialize();
		setTransform(0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f);

		while(true) {
			void_updateFrame();
		}
	}

	void SoraOGLRenderer::beginScene(uint32 color, ulong32 target, bool clear) {
		_glBeginScene(color, target, clear);
	}

	void SoraOGLRenderer::endScene() {
		currShader = 0;
		_glEndScene();
	}

	SoraWindowHandle SoraOGLRenderer::createWindow(SoraWindowInfoBase* windowInfo) {
		glfwInit();
        
        int32 FSAASamples = SoraRenderSystemExtension::Instance()->getExtensionParam(SORA_EXTENSION_FSAA);
        
        glfwOpenWindowHint(GLFW_FSAA_SAMPLES, FSAASamples);
        
		if(!glfwOpenWindow(windowInfo->getWindowWidth(), windowInfo->getWindowHeight()
                           , 8, 8, 8, 8, 16, 0, windowInfo->isWindowed()?GLFW_WINDOW:GLFW_FULLSCREEN)) {
            if(FSAASamples == 0) {
                glfwTerminate();
                THROW_SORA_EXCEPTION(SystemException, "Error initializing GLFW");
            } else {
                glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 0);
                if(!glfwOpenWindow(windowInfo->getWindowWidth(), windowInfo->getWindowHeight()
                                  , 8, 8, 8, 8, 16, 0, windowInfo->isWindowed()?GLFW_WINDOW:GLFW_FULLSCREEN)) {
                    glfwTerminate();
                    THROW_SORA_EXCEPTION(SystemException, "Error initializing GLFW");
                }
            }
        }
        if(glfwGetWindowParam(GLFW_FSAA_SAMPLES) != FSAASamples) {
            THROW_SORA_EXCEPTION(SystemException, vamssg("Error creating window with FSAA sample = %d", FSAASamples));
        }
        
		glfwSetWindowTitle(windowInfo->getWindowName().c_str());
		if(windowInfo->getWindowPosX() != 0.f && windowInfo->getWindowPosY() != 0.f)
			glfwSetWindowPos(windowInfo->getWindowPosX(), windowInfo->getWindowPosY());
		glfwSetWindowCloseCallback(int_exitFunc);
		glfwSetKeyCallback(glfwKeyCallback);

        mainWindow = windowInfo;
		_oglWindowInfo.width = windowInfo->getWindowWidth();
		_oglWindowInfo.height = windowInfo->getWindowHeight();
		windowWidth = _oglWindowInfo.width;
		windowHeight = _oglWindowInfo.height;

		_glInitialize();
		_glSetProjectionMatrix(windowInfo->getWindowWidth(), windowInfo->getWindowHeight());

		if(mainWindow->hideMouse())
			glfwDisable(GLFW_MOUSE_CURSOR);

		bFullscreen = !windowInfo->isWindowed();
        
        if(windowInfo->getIcon() != NULL)
            setIcon(mainWindow->getIcon());
        if(windowInfo->getCursor() != NULL)
            setCursor(mainWindow->getCursor());

		return (ulong32)windowInfo;

		return 0;
	}

	void SoraOGLRenderer::setWindowSize(int32 w, int32 h) {
		glfwSetWindowSize(w, h);
		
		_oglWindowInfo.width = w;
		_oglWindowInfo.height = h;
		windowWidth = _oglWindowInfo.width;
		windowHeight = _oglWindowInfo.height;
		
		_glSetProjectionMatrix(w, h);
	}

	void SoraOGLRenderer::setWindowTitle(const SoraWString& title) {
		glfwSetWindowTitle(ws2s(title).c_str());
	}

	void SoraOGLRenderer::setWindowPos(int32 px, int32 py) {
		glfwSetWindowPos(px, py);
	}

	void SoraOGLRenderer::setFullscreen(bool flag) {
		if(bFullscreen == flag)
			return;

		endScene();

		bFullscreen = flag;
		glfwCloseWindow();
		glfwOpenWindow(mainWindow->getWindowWidth(), mainWindow->getWindowHeight(),
					   8, 8, 8, 8, 16, 0,
					   bFullscreen==true?GLFW_FULLSCREEN:GLFW_WINDOW);
		glfwSetWindowTitle(mainWindow->getWindowName().c_str());
		glfwSetWindowCloseCallback(int_exitFunc);

		_glInitialize();
		_glSetProjectionMatrix(_oglWindowInfo.width, _oglWindowInfo.height);

		beginScene();
	}

	bool SoraOGLRenderer::isFullscreen() {
		return bFullscreen;
	}

	ulong32 SoraOGLRenderer::getVideoDeviceHandle() {
		return (ulong32)this;
	}

	int32 SoraOGLRenderer::_glTextureGetWidth(int32 tex, bool bOriginal) {
		GLint w;
		glBindTexture(GL_TEXTURE_2D, tex);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glBindTexture(GL_TEXTURE_2D, mCurrTexture);
		return w;
	}

	int32 SoraOGLRenderer::_glTextureGetHeight(int32 tex, bool bOriginal) {
		GLint h;
		glBindTexture(GL_TEXTURE_2D, tex);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
		glBindTexture(GL_TEXTURE_2D, mCurrTexture);
		return h;
	}

	void SoraOGLRenderer::bindTexture(SoraTexture* tex) {
        if(!tex) {
            mCurrTexture = 0;
            glBindTexture(GL_TEXTURE_2D, 0);
            return;
        }
		else if (mCurrTexture != tex->mTextureID) {
			flush();

			glBindTexture(GL_TEXTURE_2D, tex->mTextureID);
			mCurrTexture = tex->mTextureID;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}

	void SoraOGLRenderer::flush() {
		if (mVertexCount > 0) {
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, mVertices);
			glTexCoordPointer(2, GL_FLOAT, 0, mUVs);

			glColorPointer (4, GL_UNSIGNED_BYTE, 0, mColors);
			glDrawArrays(CurDrawMode, 0, mVertexCount);
			mVertexCount = 0;

        //    glFlush();
		}
	}

	SoraTexture* SoraOGLRenderer::createTexture(const SoraWString& sTexturePath, bool bMipmap) {
		// to do
		return 0;
	}

	// to do
	SoraTexture* SoraOGLRenderer::createTextureFromMem(void* ptr, ulong32 size, bool bMipmap) {
		ulong32  texid;
		int w, h, channels;

		texid = SOIL_load_OGL_texture_and_info_from_memory((const unsigned char* const)ptr,
														  size,
														  SOIL_LOAD_RGBA,
														  SOIL_CREATE_NEW_ID,
														  (bMipmap ? SOIL_FLAG_MIPMAPS : 0)
														  | /*SOIL_FLAG_POWER_OF_TWO | */SOIL_FLAG_TEXTURE_REPEATS/*| SOIL_FLAG_INVERT_Y*/,
														  &w, &h, &channels);

		SoraTexture* tex = new SoraTexture(texid,
										   _glTextureGetWidth(texid),
										   _glTextureGetHeight(texid),
										   w,
										   h);
		return tex;
	}

	SoraTexture* SoraOGLRenderer::createTextureWH(int w, int h) {
		GLuint texId;

		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		uint8* texData = new uint8[w*h*sizeof(uint32)];
		memset(texData, 255, w*h*sizeof(uint32));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		delete texData;

		return new SoraTexture(texId, w, h, w, h);
	}

	SoraTexture* SoraOGLRenderer::createTextureFromRawData(unsigned int* data, int32 w, int32 h) {
        GLuint texId;

        glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);

		if(!texId) {
			THROW_SORA_EXCEPTION(SystemException, "Error creating texture");
			return 0;
		}
		SoraTexture* tex = new SoraTexture(texId,
										   _glTextureGetWidth(texId),
										   _glTextureGetHeight(texId),
										   w,
										   h);
		return tex;
	}

	uint32* SoraOGLRenderer::textureLock(SoraTexture* ht) {
		ht->dataRef.texData = new uint32[ht->mOriginalWidth * ht->mOriginalHeight];
		memset(ht->dataRef.texData, 0, ht->mOriginalWidth * ht->mOriginalHeight);
		if(ht->dataRef.texData) {
            glEnable(GL_TEXTURE_2D);

            GLint PreviousTexture;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &PreviousTexture);

            glBindTexture(GL_TEXTURE_2D, ht->mTextureID);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, ht->dataRef.texData);
            if(glGetError() != GL_NO_ERROR) {
                delete ht->dataRef.texData;
                ht->dataRef.texData = 0;
                return NULL;
            }

            glBindTexture(GL_TEXTURE_2D, PreviousTexture);
			return (uint32*)ht->dataRef.texData;
		}
		return 0;
	}

	void SoraOGLRenderer::textureUnlock(SoraTexture* ht) {
		if(ht->dataRef.texData != NULL) {
			glEnable(GL_TEXTURE_2D);

            GLint PreviousTexture;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &PreviousTexture);

            glBindTexture(GL_TEXTURE_2D, ht->mTextureID);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ht->mOriginalWidth, ht->mOriginalHeight, GL_RGBA, GL_UNSIGNED_BYTE, ht->dataRef.texData);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, PreviousTexture);

			delete[] ht->dataRef.texData;
			ht->dataRef.texData = 0;
		}

	}

	void SoraOGLRenderer::releaseTexture(SoraTexture* tex) {
		glDeleteTextures(1, (const GLuint*)&tex->mTextureID);
		delete tex;
		tex = 0;
	}
	
	int32 SoraOGLRenderer::_modeToGLMode(int32 mode) {
		switch (mode) {
			case SORA_LINE:				return GL_LINE;
			case SORA_TRIANGLES:		return GL_TRIANGLES;
			case SORA_TRIANGLES_FAN:	return GL_TRIANGLE_FAN;
			case SORA_TRIANGLES_STRIP:	return GL_TRIANGLE_STRIP;
			case SORA_QUAD:				return GL_QUADS;
		}
		return GL_TRIANGLES;
	}

	void SoraOGLRenderer::renderTriple(SoraTriple& trip) {
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

		if(currShader)
			flush();
		if(!trip.tex)
			flush();
	}
	
	void SoraOGLRenderer::renderWithVertices(SoraTexture* tex, int32 blendMode,  SoraVertex* vertices, uint32 vsize, int32 mode) {		
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
		
		flush();
	}

	void SoraOGLRenderer::renderQuad(SoraQuad& quad) {
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
		
		if(currShader)
			flush();
		if(!quad.tex)
			flush();
	}

	bool SoraOGLRenderer::isActive() {
		return static_cast<bool>(glfwGetWindowParam(GLFW_ACTIVE));
	}

	void SoraOGLRenderer::setClipping(int32 x, int32 y, int32 w, int32 h) {
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
    
    void SoraOGLRenderer::setVerticalSync(bool flag) {
        if(flag)
            glfwSwapInterval(1); // to device refresh rate
        else
            glfwSwapInterval(0);
    }

    void SoraOGLRenderer::setViewPoint(float x, float y, float z) {
        _oglWindowInfo.x = x;
        _oglWindowInfo.y = y;
        _oglWindowInfo.z = z;
    }

	void SoraOGLRenderer::setTransform(float x, float y, float dx, float dy, float rot, float hscale, float vscale) {
		_oglWindowInfo.x		=	x;
		_oglWindowInfo.y		=	y;
		_oglWindowInfo.dx		=	dx;
		_oglWindowInfo.dy		=	dy;
		_oglWindowInfo.rot		=	RAD_DGR(rot);
		_oglWindowInfo.hscale	=	hscale!=0.f?hscale:1.f;
		_oglWindowInfo.vscale	=	vscale!=0.f?hscale:1.f;

        flush();
        applyTransform();
	}

	ulong32 SoraOGLRenderer::createTarget(int width, int height, bool zbuffer) {
		SoraRenderTargetOG* t = new SoraRenderTargetOG(width, height, zbuffer);
		liTargets.push_back((SoraRenderTargetOG*)t);
		return (ulong32)t;
	}

	void SoraOGLRenderer::freeTarget(ulong32 t) {
		SoraRenderTargetOG* pt = (SoraRenderTargetOG*)t;
		if(!pt) return;
		std::list<SoraRenderTargetOG*>::iterator itt = liTargets.begin();
		while(itt != liTargets.end()) {
			if((*itt) == pt) {
				delete pt;
				liTargets.erase(itt);
				break;
			}
			++itt;
		}
	}

	ulong32 SoraOGLRenderer::getTargetTexture(ulong32 t) {
		SoraRenderTargetOG* pt = (SoraRenderTargetOG*)t;
        assert(pt != NULL);

		return pt->getTexture();
	}

	bool SoraOGLRenderer::_glVersionCheck() {
		int majorVersion, minorVersion, rev;
		glfwGetVersion(&majorVersion, &minorVersion, &rev);
		return majorVersion >= 2;
	}

	bool SoraOGLRenderer::_glShaderCheck() {
		// to do
		// check extensions
		// return glfwExtensionSupported("GL_ARB_fragment_shade");
		return _glVersionCheck();
	}


	inline bool SoraOGLRenderer::_glCheckError() {
		return glGetError() != GL_NO_ERROR;
	}

	SoraWString SoraOGLRenderer::videoInfo() {
		SoraWString info(L"Driver=OpenGL Version=");
		info += s2ws((char*)glGetString(GL_VERSION));

		return info;
	}

    SoraShaderContext* SoraOGLRenderer::createShaderContext() {
#ifdef SORA_USE_SHADER
        return new SoraCGGLShaderContext;
#endif
		return NULL;
    }

	void SoraOGLRenderer::attachShaderContext(SoraShaderContext* context) {
		flush();
		
		currShader = context;
		if(!currShader->attachShaderList()) {
			log_mssg("SoraOGLRenderer: Error attaching shader list");
		}
	}

	void SoraOGLRenderer::detachShaderContext() {
		if(!currShader)
			return;
		
		if(!currShader->detachShaderList())
			SoraCore::Instance()->log("SoraOGLRenderer: Error detaching shader list");
		flush();
		currShader = 0;
	}

    void SoraOGLRenderer::snapshot(const SoraString& path) {
        SOIL_save_screenshot(path.c_str(), SOIL_SAVE_TYPE_BMP, 0, 0, SORA->getScreenWidth(), SORA->getScreenHeight());
    }

    void SoraOGLRenderer::setIcon(const SoraString& icon) {
#ifdef OS_OSX
        osx_setDockIcon(icon);
#endif // OS_OSX
    }
    
    void SoraOGLRenderer::setCursor(const SoraString& cursor) {
#ifdef OS_OSX
        osx_setAppCursor(cursor);
#endif
    }
    
    void SoraOGLRenderer::onExtensionStateChanged(int32 extension, bool state, int32 param) {
        // multisampling
#ifndef OS_WIN32
        if(extension == SORA_EXTENSION_FSAA) {
            if(state) {
                glEnable(GL_MULTISAMPLE);
            } else {
                glDisable(GL_MULTISAMPLE);
            }
        }
#endif // OS_WIN32
    }
    
    void SoraOGLRenderer::renderLine(float x1, float y1, float x2, float y2, uint32 color, float z) {
		sora::SoraQuad quad;
		
		quad.tex = NULL;
		
        if(abs(x2-x1) == 1.f || abs(y2-y1) == 1.f) {
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
        } else {
            quad.v[0].x   = x1-0.5f;
            quad.v[0].y   = y1-0.5f;
            quad.v[0].col = color;
            
            quad.v[1].x   = x1+0.5f;
            quad.v[1].y   = y1+0.5f;
            quad.v[1].col = color;
            
            quad.v[2].x   = x2-0.5f;
            quad.v[2].y   = y2-0.5f;
            quad.v[2].col = color;
            
            quad.v[3].x   = x2+0.5f;
            quad.v[3].y   = y2+0.5f;
            quad.v[3].col = color;
        }
		
		int i;
		for (i = 0; i < 4; ++i) {
			quad.v[i].z = z;
		}
		
		quad.blend = BLEND_DEFAULT;
		
		renderQuad(quad);
	}
    
    void SoraOGLRenderer::fillBox(float x1, float y1, float x2, float y2, uint32 color, float z) {
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
	
	void SoraOGLRenderer::renderBox(float x1, float y1, float x2, float y2, uint32 color, float z) {
		renderLine(x1, y1, x2, y1+1.f, color, z);
		renderLine(x2, y1, x2+1.f, y2, color, z);
		renderLine(x2, y2, x1, y2+1.f, color, z);
		renderLine(x1, y2, x1+1.f, y1, color, z);
	}
    
    
#ifdef SORA_AUTOMATIC_REGISTER
    SORA_STATIC_RUN_CODE(SoraCore::Instance()->registerRenderSystem(new SoraOGLRenderer()));
#endif


} // namespace sora
