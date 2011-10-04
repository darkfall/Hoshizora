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

		void beginScene(uint32 color=0, ulong32 target=0, bool clear=true);
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
		uint32*		 textureLock(SoraTexture*);
		void		 textureUnlock(SoraTexture*);
		void		 releaseTexture(SoraTexture* tex);

		void renderQuad(SoraQuad& quad);
		void renderTriple(SoraTriple& trip);
		void renderWithVertices(SoraTexture* tex, int32 blendMode, SoraVertex* vertices, uint32 size, int32 mode);

		void setClipping(int32 x=0, int32 y=0, int32 w=0, int32 h=0);
		void setTransform(float32 x=0.f, float32 y=0.f, float32 dx=0.f, float32 dy=0.f, float32 rot=0.f, float32 hscale=0.f, float32 vscale=0.f);
		void setTransformWindowSize(float32 w, float32 h);
        void setViewPoint(float32 x=0.f, float32 y=0.f, float32 z=0.f);
		void setVerticalSync(bool flag);
		
		ulong32 getMainWindowHandle();
		SoraWindowInfoBase* getMainWindow();

		SoraShaderContext* createShaderContext();
		void attachShaderContext(SoraShaderContext* context);
		void detachShaderContext();

		SoraWString videoInfo();
		ulong32 getVideoDeviceHandle();
        void snapshot(const SoraString& path);

		void flush();

		void setIcon(const SoraString& icon);
		void setCursor(const SoraString& cursor);
		
		void onExtensionStateChanged(int32 extension, bool state, int32 param);
        void renderLine(float32 x1, float32 y1, float32 x2, float32 y2, uint32 color, float width, float32 z);
        void renderBox(float32 x1, float32 y1, float32 x2, float32 y2, uint32 color, float width, float32 z);
		void fillBox(float32 x1, float32 y1, float32 x2, float32 y2, uint32 color, float32 z);

		void getDesktopResolution(int* w, int *y);
		void setQueryVideoModeCallback(QueryVideoMode func);
		
	private:
		inline int32 _modeToDXMode(int32 mode);
		
		typedef std::map<HTEXTURE, SoraTextureHandle> TARGET_TEX_MAP;
		TARGET_TEX_MAP mTargetTextures;
		
		HGE* pHGE;

		SoraTimer* pTimer;
		SoraWindowInfoBase* pMainWindow;

		bool bFullscreen;

		SoraShaderContext* currShader;
	};
} // namespace sora

#endif