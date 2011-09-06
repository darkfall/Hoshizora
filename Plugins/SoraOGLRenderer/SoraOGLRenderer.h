/*
 *  SoraOGLRenderer.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/23/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
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
        
        void renderLine     (float x1, float y1, float x2, float y2, uint32 color, float z=0.f);
		void renderBox		(float x1, float y1, float x2, float y2, uint32 color, float z=0.f);
        void fillBox        (float x1, float y1, float x2, float y2, uint32 color, float z=0.f);

		void setClipping(int32 x=0, int32 y=0, int32 w=0, int32 h=0);
		void setTransform(float x=0.f, float y=0.f, float dx=0.f, float dy=0.f, float rot=0.f, float hscale=1.f, float vscale=1.f);
		void setTransformWindowSize(float w, float h);
        void setViewPoint(float x=0.f, float y=0.f, float z=0.f);
        void setVerticalSync(bool flag);

		ulong32 getMainWindowHandle() { return (ulong32)mainWindow; }
		SoraWindowInfoBase* getMainWindow() { return mainWindow; }

        SoraShaderContext* createShaderContext();
		void attachShaderContext(SoraShaderContext* context);
		void detachShaderContext();

		SoraWString videoInfo();
		ulong32 getVideoDeviceHandle();

		void flush();
        
        void snapshot(const SoraString& path);
        
        void setIcon(const SoraString& icon);
        void setCursor(const SoraString& cursor);
        
        void onExtensionStateChanged(int32 extension, bool state, int32 param);

	private:
		void applyTransform();
		void bindTexture(SoraTexture* tex);
		
		
		inline int32 _modeToGLMode(int32 mode);
		inline void _glInitialize();
		inline void _glEndFrame();
		inline void _glBeginFrame();
		inline void _glBeginScene(uint32 color, ulong32 target, bool clear);
		inline void _glEndScene();
		inline int32 _glTextureGetWidth(int32 tex, bool bOriginal=false);
		inline int32 _glTextureGetHeight(int32 tex, bool bOriginal=false);
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

		struct _SoraOGLWindowInfo {
			float x, y, z;
			float dx, dy;
			float rot;
			float hscale, vscale;
			int32 width, height;
			
			_SoraOGLWindowInfo(): hscale(1.f), vscale(1.f), x(0.f), y(0.f), z(0.f), dx(0.f), dy(0.f), rot(0.f), width(0), height(0) {}
		};
		_SoraOGLWindowInfo _oglWindowInfo;

		int32 CurBlendMode;
		int32 CurDrawMode;

		SoraTimer* pTimer;

		std::list<SoraRenderTargetOG*> liTargets;
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
