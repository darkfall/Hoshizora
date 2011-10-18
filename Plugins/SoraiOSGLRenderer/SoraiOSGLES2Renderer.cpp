/*
 *  SoraiOSGLES2Renderer.cpp
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/23/10.
 *  Copyright 2010 Griffin Bu. All rights reserved.
 *
 */

#include "SoraiOSGLES2Renderer.h"

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
#include "SoraResourceFile.h"

#include "SOIL/SOIL.h"

namespace sora {
    
    const uint32 MAX_VERTEX_BUFFER = 128;
    
    enum {
        VERTEX_ARRAY,
        COLOR_ARRAY,
        UV_ARRAY,
        MVP_ARRAY,
        NUM_ELEMENTS_ARRAY,
    };

    static const GLfloat g_vertex_buffer_data[] = { 
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f,  1.0f,
        1.0f,  1.0f
    };
    
    static const GLubyte squareColors[] = {
        255, 255,   0, 255,
        0,   255, 255, 255,
        0,     0,   0,   0,
        255,   0, 255, 255,
    };
    
	SoraiOSGLES2Renderer::SoraiOSGLES2Renderer() {
		pCurTarget = 0;
		
		mCurrTexture = -1;
		
		currShader = 0;
		iFrameStart = 1;
		CurBlendMode = 0;
        mVertexCount = 0;
        
        g_shader = new SoraGLSLShaderContext;
        g_shader->attachVertexShader("vertex.vs", "main");
        g_shader->attachFragmentShader("fragment.fs", "main");
        
        SoraGLSLShader* vertex = (SoraGLSLShader*)g_shader->getVertexShader();
        glAttachShader(vertex->mProgram, vertex->mShader);
        
        glBindAttribLocation(vertex->mProgram, UV_ARRAY, "uv");
        glBindAttribLocation(vertex->mProgram, VERTEX_ARRAY, "position");
        glBindAttribLocation(vertex->mProgram, COLOR_ARRAY, "color");
    }

	SoraiOSGLES2Renderer::~SoraiOSGLES2Renderer() {
	//	shutdown();
	}

	void SoraiOSGLES2Renderer::shutdown() {
		std::list<SoraRenderTargetiOSGL*>::iterator itt = liTargets.begin();
		while(itt != liTargets.end()) {
			delete (*itt);
			++itt;
		}
		liTargets.clear();
		
		//delete mainWindow;
	}

	void SoraiOSGLES2Renderer::_glInitialize() {		
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearStencil(0);                          // clear stencil buffer
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);                       // The Type Of Depth Test To Do
		glClearDepthf(1.f);                          // Depth Buffer Setup
        
        //glDepthMask(GL_FALSE);
        
	//	glEnable(GL_CULL_FACE);
	//	glEnable(GL_COLOR_MATERIAL);
		
		//   InitPerspective(60, (float)_oglWindowInfo.width / _oglWindowInfo.height, 0.f, 1.f);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations
		
