/*
 *  SoraOGLESRenderer.cpp
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/23/10.
 *  Copyright 2010 Griffin Bu. All rights reserved.
 *
 */

#include "SoraOGLESRenderer.h"

#include "SoraStringConv.h"
#include "SoraInternalLogger.h"
#include "SoraFileUtility.h"
#include "SoraMath.h"
#include "Rect4V.h"
#include "SoraColor.h"

#include "SoraCore.h"
#include "SoraInfiniteRendererCallback.h"

#include "SoraiOSInitializer.h"
#include "SOIL/SOIL.h"
const uint32 MAX_VERTEX_BUFFER = 2048;

static int mVertexCount = 0;
static GLfloat mVertices[MAX_VERTEX_BUFFER*3];
static GLfloat mUVs[MAX_VERTEX_BUFFER<<1];
static GLubyte mColors[MAX_VERTEX_BUFFER<<2];

namespace sora{

	SoraOGLESRenderer::SoraOGLESRenderer() {
		//pCurTarget = 0;
		uGLShaderProgram = 0;
		
		bShaderAvailable = _glShaderCheck();
		
		mCurrTexture = -1;

		currShader = 0;
	}

	SoraOGLESRenderer::~SoraOGLESRenderer() {
	//	shutdown();
	}

	void SoraOGLESRenderer::shutdown() {
	/*	std::list<SoraRenderTarget*>::iterator itt = liTargets.begin();
		while(itt != liTargets.end()) {
			delete (*itt);
			++itt;
		}
		liTargets.clear();*/
		
		//delete mainWindow;
	}

	static void InitPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
	{
		GLfloat xmin, xmax, ymin, ymax;
		
		ymax = zNear * (GLfloat)tan(fovy * 3.1415962f / 360.0);
		ymin = -ymax;
		xmin = ymin * aspect;
		xmax = ymax * aspect;
		
		glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
	}
	
	void SoraOGLESRenderer::_to2D() {
		glEnable				(GL_BLEND);
		glEnable				(GL_TEXTURE_2D);
		glEnable				(GL_DEPTH_TEST);
		glDisable				(GL_CULL_FACE);
		glDisable				(GL_LIGHTING);
		glDisableClientState	(GL_NORMAL_ARRAY);
	}
	
	void SoraOGLESRenderer::_to3D() {		
		glViewport          (0, 0, _oglWindowInfo.width, _oglWindowInfo.height);
		glScissor           (0, 0, _oglWindowInfo.width, _oglWindowInfo.height);
		
		glMatrixMode        (GL_PROJECTION);
		glLoadIdentity      ();
		float aspect = (float)_oglWindowInfo.width/(float)_oglWindowInfo.height;
		InitPerspective(60.f, aspect, 0.1f, 1000.f);
		
		glMatrixMode        (GL_MODELVIEW);
		glLoadIdentity      ();
		
		glEnable			(GL_CULL_FACE);
		glEnable			(GL_DEPTH_TEST);
	}
	
