/*
 *  SoraRenderSystem.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/24/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */
#ifndef SORA_RENDER_SYSTEM_H_
#define SORA_RENDER_SYSTEM_H_

#include "SoraPlatform.h"
#include "SoraException.h"

#include "SoraWindowInfo.h"

#include "SoraSprite.h"
#include "SoraTexture.h"
#include "SoraTimer.h"
#include "SoraShader.h"

#include <map>

namespace sora {
	/**
		Abstarct Render System Base
	*/

	class SORA_API SoraRenderSystem {
	public:
		virtual bool update() = 0;

		virtual void beginScene(ulong32 c, ulong32 t, bool clear=true) = 0;
		virtual void endScene() = 0;
		
		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;

		virtual void start(SoraTimer* timer) = 0;

		virtual ulong32 createTarget(int width, int height, bool zbuffer=true) = 0;
		virtual void	freeTarget(ulong32 t) = 0;
		virtual ulong32 getTargetTexture(ulong32 t) = 0;

		virtual SoraWindowHandle createWindow(SoraWindowInfoBase* windowInfo) = 0;

		virtual SoraTexture* createTexture(const SoraWString& sTexturePath, bool bMipmap=false) = 0;
		virtual SoraTexture* createTextureWH(int32 w, int32 h) = 0;
		virtual SoraTexture* createTextureFromMem(void* ptr, ulong32 size, bool bMipmap=false) = 0;
		virtual SoraTexture* createTextureFromRawData(unsigned int* data, int32 w, int32 h) = 0;

		virtual uint32*  	 textureLock(SoraTexture* ht) = 0;
		virtual void         textureUnlock(SoraTexture* ht) = 0;
        virtual void		 releaseTexture(SoraTexture* pTexture) = 0;

		virtual void renderQuad(SoraQuad& quad) = 0;
		virtual void renderTriple(SoraTriple& trip) = 0;
		virtual void renderWithVertices(SoraTexture* tex, int32 blendMode, SoraVertex* vertices, uint32 vsize, int32 mode) = 0;

		virtual void renderRect(float32 x1, float32 y1, float32 x2, float32 y2, float32 fWidth, uint32 color=0xFFFFFFFF, float32 z=0.0f) = 0;
		virtual void renderBox(float32 x, float32 y, float32 width, float32 height, uint32 color=0xFFFFFFFF, float32 z=0.0f) = 0;
		virtual void setClipping(int32 x=0, int32 y=0, int32 w=0, int32 h=0) = 0;
		virtual void setTransform(float32 x=0.f, float32 y=0.f, float32 dx=0.f, float32 dy=0.f, float32 rot=0.f, float32 hscale=0.f, float32 vscale=0.f) = 0;
        virtual void setViewPoint(float32 x=0.f, float32 y=0.f, float32 z=0.f) = 0;
        
		virtual void shutdown() = 0;
		virtual ulong32 getMainWindowHandle() = 0;
		virtual SoraWindowInfoBase* getMainWindow() = 0;
        
		virtual void flush() = 0;

		virtual bool isActive() = 0;
		virtual SoraWString videoInfo() = 0;

        virtual SoraShaderContext* createShaderContext() = 0;
		virtual	void attachShaderContext(SoraShaderContext* context) = 0;
		virtual void detachShaderContext() = 0;

		virtual ulong32 getVideoDeviceHandle() = 0;

		virtual void setWindowSize(int32 w, int32 h) = 0;
		virtual void setWindowTitle(const SoraWString& title) = 0;
		virtual void setWindowPos(int32 px, int32 py) = 0;
		virtual void setFullscreen(bool flag) = 0;
		virtual bool isFullscreen() = 0;
        virtual void setVerticalSync(bool flag) = 0;
        
        virtual void snapshot(const SoraString& path) = 0;

		virtual void setIcon(const SoraString& icon) = 0;
		virtual void setCursor(const SoraString& cursor) = 0;
        
        virtual void onExtensionStateChanged(int32 extension, bool state, int32 param) = 0;
	};

} // namespace sora

#endif // SORA_RENDER_SYSTEM_H
