/*
 *  SoraiOSGLES2Renderer.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 1/26/11.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef _SORA_IOSGLES2_RENDERER_H_
#define _SORA_IOSGLES2_RENDERER_H_

#include "SoraRenderSystem.h"
#include "SoraiOSDeviceHelper.h"

#include "math/SoraMatrix4.h"
#include "math/SoraMatrix3.h"

#ifdef OS_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

namespace sora {
    
    class SoraRenderTargetiOSGL;

	class SoraiOSGLES2Renderer: public SoraRenderSystem {
	public:
		SoraiOSGLES2Renderer();
		~SoraiOSGLES2Renderer();
		
		bool update();
		
		void beginScene(uint32 color=0, ulong32 target=0, bool clear=true);
		void endScene();
		
		void beginFrame();
		void endFrame();
		
		void shutdown();
		bool isActive();
		
		void start(SoraTimer* timer);
		
		ulong32 createTarget(int width, int height, bool zbuffer=true);
		void	freeTarget(ulong32 t);
		ulong32 getTargetTexture(ulong32 t);
		
		SoraWindowHandle createWindow(SoraWindowInfoBase* windowInfo);
		void setWindowSize(int32 w, int32 h);
		void setWindowTitle(const SoraWString& title);
		void setWindowPos(int32 px, int32 py);
		void setFullscreen(bool flag);
		bool isFullscreen();
		
		SoraTexture* createTexture(const SoraWString& sTexturePath, bool bMipmap=false);
		SoraTexture* createTextureWH(int w, int h);
		SoraTexture* createTextureFromMem(void* ptr, ulong32 size, bool bMipmap=false);
		SoraTexture* createTextureFromRawData(unsigned int* data, int32 w, int32 h);
		
		uint32*		 textureLock(SoraTexture*);
		void		 textureUnlock(SoraTexture*);
		void		 releaseTexture(SoraTexture* tex);
		
		void renderQuad(SoraQuad& quad);
		void renderTriple(SoraTriple& trip);
		void renderWithVertices(SoraTexture* tex, int32 blendMode, SoraVertex* vertices, uint32 vsize, int32 mode);
		
		void setClipping(int32 x=0, int32 y=0, int32 w=0, int32 h=0);
		void setTransform(float32 x=0.f, float32 y=0.f, float32 dx=0.f, float32 dy=0.f, float32 rot=0.f, float32 hscale=1.f, float32 vscale=1.f);
		void setTransformWindowSize(float32 w, float32 h);
        void setViewPoint(float32 x=0.f, float32 y=0.f, float32 z=0.f);
		
		ulong32 getMainWindowHandle();
		SoraWindowInfoBase* getMainWindow();
		
        SoraShaderContext* createShaderContext();
		void attachShaderContext(SoraShaderContext* context);
		void detachShaderContext();
		
		StringType videoInfo();
		ulong32 getVideoDeviceHandle();
        void setVerticalSync(bool flag);
		
		void flush();
        
        void snapshot(const SoraString& path);
		void onExtensionStateChanged(int32 extension, bool state, int32 param);
        
        void renderLine(float x1, float y1, float x2, float y2, uint32 color, float width, float z=0.f);
		void renderBox(float x1, float y1, float x2, float y2, uint32 color, float width, float z=0.f);
        void fillBox(float x1, float y1, float x2, float y2, uint32 color, float z=0.f);

        void setIcon(const SoraString& icon);
        void setCursor(const SoraString& cursor);
        
        void setOrientation(iOSOrientation por);
        iOSOrientation getOrientation() const;
        
        void tranlatePointToGL(float* x, float* y);
        
        void getDesktopResolution(int* w, int* h);
        void setQueryVideoModeCallback(QueryVideoMode func);
        
	private:
		void applyTransform();
		void bindTexture(SoraTexture* tex);
		
		inline int32 _modeToGLMode(int32 mode);
		inline void _glInitialize();
		inline void _glEndFrame();
		inline void _glBeginFrame();
		inline void _glBeginScene(ulong32 color, ulong32 target, bool clear);
		inline void _glEndScene();
		inline int32 _glTextureGetWidth(ulong32 tex, bool bOriginal=false);
		inline int32 _glTextureGetHeight(ulong32 tex, bool bOriginal=false);
		inline void _glSetProjectionMatrix(int32 w, int32 h);
		inline void _glSetBlendMode(int32 mode);
		
		// checks if opengl 2.0 is available
		inline bool _glVersionCheck();
		// checks if glsl is avaiable
		inline bool _glShaderCheck();
		const char* _glGetShaderLog(GLuint shader);
		
		inline bool _glCheckError();
		
		SoraWindowInfoBase* mainWindow;
		int32 windowWidth;
		int32 windowHeight;
		
        /**
         * OGL ES2 Shader Param
         **/
        SoraShader* mVertexProgram;
        
		struct _SoraOGLWindowInfo {
			float32 x, y, z;
			float32 dx, dy;
			float32 rot;
			float32 hscale, vscale;
			int32 width, height;
			
			_SoraOGLWindowInfo(): hscale(1.f), vscale(1.f), x(0.f), y(0.f), z(0.f), dx(0.f), dy(0.f), rot(0.f), width(0), height(0) {}
		};
		_SoraOGLWindowInfo _oglWindowInfo;
		
		int32 CurBlendMode;
		int32 CurDrawMode;
				
		std::list<SoraRenderTargetiOSGL*> liTargets;
		SoraRenderTargetiOSGL* pCurTarget;
		
		GLuint mCurrTexture;
		SoraShaderContext* currShader;

		int iFrameStart;
        
        iOSOrientation mOrientation;
        
        SoraShaderContext* g_shader;
        GLuint g_vertexbuffer;
        GLuint g_elementbuffer;
                
        int mVertexCount;
        GLfloat mVertices[84];
        GLfloat mUVs[26];
        GLubyte mColors[26];

        SoraMatrix4 mUVPMatrix;
	};
} // namespace sora

#endif