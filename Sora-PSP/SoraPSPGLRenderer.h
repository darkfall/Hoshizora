/*
 *  SoraOGLESRenderer.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 1/26/11.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef _SORA_PSPGL_RENDERER_H_
#define _SORA_PSPGL_RENDERER_H_

#include "SoraPlatform.h"
#include "SoraRenderSystem.h"
#include "SoraRenderTarget.h"

class JGE;
class JRenderer;
class JTexture;

namespace sora {

	class SoraRenderTargetPSPGL;
	class SoraJGEApp;
	
	
	class SoraPSPGLRenderer: public SoraRenderSystem {
	public:
		SoraPSPGLRenderer();
		~SoraPSPGLRenderer();
		
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
		
		ulong32 getMainWindowHandle() { return (ulong32)mainWindow; }
		SoraWindowInfoBase* getMainWindow() { return mainWindow; }
		
        SoraShaderContext* createShaderContext();
		void attachShaderContext(SoraShaderContext* context);
		void detachShaderContext();
		
		SoraWString videoInfo();
		ulong32 getVideoDeviceHandle();
		
		void flush();
        
        void snapshot(const SoraString& path);
		
	private:	
		SoraWindowInfoBase* mainWindow;
		int32 windowWidth;
		int32 windowHeight;
		
		std::list<SoraRenderTarget*> liTargets;
		SoraRenderTargetPSPGL* pCurTarget;
		
		bool bShaderAvailable;

		SoraShaderContext* currShader;
		int iFrameStart;
		
		SoraJGEApp* JGEApp;
		JRenderer* JGERenderer;
		
		typedef hash_map<SoraTexture*, JTexture*> TEXTURE_MAP;
		TEXTURE_MAP mTextureMap;
	};
} // namespace sora

#endif