		glDisable(GL_DITHER);
		glDisable(GL_FOG);
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
	//	glDisable(GL_TEXTURE_2D);
    }
	
	void SoraiOSGLES2Renderer::_glBeginFrame() {
	}
	
	void SoraiOSGLES2Renderer::_glEndFrame() {
		glFlush();
	}
	
	void SoraiOSGLES2Renderer::beginFrame() {
		applyTransform();
		iFrameStart = 1;
	}
	
	void SoraiOSGLES2Renderer::endFrame() {
		_glEndFrame();
	}
	
	void SoraiOSGLES2Renderer::_glSetProjectionMatrix(int32 w, int32 h) {
		glViewport(0, 0, w, h);
	//	glMatrixMode(GL_PROJECTION);
		
		//Clearing the projection matrix...
	//	glLoadIdentity();
		//Creating an orthoscopic view matrix going from -1 -> 1 in each
		//dimension on the screen (x, y, z).
	//	glOrthof(0.f, 320, 480, 0.f, -1.f, 1.f);
	}
	
	void SoraiOSGLES2Renderer::applyTransform() {
        
        float w = _oglWindowInfo.width * getContentsScale();
        float h = _oglWindowInfo.height * getContentsScale();
        
        glViewport(0, 0,
                   w, h);
        float mat[] = {2.f/w, 0, 0, -1.f, 
                        0, 2.f/-h, 0.f, 1.f, 
                        0.f, 0.f, 0.f, -1.f, 
                        0.f, 0.f, 0.f, 1.f};
        mUVPMatrix = SoraMatrix4(mat);
       /* mUVPMatrix.translate(-_oglWindowInfo.x, -_oglWindowInfo.y, 0.f);
        switch(mOrientation) {
            case ORIENTATION_LANDSCAPE_LEFT:
                mUVPMatrix.rotate(90+_oglWindowInfo.rot,0,0);
                mUVPMatrix.translate(0, -w, 0);
                break;
            case ORIENTATION_LANDSCAPE_RIGHT:
                mUVPMatrix.rotate(-90+_oglWindowInfo.rot,0,0);
                mUVPMatrix.translate(-h, 0 ,0);
                break;
            case ORIENTATION_PORTRAIT_UPSIDE_DOWN:
                mUVPMatrix.translate(w/2,h/2,0);
                mUVPMatrix.rotate(180+_oglWindowInfo.rot,0,0);
                mUVPMatrix.translate(-w/2,-h/2,0);
                break;
            case ORIENTATION_PORTRAIT:
                mUVPMatrix.rotate(_oglWindowInfo.rot, 0.f, 0.f);
                break;
        }
        mUVPMatrix.scale(_oglWindowInfo.hscale,
                         _oglWindowInfo.vscale,
                         1.f);*/
        
       /* if(!pCurTarget) {
            float w = _oglWindowInfo.width * getContentsScale();
            float h = _oglWindowInfo.height * getContentsScale();
            
            glViewport(0, 0,
                       w, h);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrthof(0.f,
                     w, h
                     , 0.f, 1.f, -1.f);
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
                    , pCurTarget->getHeight(), 1.f, -1.f);
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(-_oglWindowInfo.x, -_oglWindowInfo.y, 0.f);
            glRotatef(_oglWindowInfo.rot, -0.f, 0.f, 1.f);
            glScalef(_oglWindowInfo.hscale, _oglWindowInfo.vscale, 1.0f);//Transformation follows order scale->rotation->displacement
            glTranslatef(_oglWindowInfo.x+_oglWindowInfo.dx, _oglWindowInfo.y+_oglWindowInfo.dy, 0.f); //Set Center Coodinates
        }*/
	}
	
	void SoraiOSGLES2Renderer::_glBeginScene(ulong32 color, ulong32 t, bool clear) {
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
	
	void SoraiOSGLES2Renderer::_glEndScene() {
		if(pCurTarget != NULL) {
			pCurTarget->detachFromRender();
            pCurTarget = 0;
			
            setTransform(0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f);
            setClipping();
        } else {
            iFrameStart = 0;
            presentEAGLFBO();
        }
	}
	
	void SoraiOSGLES2Renderer::_glSetBlendMode(int32 blend) {
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
	
	bool SoraiOSGLES2Renderer::update() {
		//clearPoll();
		return false;
	}
	
	void SoraiOSGLES2Renderer::start(SoraTimer* timer) {
		SoraiOSInitializer::Instance()->setTimer(timer);
		g_timer = timer;
        
	}

	void SoraiOSGLES2Renderer::beginScene(uint32 color, ulong32 target, bool clear) {
		_glBeginScene(color, target, clear);
	}

	void SoraiOSGLES2Renderer::endScene() {
		currShader = 0;
		_glEndScene();
	}

	SoraWindowHandle SoraiOSGLES2Renderer::createWindow(SoraWindowInfoBase* windowInfo) {
		mainWindow = windowInfo;
		_oglWindowInfo.width = windowInfo->getWindowWidth();
		_oglWindowInfo.height = windowInfo->getWindowHeight();
		
		_glInitialize();
		_glSetProjectionMatrix(_oglWindowInfo.width, _oglWindowInfo.height);
		applyTransform();
		
		return (ulong32)windowInfo;
		
		return 0;
	}
	
	void SoraiOSGLES2Renderer::setWindowSize(int32 w, int32 h) {
	}
	
	void SoraiOSGLES2Renderer::setWindowTitle(const SoraWString& title) {
	}
	
	void SoraiOSGLES2Renderer::setWindowPos(int32 px, int32 py) {
	}
	
	void SoraiOSGLES2Renderer::setFullscreen(bool flag) {

	}
	
	bool SoraiOSGLES2Renderer::isFullscreen() {
        return true;
	}
	
	ulong32 SoraiOSGLES2Renderer::getVideoDeviceHandle() {
		return (ulong32)this;
	}
	
	int32 SoraiOSGLES2Renderer::_glTextureGetWidth(ulong32 tex, bool bOriginal) {
		/*GLint w; 
		glBindTexture(GL_TEXTURE_2D, tex);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glBindTexture(GL_TEXTURE_2D, mCurrTexture);
		return w;*/
		return 0;
	}
	
	int32 SoraiOSGLES2Renderer::_glTextureGetHeight(ulong32 tex, bool bOriginal) {
		/*GLint h; 
		glBindTexture(GL_TEXTURE_2D, tex);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
		glBindTexture(GL_TEXTURE_2D, mCurrTexture);
		return h;*/
		return 0;
	}
	
	void SoraiOSGLES2Renderer::bindTexture(SoraTexture* tex) {
        /*if(!tex) {
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
			
		}*/
	}
	
	void SoraiOSGLES2Renderer::flush() {
		if (mVertexCount > 0) {
            SoraGLSLShader* vertex = (SoraGLSLShader*)g_shader->getVertexShader();
            sora_assert(vertex);
            SoraGLSLShader* fragment = (SoraGLSLShader*)g_shader->getFragmentShader();
            
            if(fragment)
                glAttachShader(vertex->mProgram, fragment->mShader);
            glLinkProgram(vertex->mProgram);
        
            if(fragment) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, mCurrTexture);
                
                GLint texloc = glGetUniformLocation(fragment->mProgram, "texture");
                glUniform1i(texloc, 0);
            }
            GLint mvp = glGetUniformLocation(vertex->mProgram, "mvpmatrix");
            glUniformMatrix4fv(mvp, 1, 0, &mUVPMatrix.x[0]);
            
            glUseProgram(vertex->mProgram);
            
            static float vert[] = {
                -1, -1, 0,
                1, -1, 0,
                -1, 1, 0,
                1, 1, 0,
            };
            
            /*SoraVector4 vt = mUVPMatrix * SoraVector4(0, 0, 0, 0);
            printf("vt : %f, %f, %f, %f\n", vt.x, vt.y, vt.z, vt.w);
            vt = mUVPMatrix * SoraVector4(320, 0, 0, 0);
            printf("vt2 : %f, %f, %f, %f\n", vt.x, vt.y, vt.z, vt.w);
            vt = mUVPMatrix * SoraVector4(0, 480, 0, 0);
            printf("vt3 : %f, %f, %f, %f\n", vt.x, vt.y, vt.z, vt.w);
            vt = mUVPMatrix * SoraVector4(320, 480, 0, 0);
            printf("vt4 : %f, %f, %f, %f\n", vt.x, vt.y, vt.z, vt.w);*/

            glVertexAttribPointer(VERTEX_ARRAY, 4, GL_FLOAT, 0, 0, mVertices);
            glEnableVertexAttribArray(VERTEX_ARRAY);
   
            glVertexAttribPointer(UV_ARRAY, 2, GL_FLOAT, 0, 0, mUVs);
            glEnableVertexAttribArray(UV_ARRAY);
            
            glVertexAttribPointer(COLOR_ARRAY, 4, GL_UNSIGNED_BYTE, 1, 0, mColors);
            glEnableVertexAttribArray(COLOR_ARRAY);
            
            
            glDrawArrays(CurDrawMode, 0, mVertexCount);

            glDisableVertexAttribArray(VERTEX_ARRAY);
            glDisableVertexAttribArray(COLOR_ARRAY);
            glDisableVertexAttribArray(UV_ARRAY);
          
          //  if(fragment)
          //      glDetachShader(vertex->mProgram, fragment->mShader);
            
            mVertexCount = 0;
		}
	}

	SoraTexture* SoraiOSGLES2Renderer::createTexture(const SoraWString& sTexturePath, bool bMipmap) {
		return 0;
	}
	
	SoraTexture* SoraiOSGLES2Renderer::createTextureFromMem(void* ptr, ulong32 size, bool bMipmap) {
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

	SoraTexture* SoraiOSGLES2Renderer::createTextureWH(int w, int h) {
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
		glTexParameterf(texId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(texId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(texId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(texId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    //    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		return new SoraTexture(texId, p2w, p2h, w, h);
	}
	
	SoraTexture* SoraiOSGLES2Renderer::createTextureFromRawData(unsigned int* data, int32 w, int32 h) {
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

	uint32* SoraiOSGLES2Renderer::textureLock(SoraTexture* ht) {
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
	
	void SoraiOSGLES2Renderer::textureUnlock(SoraTexture* ht) {
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

	void SoraiOSGLES2Renderer::releaseTexture(SoraTexture* tex) {
		glDeleteTextures(1, (const GLuint*)&tex->mTextureID);
		delete tex;
		tex = 0;
	}
	
	int32 SoraiOSGLES2Renderer::_modeToGLMode(int32 mode) {
		switch (mode) {
	//		case SORA_LINE:				return GL_LINE;
			case SORA_TRIANGLES:		return GL_TRIANGLES;
			case SORA_TRIANGLES_FAN:	return GL_TRIANGLE_FAN;
			case SORA_TRIANGLES_STRIP:	return GL_TRIANGLE_STRIP;
	//		case SORA_QUAD:				return GL_QUADS;
		}
		return GL_TRIANGLES;
	}
	
	void SoraiOSGLES2Renderer::renderTriple(SoraTriple& trip) {
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
		
		flush();	
	}
	
	void SoraiOSGLES2Renderer::renderWithVertices(SoraTexture* tex, int32 blendMode,  SoraVertex* vertices, uint32 vsize, int32 mode) {				
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
	
	void SoraiOSGLES2Renderer::renderQuad(SoraQuad& quad) {
		glEnable(GL_TEXTURE_2D); // Enable Texture Mapping
		mCurrTexture = quad.tex->mTextureID;
		_glSetBlendMode(quad.blend);
		CurDrawMode = GL_TRIANGLE_STRIP;
		
		//TODO: insert code here
		GLfloat verteces[16] = {
			quad.v[0].x, quad.v[0].y, quad.v[0].z, 1.0,
			quad.v[1].x, quad.v[1].y, quad.v[1].z, 1.0,
			quad.v[3].x, quad.v[3].y, quad.v[3].z, 1.0,
			quad.v[2].x, quad.v[2].y, quad.v[3].z, 1.0,
		};
		
		GLfloat texCoords[8] = {
			quad.v[0].tx, quad.v[0].ty,
			quad.v[1].tx, quad.v[1].ty,
			quad.v[3].tx, quad.v[3].ty,
			quad.v[2].tx, quad.v[2].ty,
		};
		GLubyte colors[16] = {
			(GLubyte)CGETR(quad.v[0].col), (GLubyte)CGETG(quad.v[0].col), (GLubyte)CGETB(quad.v[0].col), (GLubyte)CGETA(quad.v[0].col),
			(GLubyte)CGETR(quad.v[1].col), (GLubyte)CGETG(quad.v[1].col), (GLubyte)CGETB(quad.v[1].col), (GLubyte)CGETA(quad.v[1].col),
			(GLubyte)CGETR(quad.v[3].col), (GLubyte)CGETG(quad.v[3].col), (GLubyte)CGETB(quad.v[3].col), (GLubyte)CGETA(quad.v[3].col),
			(GLubyte)CGETR(quad.v[2].col), (GLubyte)CGETG(quad.v[2].col), (GLubyte)CGETB(quad.v[2].col), (GLubyte)CGETA(quad.v[2].col),
		};
		
		int u = 0;
		int idx = 0;
		int cdx = 0;
		for (int i=0;i<4;i++) {
			mVertices[(mVertexCount*4)] = verteces[idx++];
			mVertices[(mVertexCount*4)+1] = verteces[idx++];
			mVertices[(mVertexCount*4)+2] = verteces[idx++];
            mVertices[(mVertexCount*4)+3] = verteces[idx++];
			
			mUVs[(mVertexCount<<1)] = texCoords[u++];
			mUVs[(mVertexCount<<1)+1] = texCoords[u++];
			
			mColors[(mVertexCount<<2)] = colors[cdx++];
			mColors[(mVertexCount<<2)+1] = colors[cdx++];
			mColors[(mVertexCount<<2)+2] = colors[cdx++];
			mColors[(mVertexCount<<2)+3] = colors[cdx++];
			mVertexCount++;
		}
		
        flush();
    }

	bool SoraiOSGLES2Renderer::isActive() {
		return true;
	}

	void SoraiOSGLES2Renderer::setClipping(int32 x, int32 y, int32 w, int32 h) {
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

	void SoraiOSGLES2Renderer::setTransform(float32 x, float32 y, float32 dx, float32 dy, float32 rot, float32 hscale, float32 vscale) {
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

	ulong32 SoraiOSGLES2Renderer::createTarget(int width, int height, bool zbuffer) {        
		SoraRenderTargetiOSGL* t = new SoraRenderTargetiOSGL(width, height, zbuffer);
		liTargets.push_back((SoraRenderTargetiOSGL*)t);
		return (ulong32)t;
	}

	void SoraiOSGLES2Renderer::freeTarget(ulong32 t) {
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

	ulong32 SoraiOSGLES2Renderer::getTargetTexture(ulong32 t) {
		SoraRenderTargetiOSGL* pt = (SoraRenderTargetiOSGL*)t;
        assert(pt != NULL);
		
		return pt->getTexture();
	}
	
	bool SoraiOSGLES2Renderer::_glVersionCheck() {
		return true;
	}
	
	bool SoraiOSGLES2Renderer::_glShaderCheck() {
		// to do
		// check extensions
		// return glfwExtensionSupported("GL_ARB_fragment_shade");
		return _glVersionCheck();
	}
	
	inline bool SoraiOSGLES2Renderer::_glCheckError() {
		return glGetError() != GL_NO_ERROR;
	}
	
	StringType SoraiOSGLES2Renderer::videoInfo() {
		StringType info("Driver: OpenGL Version ");
		//int mav, miv, rev;
		info += (char*)glGetString(GL_VERSION);
		return info;
	}
	
	SoraShaderContext* SoraiOSGLES2Renderer::createShaderContext() {
		return new SoraGLSLShaderContext();
        return 0;
	}
	
	void SoraiOSGLES2Renderer::snapshot(const SoraString& file) {
	}
	
	void SoraiOSGLES2Renderer::setViewPoint(float, float, float) {
	}

	void SoraiOSGLES2Renderer::attachShaderContext(SoraShaderContext* context) {
		currShader = context;
	}
    
    void SoraiOSGLES2Renderer::setVerticalSync(bool flag) {
    }

	void SoraiOSGLES2Renderer::detachShaderContext() {
		if(!currShader->detachShaderList())
			log_error("SoraiOSGLES2Renderer: error detaching shader list");
		flush();
		currShader = 0;
	}

    void SoraiOSGLES2Renderer::onExtensionStateChanged(int32 extension, bool state, int32 param) {

    }
    
    void SoraiOSGLES2Renderer::renderLine(float x1, float y1, float x2, float y2, uint32 color, float width, float z) {
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
    
    void SoraiOSGLES2Renderer::fillBox(float x1, float y1, float x2, float y2, uint32 color, float z) {
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
	
	void SoraiOSGLES2Renderer::renderBox(float x1, float y1, float x2, float y2, uint32 color, float lineWidth, float z) {
		renderLine(x1, y1, x2, y1+1.f, color, lineWidth, z);
		renderLine(x2, y1, x2+1.f, y2, color, lineWidth, z);
		renderLine(x2, y2, x1, y2+1.f, color, lineWidth, z);
		renderLine(x1, y2, x1+1.f, y1, color, lineWidth, z);
	}
    
    void SoraiOSGLES2Renderer::setIcon(const SoraString& icon) {
        
    }
    
    void SoraiOSGLES2Renderer::setCursor(const SoraString& cursor) {
        
    }
    
    void SoraiOSGLES2Renderer::setOrientation(iOSOrientation por) {
        mOrientation = por;
    }
    
    iOSOrientation SoraiOSGLES2Renderer::getOrientation() const {
        return mOrientation;
    }
    
    void SoraiOSGLES2Renderer::getDesktopResolution(int* w, int* h) {
        *w = sora::getScreenWidth();
        *h = sora::getScreenHeight();
    }
    
    void SoraiOSGLES2Renderer::setQueryVideoModeCallback(QueryVideoMode func) {
        if(func) {
            func(getScreenWidth(), getScreenHeight());
        }
    }
    
    ulong32 SoraiOSGLES2Renderer::getMainWindowHandle() { 
        return (ulong32)mainWindow;
    }
    
    SoraWindowInfoBase* SoraiOSGLES2Renderer::getMainWindow() {
        return mainWindow;
    }
    
} // namespace sora