	void SoraOGLESRenderer::_glInitialize() {
		glMatrixMode        (GL_MODELVIEW);
		glLoadIdentity      ();
		
		glHint              (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		
		glShadeModel        (GL_SMOOTH);
		glDisable           (GL_DITHER);
			
		glMatrixMode        (GL_PROJECTION);
		glLoadIdentity      ();
		
	//	float aspect = (float)_oglWindowInfo.width/(float)_oglWindowInfo.height;
	//	InitPerspective(60.f, aspect, 0.1f, 1000.f);
		
		_to2D();
	}
	
	void SoraOGLESRenderer::_glBeginFrame() {
	}
	
	void SoraOGLESRenderer::_glEndFrame() {
	}
	
	void SoraOGLESRenderer::_glSetProjectionMatrix(int32 w, int32 h) {
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		
		//Clearing the projection matrix...
		glLoadIdentity();
		//Creating an orthoscopic view matrix going from -1 -> 1 in each
		//dimension on the screen (x, y, z). 
		glOrthof(0, _oglWindowInfo.width, _oglWindowInfo.height, 0, -1, 1);
	}
	
	void SoraOGLESRenderer::applyTransform() {
		glViewport(0, 0, _oglWindowInfo.width, _oglWindowInfo.height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrthof(0, _oglWindowInfo.width, _oglWindowInfo.height, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(_oglWindowInfo.x-_oglWindowInfo.dx, _oglWindowInfo.y-_oglWindowInfo.dy, 0.f); //Set Center Coodinates
		glRotatef(_oglWindowInfo.rot, -0.f, 0.f, 1.f);
		//glRotatef(DGR_RAD(20), -1.f, 1.f, 1.f);
		glScalef(_oglWindowInfo.hscale*2.1, _oglWindowInfo.vscale, 1.0f);//Transformation follows order scale->rotation->displacement
		
		glTranslatef(-_oglWindowInfo.x, -_oglWindowInfo.y, 0.0f);
	}
	
	void SoraOGLESRenderer::_glBeginScene(ulong32 color, ulong32 t) {
		//glDisable(GL_BLEND);
		//glDisable(GL_DEPTH_TEST);
		glDisable(GL_DITHER);
		glDisable(GL_FOG);
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		//glDisable(GL_LOGIC_OP);
		glDisable(GL_STENCIL_TEST);
		//glDisable(GL_TEXTURE_2D);
	
		glEnable(GL_DEPTH_BUFFER_BIT);
		glDepthMask(0);
	
		//int32 width = _oglWindowInfo.width;
		//int32 height = _oglWindowInfo.height;
		/*if(t) {
			pCurTarget = (SoraRenderTargetOG*)t;
			width = pCurTarget->w;
			height = pCurTarget->h;
		} else pCurTarget = 0;
		
		if(pCurTarget) pCurTarget->attachToRender();*/
		//applyTransform();
		
		glClearColor((float)(color>>24&0xFF)/0xff, (float)(color>>16&0xFF)/0xff, (float)(color>>8&0xFF)/0xff, (float)(color&0xFF)/0xff);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void SoraOGLESRenderer::_glEndScene() {
		glFlush();
		/*if(pCurTarget)
			pCurTarget->detachFromRender();
		else
			glfwSwapBuffers();*/
		
		mCurrTexture = 0;
		//glBindTexture(GL_TEXTURE_2D, 0);
	}

	void SoraOGLESRenderer::_glSetBlendMode(int32 blend) {
		glDisable(GL_ALPHA_TEST);
		glEnable(GL_BLEND); // Enable Blending
		
		if((blend & BLEND_ALPHABLEND) != (CurBlendMode & BLEND_ALPHABLEND)) {
			if(blend & BLEND_ALPHABLEND) 
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);      //Alpha blending
			else 
				glBlendFunc(GL_SRC_ALPHA, GL_ONE); //Addictive  
		}
		
		if((blend & BLEND_ZWRITE) != (CurBlendMode & BLEND_ZWRITE)) {
			if(blend & BLEND_ZWRITE) glDepthMask(GL_TRUE);
			else glDepthMask(GL_FALSE);
		}       
		
		/*if((blend & BLEND_COLORADD) != (CurBlendMode & BLEND_COLORADD)) {
			if(blend & BLEND_COLORADD) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD); 
			else glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
		}*/
		
		CurBlendMode = blend;
	}
	
	bool SoraOGLESRenderer::update() {
		//clearPoll();
		return false;
	}
	
	void SoraOGLESRenderer::start(SoraTimer* timer) {
		SORA_IOS->setTimer(timer);
	}

	void SoraOGLESRenderer::beginScene(ulong32 color, ulong32 target) {
		_glBeginScene(color, target);
	}

	void SoraOGLESRenderer::endScene() {
		currShader = 0;
		flush();
		_glEndScene();
	}

	SoraWindowHandle SoraOGLESRenderer::createWindow(SoraWindowInfoBase* windowInfo) {
		mainWindow = windowInfo;
		_oglWindowInfo.width = windowInfo->getWindowWidth();
		_oglWindowInfo.height = windowInfo->getWindowHeight();
#ifdef _DEBUG
		printf("%d, %d\n", _oglWindowInfo.width, _oglWindowInfo.height);
#endif
		
		_glInitialize();
		//_glSetProjectionMatrix(_oglWindowInfo.width, _oglWindowInfo.height);
		applyTransform();
		
		bFullscreen = /*windowInfo->isWindowed()*/ true;
		windowInfo->init();		

		return (ulong32)windowInfo;
		
		return 0;
	}
	
	void SoraOGLESRenderer::setWindowSize(int32 w, int32 h) {
	//	glfwSetWindowSize(w, h);
	}
	
	void SoraOGLESRenderer::setWindowTitle(const SoraWString& title) {
	//	glfwSetWindowTitle(ws2s(title).c_str());
	}
	
	void SoraOGLESRenderer::setWindowPos(int32 px, int32 py) {
	//	glfwSetWindowPos(px, py);
	}
	
	void SoraOGLESRenderer::setFullscreen(bool flag) {
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
	
	bool SoraOGLESRenderer::isFullscreen() {
		return bFullscreen;
	}
	
	ulong32 SoraOGLESRenderer::getVideoDeviceHandle() {
		return (ulong32)this;
	}
	
	int32 SoraOGLESRenderer::_glTextureGetWidth(ulong32 tex, bool bOriginal) {
		/*GLint w; 
		glBindTexture(GL_TEXTURE_2D, tex);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glBindTexture(GL_TEXTURE_2D, mCurrTexture);
		return w;*/
		return 0;
	}
	
	int32 SoraOGLESRenderer::_glTextureGetHeight(ulong32 tex, bool bOriginal) {
		/*GLint h; 
		glBindTexture(GL_TEXTURE_2D, tex);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
		glBindTexture(GL_TEXTURE_2D, mCurrTexture);
		return h;*/
		return 0;
	}
	
	void SoraOGLESRenderer::bindTexture(SoraTexture* tex) {
		if (mCurrTexture != tex->mTextureID) {
			flush();
			
			glBindTexture(GL_TEXTURE_2D, tex->mTextureID);
			mCurrTexture = tex->mTextureID;

		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			
			/*if (mLinearFiltering) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/
		}
	}
	
	void SoraOGLESRenderer::flush() {
		if (mVertexCount > 0) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, mVertices);
			glTexCoordPointer(2, GL_FLOAT, 0, mUVs);
			glColorPointer (4, GL_UNSIGNED_BYTE, 0, mColors);
			glDrawArrays(GL_TRIANGLES, 0, mVertexCount);
			mVertexCount = 0;
		}
	}

	SoraTexture* SoraOGLESRenderer::createTexture(const SoraWString& sTexturePath, bool bMipmap) {
		//return mTextureCreate(ws2s(sTexturePath).c_str(), false);
		// to do
		return 0;
	}
	
	// to do
	SoraTexture* SoraOGLESRenderer::createTextureFromMem(void* ptr, ulong32 size, bool bMipmap) {
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
		
		printf("%d %d %d\n", w, h, channels);
		
		SoraTexture* tex = new SoraTexture(texid,
										   w,
										   h,
										   w,
										   h);
		return tex;
	}

	SoraTexture* SoraOGLESRenderer::createTextureWH(int w, int h) {
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
	
	SoraTexture* SoraOGLESRenderer::createTextureFromRawData(unsigned int* data, int32 w, int32 h) {
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
		SoraTexture* tex = new SoraTexture(texid,
										   _glTextureGetWidth(texid),
										   _glTextureGetHeight(texid),
										   w,
										   h);
		return tex;
	}

	ulong32* SoraOGLESRenderer::textureLock(SoraTexture* ht, bool bReadOnly, uint32 x, uint32 y, uint32 w, uint32 h) {
		return 0;		
	}

	void SoraOGLESRenderer::textureUnlock(SoraTexture* h) {
	/*	if(h->dataRef.texData) {
			glEnable(GL_TEXTURE_2D);
			bindTexture(h);
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, h->mTextureWidth, h->mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_INT, h->dataRef.texData);
		//	glTexSubImage2D(GL_TEXTURE_2D, 0, h->dataRef.x, h->dataRef.y, h->dataRef.w, h->dataRef.h, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, h->dataRef.texData);
			printf("%d, %d, %d, %d, %d\n", glGetError(), GL_INVALID_ENUM, GL_INVALID_OPERATION, GL_INVALID_VALUE, GL_NO_ERROR);
			
		//	printf("%d, %d, %d, %d\n", h->dataRef.texData[0], h->dataRef.texData[1], h->dataRef.texData[2], h->dataRef.texData[3]);
			//free(h->dataRef.texData);
			//h->dataRef.texData = 0;
		}*/
		
	}

	void SoraOGLESRenderer::releaseTexture(SoraTexture* tex) {
		glDeleteTextures(1, (const GLuint*)&tex->mTextureID);
	}
	
	void SoraOGLESRenderer::renderTriple(SoraTriple& trip) {
		if(currShader != NULL) {
			flush();
		}
		
		glEnable(GL_TEXTURE_2D);
		if(trip.tex)
			bindTexture(trip.tex);
		

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
				SoraCore::Instance()->log("SoraOGLESRenderer: error attaching shader list");
			flush();
		}
			
	}
	
	void SoraOGLESRenderer::renderQuad(SoraQuad& quad) {
		if(currShader != NULL) {
			flush();
		}

		glEnable(GL_TEXTURE_2D);
		if(quad.tex) 
			bindTexture(quad.tex);
	
		_glSetBlendMode(quad.blend);		
		GLfloat verteces[18] = {
			quad.v[0].x, quad.v[0].y, quad.v[0].z,
			quad.v[1].x, quad.v[1].y, quad.v[0].z,
			quad.v[3].x, quad.v[3].y, quad.v[0].z,
			quad.v[2].x, quad.v[2].y, quad.v[0].z,
			quad.v[3].x, quad.v[2].y, quad.v[0].z,
			quad.v[1].x, quad.v[1].y, quad.v[0].z
		};

		GLfloat texCoords[12] = {
			quad.v[0].tx, quad.v[0].ty,
			quad.v[1].tx, quad.v[1].ty,
			quad.v[3].tx, quad.v[3].ty,
			quad.v[2].tx, quad.v[2].ty,
			quad.v[3].tx, quad.v[2].ty,
			quad.v[1].tx, quad.v[1].ty
		};
		
		/*for(int i=0; i<6; ++i) {
			for(int j=0; j<3; ++j)
				printf("%d=%f, ", i*3+j, verteces[i*3+j]);
			printf("\n");
		}
		*/
	/*	GLubyte colors[24] = {
			(GLubyte)CGETR(quad.v[3].col), (GLubyte)CGETG(quad.v[3].col), (GLubyte)CGETB(quad.v[3].col), (GLubyte)CGETA(quad.v[3].col),
			(GLubyte)CGETR(quad.v[2].col), (GLubyte)CGETG(quad.v[2].col), (GLubyte)CGETB(quad.v[2].col), (GLubyte)CGETA(quad.v[2].col),
			(GLubyte)CGETR(quad.v[1].col), (GLubyte)CGETG(quad.v[1].col), (GLubyte)CGETB(quad.v[1].col), (GLubyte)CGETA(quad.v[1].col),
			(GLubyte)CGETR(quad.v[0].col), (GLubyte)CGETG(quad.v[0].col), (GLubyte)CGETB(quad.v[0].col), (GLubyte)CGETA(quad.v[0].col),
			(GLubyte)CGETR(quad.v[1].col), (GLubyte)CGETG(quad.v[1].col), (GLubyte)CGETB(quad.v[1].col), (GLubyte)CGETA(quad.v[1].col),
			(GLubyte)CGETR(quad.v[0].col), (GLubyte)CGETG(quad.v[0].col), (GLubyte)CGETB(quad.v[0].col), (GLubyte)CGETA(quad.v[0].col)
		};
				*/
		if (mVertexCount >= MAX_VERTEX_BUFFER-6)
			flush();
		
		int u = 0;
		int idx = 0;
		GLubyte colors[4] = {
			(GLubyte)CGETR(quad.v[0].col), (GLubyte)CGETG(quad.v[0].col), (GLubyte)CGETB(quad.v[0].col), (GLubyte)CGETA(quad.v[0].col)
		};
		for (int i=0;i<6;i++) {
			mVertices[(mVertexCount*3)] = verteces[idx++];
			mVertices[(mVertexCount*3)+1] = verteces[idx++];
			mVertices[(mVertexCount*3)+2] = verteces[idx++];

			mUVs[(mVertexCount<<1)] = texCoords[u++];
			mUVs[(mVertexCount<<1)+1] = texCoords[u++];
				
			mColors[(mVertexCount<<2)] = colors[0];
			mColors[(mVertexCount<<2)+1] = colors[1];
			mColors[(mVertexCount<<2)+2] = colors[2];
			mColors[(mVertexCount<<2)+3] = colors[3];
			mVertexCount++;
		}

		if(currShader) {
			if(!currShader->attachShaderList())
				SoraCore::Instance()->log("SoraOGLESRenderer: error attaching shader list");
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

	bool SoraOGLESRenderer::isActive() {
		//return pHGE->System_GetState(HGE_ACTIVE);
		return true;
	}

	void SoraOGLESRenderer::renderRect(float32 x1, float32 y1, float32 x2, float32 y2, float32 fWidth, DWORD color, float32 z) {
		Rect4V rect;
			
		if(fWidth != y2-y1 && fWidth != x2-x1) {
			float rotAng = (float)atan2f(y2-y1, x2-x1)-D_PI_4;
			
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
            quad.v[i].z = 0.5f;
        }
		
        quad.blend = BLEND_DEFAULT;
		
		renderQuad(quad);
	}

	void SoraOGLESRenderer::setClipping(int32 x, int32 y, int32 w, int32 h) {
		// to do
		/*printf("%d, %d, %d, %d", x, y, w, h);
		glEnable(GL_SCISSOR_TEST);
		glScissor(x+w!=0?w:mainWindow->getWindowWidth(), y+h!=0?h:mainWindow->getWindowHeight(), w!=0?w:mainWindow->getWindowWidth(), h!=0?h:mainWindow->getWindowHeight());
	setTransform(x+w, y+h);*/
		int32 width = w;
		int32 height = h;
		if(w==0) {
			width = mainWindow->getWindowWidth();
			height = mainWindow->getWindowHeight();
		}
		_oglWindowInfo.width = width;
		_oglWindowInfo.height = height;
		_oglWindowInfo.x = x;
		_oglWindowInfo.y = y;
		applyTransform();
		//glViewport(x, y, width, height); 
		//setTransform(x, y);
	}
	
	void SoraOGLESRenderer::setTransformWindowSize(float32 w, float32 h) {
		_oglWindowInfo.width = w!=0.f?w:mainWindow->getWindowWidth();
		_oglWindowInfo.height = h!=0.f?h:mainWindow->getWindowHeight();
		applyTransform();

	}

	void SoraOGLESRenderer::setTransform(float32 x, float32 y, float32 dx, float32 dy, float32 rot, float32 hscale, float32 vscale) {
		_oglWindowInfo.x		=	x;
		_oglWindowInfo.y		=	y;
		_oglWindowInfo.dx		=	dx;
		_oglWindowInfo.dy		=	dy;
		_oglWindowInfo.rot		=	rot;
		_oglWindowInfo.hscale	=	hscale!=0.f?hscale:1.f;
		_oglWindowInfo.vscale	=	vscale!=0.f?hscale:1.f;
	}

	ulong32 SoraOGLESRenderer::createTarget(int width, int height, bool zbuffer) {
	/*	SoraRenderTargetOG* t = new SoraRenderTargetOG(width, height, zbuffer);
		liTargets.push_back((SoraRenderTarget*)t);
		return (ulong32)t;*/
	}

	void SoraOGLESRenderer::freeTarget(ulong32 t) {
	/*	SoraRenderTarget* pt = (SoraRenderTarget*)t;
		if(!pt) return;
		std::list<SoraRenderTarget*>::iterator itt = liTargets.begin();
		while(itt != liTargets.end()) {
			if((*itt) == pt) {
				delete pt;
				liTargets.erase(itt);
				break;
			}
			++itt;
		}*/
	}

	ulong32 SoraOGLESRenderer::getTargetTexture(ulong32 t) {
	/*	SoraRenderTarget* pt = (SoraRenderTarget*)t;
		if(!pt) return 0;
		pt->stex;
		return (ulong32)pt->stex;*/
	}
	
	bool SoraOGLESRenderer::_glVersionCheck() {
	/*	int majorVersion, minorVersion, rev;
		glfwGetVersion(&majorVersion, &minorVersion, &rev);
		return majorVersion >= 2;*/
		return true;
	}
	
	bool SoraOGLESRenderer::_glShaderCheck() {
		// to do
		// check extensions
		// return glfwExtensionSupported("GL_ARB_fragment_shade");
		return _glVersionCheck();
	}
	
	
	inline bool SoraOGLESRenderer::_glCheckError() {
		return glGetError() != GL_NO_ERROR;
	}
	
	SoraWString SoraOGLESRenderer::videoInfo() {
		SoraWString info(L"Driver=OpenGL Version=");
		//int mav, miv, rev;
		info += s2ws((char*)glGetString(GL_VERSION));

		//glfwGetVersion(&mav, &miv, &rev);
		//info += int_to_str(mav); info+=".";
		//info += int_to_str(miv); info+=".";
		//info += int_to_str(rev); info+="\n";
		return info;
	}

	void SoraOGLESRenderer::attachShaderContext(SoraShaderContext* context) {
		currShader = context;
	}

	void SoraOGLESRenderer::detachShaderContext() {
		if(!currShader->detachShaderList())
			SoraCore::Instance()->log("SoraOGLESRenderer: error detaching shader list");
		flush();
		currShader = 0;
	}

} // namespace sora