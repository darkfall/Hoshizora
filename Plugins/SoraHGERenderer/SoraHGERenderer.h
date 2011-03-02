/*
 *  SoraHGERenderer.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

// for HGE

#ifndef _SORA_HGE_RENDERER_H_
#define _SORA_HGE_RENDERER_H_

#include "hge/hge.h"
#include "SoraRenderSystem.h"

namespace sora {
	class SoraHGERenderer: public SoraRenderSystem {
	public:
		SoraHGERenderer();
		~SoraHGERenderer();

		bool update();

		void beginScene(ulong32 color=0, ulong32 target=0);
		void endScene();
		
		void beginFrame() {}
		void endFrame() {}

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
		void setTransform(float32 x=0.f, float32 y=0.f, float32 dx=0.f, float32 dy=0.f, float32 rot=0.f, float32 hscale=0.f, float32 vscale=0.f);
		void setTransformWindowSize(float32 w, float32 h);

		
		ulong32 getMainWindowHandle();
		SoraWindowInfoBase* getMainWindow();

		SoraShaderContext* createShaderContext();
		void attachShaderContext(SoraShaderContext* context);
		void detachShaderContext();

		SoraWString videoInfo();
		ulong32 getVideoDeviceHandle();

		void flush();
		
	private:
		HGE* pHGE;

		SoraTimer* pTimer;
		SoraWindowInfoBase* pMainWindow;

		bool bFullscreen;

		SoraShaderContext* currShader;
	};
} // namespace sora

#endif