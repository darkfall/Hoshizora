/*
 *  SoraOGLRenderer.cpp
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoraOGLRenderer.h"

#include "Debug/SoraInternalLogger.h"

#include "SoraStringConv.h"
#include "SoraFileUtility.h"
#include "SoraMath.h"
#include "Rect4V.h"
#include "SoraColor.h"

#include "SoraCore.h"
#include "SoraInfiniteRendererCallback.h"

#include "SoraOGLKeyPoll.h"
#include "SoraShader/SoraCGGLShader.h"

#include "glfw/GL/glfw.h"
#include "soil/SOIL.h"

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
	}

	SoraOGLRenderer::~SoraOGLRenderer() {
	//	shutdown();
	}

	void SoraOGLRenderer::shutdown() {
		std::list<SoraRenderTarget*>::iterator itt = liTargets.begin();
		while(itt != liTargets.end()) {
			delete (*itt);
			++itt;
		}
		liTargets.clear();

		//delete mainWindow;
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
        
        glDepthMask(GL_FALSE);
        
		glEnable(GL_CULL_FACE);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);

     //   InitPerspective(60, (float)_oglWindowInfo.width / _oglWindowInfo.height, 0.f, 1.f);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations
		
		glDisable(GL_DITHER);
		glDisable(GL_FOG);
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		glDisable(GL_TEXTURE_2D);
    }

	void SoraOGLRenderer::_glBeginFrame() {
	}

	void SoraOGLRenderer::_glEndFrame() {
		glFlush();
		glfwSwapBuffers();
	}
	
	void SoraOGLRenderer::beginFrame() {
		applyTransform();
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
            glOrtho(0, 
                    pCurTarget->getWidth(), 
                    0
                    , pCurTarget->getHeight(), -1, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(0.f, 0.f, 0.f); //Set Center Coodinates
            glRotatef(0.f, -0.f, 0.f, 1.f);
            glScalef(1.f, 1.f, 1.0f);//
       
            glTranslatef(0.f, 0.f, 0.f);
        }
	}

	void SoraOGLRenderer::_glBeginScene(ulong32 color, ulong32 t) {
		int32 width = _oglWindowInfo.width;
		int32 height = _oglWindowInfo.height;
		if(t) {
			pCurTarget = (SoraRenderTargetOG*)t;
			width = pCurTarget->getWidth();
			height = pCurTarget->getHeight();
            
            pCurTarget->attachToRender();
            applyTransform();
            glClearColor((float)(color>>24&0xFF)/0xff, (float)(color>>16&0xFF)/0xff, (float)(color>>8&0xFF)/0xff, (float)(color&0xFF)/0xff);
        } else {
            if(iFrameStart) {
                glClearColor((float)(color>>24&0xFF)/0xff, (float)(color>>16&0xFF)/0xff, (float)(color>>8&0xFF)/0xff, (float)(color&0xFF)/0xff);
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void SoraOGLRenderer::_glEndScene() {
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

	void SoraOGLRenderer::_glSetBlendMode(int32 blend) {
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
                glDepthMask(GL_TRUE);
            } else {
                glDepthMask(GL_FALSE);
            }
		}

		if((blend & BLEND_COLORADD) != (CurBlendMode & BLEND_COLORADD)) {
			if(blend & BLEND_COLORADD) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
			else glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}

		CurBlendMode = blend;
	}

	bool SoraOGLRenderer::update() {
		clearPoll();
		return false;
	}

	void SoraOGLRenderer::start(SoraTimer* timer) {
		sora::g_timer = timer;
		_glInitialize();
		setTransform(0, 0, 0, 0, 0, 1, 1);

		while(true) {
			void_updateFrame();
		}
	}

	void SoraOGLRenderer::beginScene(ulong32 color, ulong32 target) {
		_glBeginScene(color, target);
	}

	void SoraOGLRenderer::endScene() {
		currShader = 0;
		_glEndScene();
	}

	SoraWindowHandle SoraOGLRenderer::createWindow(SoraWindowInfoBase* windowInfo) {
		glfwInit();
		glfwOpenWindow(windowInfo->getWindowWidth(), windowInfo->getWindowHeight()
					   , 8, 8, 8, 8, 16, 0, windowInfo->isWindowed()?GLFW_WINDOW:GLFW_FULLSCREEN);
		glfwSetWindowTitle(windowInfo->getWindowName().c_str());
		if(windowInfo->getWindowPosX() != 0.f && windowInfo->getWindowPosY() != 0.f)
			glfwSetWindowPos(windowInfo->getWindowPosX(), windowInfo->getWindowPosY());
		glfwSetWindowCloseCallback(int_exitFunc);
		glfwSetKeyCallback(glfwKeyCallback);

        mainWindow = windowInfo;
		_oglWindowInfo.width = windowInfo->getWindowWidth();
		_oglWindowInfo.height = windowInfo->getWindowHeight();
        
		_glInitialize();
		_glSetProjectionMatrix(windowInfo->getWindowWidth(), windowInfo->getWindowHeight());

		if(mainWindow->hideMouse())
			glfwDisable(GLFW_MOUSE_CURSOR);

		bFullscreen = windowInfo->isWindowed();
		windowInfo->init();

		return (ulong32)windowInfo;

		return 0;
	}

	void SoraOGLRenderer::setWindowSize(int32 w, int32 h) {
		glfwSetWindowSize(w, h);
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

	int32 SoraOGLRenderer::_glTextureGetWidth(ulong32 tex, bool bOriginal) {
		GLint w;
		glBindTexture(GL_TEXTURE_2D, tex);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glBindTexture(GL_TEXTURE_2D, mCurrTexture);
		return w;
	}

	int32 SoraOGLRenderer::_glTextureGetHeight(ulong32 tex, bool bOriginal) {
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
		if (mCurrTexture != tex->mTextureID) {
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
			glDrawArrays(GL_QUADS, 0, mVertexCount);
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
		
		if(!texid) {
			throw SORA_EXCEPTION("Error loading texture");
		}

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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

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
			throw SORA_EXCEPTION("Error creating texture");
			return 0;
		}
		SoraTexture* tex = new SoraTexture(texId,
										   _glTextureGetWidth(texId),
										   _glTextureGetHeight(texId),
										   w,
										   h);
		return tex;
	}

	ulong32* SoraOGLRenderer::textureLock(SoraTexture* ht) {
		ht->dataRef.texData = new ulong32[ht->mOriginalWidth * ht->mOriginalHeight];
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
			return (ulong32*)ht->dataRef.texData;
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

			delete ht->dataRef.texData;
			ht->dataRef.texData = 0;
		}

	}

	void SoraOGLRenderer::releaseTexture(SoraTexture* tex) {
		glDeleteTextures(1, (const GLuint*)&tex->mTextureID);
		delete tex;
		tex = 0;
	}

	void SoraOGLRenderer::renderTriple(SoraTriple& trip) {
		if(currShader != NULL) {
			flush();
		}

		glEnable(GL_TEXTURE_2D); // Enable Texture Mapping
		if(trip.tex) {
            
			//glBindTexture(GL_TEXTURE_2D, quad.tex->mTextureID);
			bindTexture(trip.tex);			
		} else {
            flush();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
		_glSetBlendMode(trip.blend);

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
				SoraCore::Instance()->log("SoraOGLRenderer: error attaching shader list");
			flush();
		}
		
		if(!trip.tex)
			flush();

	}

	void SoraOGLRenderer::renderQuad(SoraQuad& quad) {
		if(currShader != NULL) {
			flush();
		}

		glEnable(GL_TEXTURE_2D); // Enable Texture Mapping
		if(quad.tex) {

			//glBindTexture(GL_TEXTURE_2D, quad.tex->mTextureID);
			bindTexture(quad.tex);			
		} else {
            flush();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
		_glSetBlendMode(quad.blend);

		//TODO: insert code here
		GLfloat verteces[12] = {
			quad.v[3].x, quad.v[3].y, quad.v[3].z,
			quad.v[2].x, quad.v[2].y, quad.v[2].z,
			quad.v[1].x, quad.v[1].y, quad.v[1].z,
			quad.v[0].x, quad.v[0].y, quad.v[0].z,
		};

		GLfloat texCoords[8] = {
			quad.v[3].tx, quad.v[3].ty,
			quad.v[2].tx, quad.v[2].ty,
			quad.v[1].tx, quad.v[1].ty,
			quad.v[0].tx, quad.v[0].ty,
		};
		GLubyte colors[16] = {
			(GLubyte)CGETR(quad.v[3].col), (GLubyte)CGETG(quad.v[3].col), (GLubyte)CGETB(quad.v[3].col), (GLubyte)CGETA(quad.v[3].col),
			(GLubyte)CGETR(quad.v[2].col), (GLubyte)CGETG(quad.v[2].col), (GLubyte)CGETB(quad.v[2].col), (GLubyte)CGETA(quad.v[2].col),
			(GLubyte)CGETR(quad.v[1].col), (GLubyte)CGETG(quad.v[1].col), (GLubyte)CGETB(quad.v[1].col), (GLubyte)CGETA(quad.v[1].col),
			(GLubyte)CGETR(quad.v[0].col), (GLubyte)CGETG(quad.v[0].col), (GLubyte)CGETB(quad.v[0].col), (GLubyte)CGETA(quad.v[0].col),
		};

		if (mVertexCount >= MAX_VERTEX_BUFFER-4)
			flush();

		int u = 0;
		int idx = 0;
		int cdx = 0;
		for (int i=0;i<4;i++) {
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
				SoraCore::Instance()->log("SoraOGLRenderer: error attaching shader list");
			flush();
		}
		
		if(!quad.tex)
			flush();

		/*

		glVertexPointer(3, GL_FLOAT, 0, verteces);
		glEnableClientState(GL_VERTEX_ARRAY);

		if(quad.tex) {
			glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
		glEnableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);*/

		//glDisable(GL_TEXTURE_2D); // Enable Texture Mapping
		//glDisable(GL_BLEND); // Enable Texture Mapping
		//glBindTexture(GL_TEXTURE_2D, 0);
	}

	bool SoraOGLRenderer::isActive() {
		//return pHGE->System_GetState(HGE_ACTIVE);
		return true;
	}

	void SoraOGLRenderer::renderRect(float32 x1, float32 y1, float32 x2, float32 y2, float32 fWidth, DWORD color, float32 z) {
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

	void SoraOGLRenderer::setClipping(int32 x, int32 y, int32 w, int32 h) {
		int32 width = w;
		int32 height = h;
		if(pCurTarget) {
			width = pCurTarget->getWidth();
			height = pCurTarget->getHeight();
		} else {
			width = _oglWindowInfo.width;
			height = _oglWindowInfo.height;
		}
		
		if(w==0 && h==0) {
			_oglWindowInfo.width = width;
			_oglWindowInfo.height = height;
			_oglWindowInfo.x = x;
			_oglWindowInfo.y = y;
		} else {
			if(x<0) { w+=x; x=0; }
			if(y<0) { h+=y; y=0; }
			
			if(x+w > width) w=width-x;
			if(y+h > height) h=height-y;
			
			_oglWindowInfo.width = width;
			_oglWindowInfo.height = height;
			_oglWindowInfo.x = x;
			_oglWindowInfo.y = y;
		}
		
		applyTransform();
	}

	void SoraOGLRenderer::setTransformWindowSize(float32 w, float32 h) {
		_oglWindowInfo.width = w!=0?w:mainWindow->getWindowWidth();
		_oglWindowInfo.height = h!=0?h:mainWindow->getWindowHeight();
		applyTransform();

	}
    
    void SoraOGLRenderer::setViewPoint(float32 x, float32 y, float32 z) {
        _oglWindowInfo.x = x;
        _oglWindowInfo.y = y;
        _oglWindowInfo.z = z;
    }

	void SoraOGLRenderer::setTransform(float32 x, float32 y, float32 dx, float32 dy, float32 rot, float32 hscale, float32 vscale) {
		_oglWindowInfo.x		=	x;
		_oglWindowInfo.y		=	y;
		_oglWindowInfo.dx		=	dx;
		_oglWindowInfo.dy		=	dy;
		_oglWindowInfo.rot		=	rot;
		_oglWindowInfo.hscale	=	hscale!=0.f?hscale:1.f;
		_oglWindowInfo.vscale	=	vscale!=0.f?hscale:1.f;
        
        applyTransform();
	}

	ulong32 SoraOGLRenderer::createTarget(int width, int height, bool zbuffer) {
		SoraRenderTargetOG* t = new SoraRenderTargetOG(width, height, zbuffer);
		liTargets.push_back((SoraRenderTarget*)t);
		return (ulong32)t;
	}

	void SoraOGLRenderer::freeTarget(ulong32 t) {
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

	ulong32 SoraOGLRenderer::getTargetTexture(ulong32 t) {
        assert(t != NULL);
		SoraRenderTargetOG* pt = (SoraRenderTargetOG*)t;
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
		//int mav, miv, rev;
		info += s2ws((char*)glGetString(GL_VERSION));

		//glfwGetVersion(&mav, &miv, &rev);
		//info += int_to_str(mav); info+=".";
		//info += int_to_str(miv); info+=".";
		//info += int_to_str(rev); info+="\n";
		return info;
	}
    
    SoraShaderContext* SoraOGLRenderer::createShaderContext() {
        return new SoraCGGLShaderContext;
    }

	void SoraOGLRenderer::attachShaderContext(SoraShaderContext* context) {
		currShader = context;
	}

	void SoraOGLRenderer::detachShaderContext() {
		if(!currShader->detachShaderList())
			SoraCore::Instance()->log("SoraOGLRenderer: error detaching shader list");
		flush();
		currShader = 0;
	}
    
    void SoraOGLRenderer::snapshot(const SoraString& path) {
        SOIL_save_screenshot(path.c_str(), SOIL_SAVE_TYPE_BMP, 0, 0, SORA->getScreenWidth(), SORA->getScreenHeight());
    }

} // namespace sora
