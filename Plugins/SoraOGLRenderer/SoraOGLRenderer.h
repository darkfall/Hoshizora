/*
 *  SoraOGLRenderer.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SORA_OGL_RENDERER_H_
#define _SORA_OGL_RENDERER_H_

#include "SoraRenderSystem.h"

#include "SoraOGLRenderTarget.h"

namespace sora {

	class SoraOGLRenderer: public SoraRenderSystem {
	public:
		SoraOGLRenderer();
		~SoraOGLRenderer();

		bool update();

		void beginScene(ulong32 color=0, ulong32 target=0);
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

		ulong32*	 textureLock(SoraTexture*, bool bReadOnly=true, uint32 x=0, uint32 y=0, uint32 w=0, uint32 h=0);
		void		 textureUnlock(SoraTexture*);
		void		 releaseTexture(SoraTexture* tex);

		void renderRect(float32 x1, float32 y1, float32 x2, float32 y2, float32 fWidth=1.f, DWORD color=0xFFFFFFFF, float32 z=0.5f);
		void renderQuad(SoraQuad& quad);
		void renderTriple(SoraTriple& trip);

		void setClipping(int32 x=0, int32 y=0, int32 w=0, int32 h=0);
		void setTransform(float32 x=0.f, float32 y=0.f, float32 dx=0.f, float32 dy=0.f, float32 rot=0.f, float32 hscale=1.f, float32 vscale=1.f);
		void setTransformWindowSize(float32 w, float32 h);
        void setViewPoint(float32 x=0.f, float32 y=0.f, float32 z=0.f);

		ulong32 getMainWindowHandle() { return (ulong32)mainWindow; }
		SoraWindowInfoBase* getMainWindow() { return mainWindow; }

		void attachShaderContext(SoraShaderContext* context);
		void detachShaderContext();

		SoraWString videoInfo();
		ulong32 getVideoDeviceHandle();

		void flush();
        
        void snapshot(const SoraString& path);

	private:
		void applyTransform();
		void bindTexture(SoraTexture* tex);

		inline void _glInitialize();
		inline void _glEndFrame();
		inline void _glBeginFrame();
		inline void _glBeginScene(ulong32 color, ulong32 target);
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

		struct _SoraOGLWindowInfo {
			float32 x, y, z;
			float32 dx, dy;
			float32 rot;
			float32 hscale, vscale;
			int32 width, height;
			
			_SoraOGLWindowInfo(): hscale(1.f), vscale(1.f), x(0.f), y(0.f), z(0.f), dx(0.f), dy(0.f), rot(0.f), width(0.f), height(0.f) {}
		};
		_SoraOGLWindowInfo _oglWindowInfo;

		int32 CurBlendMode;

		SoraTimer* pTimer;

		std::list<SoraRenderTarget*> liTargets;
		SoraRenderTargetOG* pCurTarget;

		bool bShaderAvailable;
		GLuint uGLShaderProgram;

		GLuint mCurrTexture;

		SoraShaderContext* currShader;

		bool bFullscreen;
		int iFrameStart;
	};
} // namespace sora

#endif
