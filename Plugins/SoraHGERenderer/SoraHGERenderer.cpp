/*
 *  SoraHGERenderer.cpp
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoraHGERenderer.h"

#include "SoraStringConv.h"

#include "SoraFileUtility.h"
#include "SoraMath.h"
#include "Rect4V.h"
#include "SoraInfiniteRendererCallback.h"

#ifdef USE_SHADER
#include "SoraShader/SoraCGD3D9Shader.h"
#endif

#include "Debug/SoraInternalLogger.h"

namespace sora{
	
	SoraHGERenderer::SoraHGERenderer() {
		pHGE = hgeCreate(HGE_VERSION);
		
		if(!pHGE) {
			throw SORA_EXCEPTION("Error initiazing HGE");
		}

		currShader = 0;
	}

	SoraHGERenderer::~SoraHGERenderer() {
	//	shutdown();
	}

	void SoraHGERenderer::flush() {
		pHGE->_render_batch(false);
	}

	void SoraHGERenderer::shutdown() {
		if(pHGE) {
			pHGE->System_Shutdown();
			pHGE->Release();
		}
	}

	bool SoraHGERenderer::update() {
		return false;
	}

	void SoraHGERenderer::beginScene(ulong32 color, ulong32 target) {
		pHGE->Gfx_BeginScene(target);
		pHGE->Gfx_Clear(color);
	}

	void SoraHGERenderer::endScene() {
		currShader = 0;
		pHGE->Gfx_EndScene();
	}

	void SoraHGERenderer::start(SoraTimer* timer) {
		pTimer = timer;
		sora::g_timer = pTimer;
		assert(pTimer != 0);

		pHGE->System_Start();
		while(true) {
			MSG msg;
			if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) { 
				if (msg.message == WM_QUIT)	break;
				// TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;
			}
			if(g_timer->update()) {
				pHGE->_UpdateMouse();
				SORA->update();
				pHGE->_ClearQueue();
			}
		}
	}

	SoraWindowHandle SoraHGERenderer::createWindow(SoraWindowInfoBase* windowInfo) {
//		if(!windowInfo->isWindowSubWindow()) {
			// just for hge compablity, otherwise hge would fail to start
			pHGE->System_SetState(HGE_FRAMEFUNC, bool_updateFrame);
		
			pHGE->System_SetState(HGE_SCREENWIDTH, windowInfo->getWindowWidth());
			pHGE->System_SetState(HGE_SCREENHEIGHT, windowInfo->getWindowHeight());
			pHGE->System_SetState(HGE_WINDOWED, windowInfo->isWindowed());
			pHGE->System_SetState(HGE_HIDEMOUSE, windowInfo->hideMouse());
			pHGE->System_SetState(HGE_TITLE, windowInfo->getWindowName().c_str());
			pHGE->System_SetState(HGE_DONTSUSPEND, true);
			
			if(windowInfo->getIcon() != 0) {
				pHGE->System_SetState(HGE_ICON, windowInfo->getIcon());
			}
		
			if(windowInfo->getCursor() != 0) {
				pHGE->System_SetState(HGE_CURSOR, windowInfo->getCursor());
			}

			pHGE->System_SetState(HGE_ZBUFFER, true);
			pHGE->System_Initiate();

			windowInfo->init();

			pMainWindow = windowInfo;
			return (SoraWindowHandle)pMainWindow;
	//	}
		return 0;
	}

	void SoraHGERenderer::setWindowSize(int32 w, int32 h) {
		pHGE->_Resize(w, h);
		pHGE->_AdjustWindow();
	}

	void SoraHGERenderer::setWindowTitle(const SoraWString& title) {
		pHGE->System_SetState(HGE_TITLE, ws2s(title).c_str());
	}

	void SoraHGERenderer::setWindowPos(int32 px, int32 py) {
		return;
	}

	void SoraHGERenderer::setFullscreen(bool flag) {
		pHGE->System_SetState(HGE_WINDOWED, !flag);
	}

	bool SoraHGERenderer::isFullscreen() {
		return pHGE->System_GetState(HGE_WINDOWED);
	}

	ulong32 SoraHGERenderer::getMainWindowHandle() {
		return (ulong32)pHGE->System_GetState(HGE_HWND);
	}
	
	SoraWindowInfoBase* SoraHGERenderer::getMainWindow() {
		return pMainWindow;
	}
    
    SoraShaderContext* SoraHGERenderer::createShaderContext() {
#ifdef USE_SHADER
        return new SoraCGD3D9ShaderContext;
#endif
		return NULL;
    }

	void SoraHGERenderer::attachShaderContext(SoraShaderContext* context) {
		currShader = context;
	}

	void SoraHGERenderer::detachShaderContext() {
		if(currShader != NULL) {
			currShader->detachShaderList();
			currShader = 0;
		}
	}

	SoraTexture* SoraHGERenderer::createTexture(const SoraWString& sTexturePath, bool bMipmap) {
		HTEXTURE htex = pHGE->Texture_Load(ws2s(sTexturePath).c_str(), 0UL, bMipmap);
		if(htex) {
			SoraTexture* tex = new SoraTexture;
			tex->mTextureID = static_cast<ulong32>(htex);
			tex->mTextureWidth		= pHGE->Texture_GetWidth(htex);
			tex->mTextureHeight		= pHGE->Texture_GetHeight(htex);
			tex->mOriginalWidth		= pHGE->Texture_GetWidth(htex, true);
			tex->mOriginalHeight		= pHGE->Texture_GetHeight(htex, true);
			return tex;
		} else
			throw SORA_EXCEPTION("Error creating Texture");
		return 0;
	}
	
	// to do
	SoraTexture* SoraHGERenderer::createTextureFromMem(void* ptr, ulong32 size, bool bMipmap) {
		HTEXTURE htex = pHGE->Texture_Load(static_cast<const char*>(ptr), size, bMipmap);
		if(htex) {
			SoraTexture* tex = new SoraTexture;
			tex->mTextureID = static_cast<ulong32>(htex);
			tex->mTextureWidth		= pHGE->Texture_GetWidth(htex);
			tex->mTextureHeight		= pHGE->Texture_GetHeight(htex);	
			tex->mOriginalWidth		= pHGE->Texture_GetWidth(htex, true);
			tex->mOriginalHeight	= pHGE->Texture_GetHeight(htex, true);
			return tex;
		} else 
			throw SORA_EXCEPTION("Error creating texture");
		return 0;
	}

	SoraTexture* SoraHGERenderer::createTextureWH(int w, int h) {
		HTEXTURE htex = pHGE->Texture_Create(w, h);
		if(htex) {
			SoraTexture* tex = new SoraTexture;
			tex->mTextureID = static_cast<ulong32>(htex);
			tex->mTextureWidth		= w;
			tex->mTextureHeight		= h;
			tex->mOriginalHeight	= h;
			tex->mOriginalWidth		= w;
			return tex;
		} else
			throw SORA_EXCEPTION("Error creating Texture");
		return 0;
	}

	SoraTexture* SoraHGERenderer::createTextureFromRawData(unsigned int* data, int32 w, int32 h) {
		HTEXTURE htex = pHGE->Texture_Create(w, h);
		unsigned long* td = pHGE->Texture_Lock(htex, false, 0, 0, w, h);
		if(!td) return 0;
		memcpy(td, data, 4*w*h);
		pHGE->Texture_Unlock(htex);
		if(htex) {
			SoraTexture* tex = new SoraTexture;
			tex->mTextureID = static_cast<ulong32>(htex);
			tex->mTextureWidth		= w;
			tex->mTextureHeight		= h;
			tex->mOriginalHeight	= h;
			tex->mOriginalWidth		= w;
			return tex;
		} else
			throw SORA_EXCEPTION("Error creating Texture");
		return 0;
	}

	uint32* SoraHGERenderer::textureLock(SoraTexture* ht) {
		return (uint32*)pHGE->Texture_Lock(ht->mTextureID, false, 0, 0, ht->mOriginalWidth, ht->mOriginalHeight);
	}

	void SoraHGERenderer::textureUnlock(SoraTexture* h) {
		pHGE->Texture_Unlock(h->mTextureID);
	}

	void SoraHGERenderer::releaseTexture(SoraTexture* tex) {
		if(tex) {
			if(tex->mTextureID) {
				pHGE->Texture_Free(static_cast<HTEXTURE>(tex->mTextureID));
			}
			delete tex;
			tex = 0;
		}
	}

	void SoraHGERenderer::renderQuad(SoraQuad& quad) {
		hgeQuad hquad;
		if(quad.tex) {
			memcpy(hquad.v, quad.v, sizeof(hgeVertex)*4);
			hquad.tex = ((SoraTexture*)quad.tex)->mTextureID;
			hquad.blend = quad.blend;
		}
		else {
			hquad.tex = 0;
			memcpy(hquad.v, quad.v, sizeof(hgeVertex)*4);
			hquad.blend = quad.blend;
		}
		
		pHGE->Gfx_RenderQuad(&hquad);
			
		if(currShader) {
			currShader->attachShaderList();
			pHGE->_render_batch();
		}
	}

	void SoraHGERenderer::renderTriple(SoraTriple& trip) {
		hgeTriple htrip;
		memcpy(&htrip.v[0], &trip.v[0], sizeof(hgeVertex)*3);
		htrip.tex = ((SoraTexture*)trip.tex)->mTextureID;
		htrip.blend = trip.blend;
		pHGE->Gfx_RenderTriple(&htrip);

		if(currShader) {
			currShader->attachShaderList();
			pHGE->_render_batch();
		}
	}

	int32 SoraHGERenderer::_modeToDXMode(int32 mode) {
		switch(mode) {
			case SORA_LINE: return HGEPRIM_LINES;
			case SORA_TRIANGLES: return HGEPRIM_TRIPLES;
			case SORA_TRIANGLES_FAN: return HGEPRIM_TRIPLES_FAN;
			case SORA_TRIANGLES_STRIP: return HGEPRIM_TRIPLES_STRIP;
			case SORA_QUAD: return HGEPRIM_QUADS;
		}
		return HGEPRIM_TRIPLES;
	}

	void SoraHGERenderer::renderWithVertices(SoraTexture* tex, int32 blendMode, SoraVertex* vertices, uint32 size, int32 mode) {
		int32 maxSize;
		hgeVertex* vtxBuffer = pHGE->Gfx_StartBatch(_modeToDXMode(mode), tex->mTextureID, blendMode, &maxSize);
		memcpy(&vtxBuffer[0], vertices, sizeof(hgeVertex)*size);
		pHGE->Gfx_FinishBatch(size);
		pHGE->_render_batch();
	}

	bool SoraHGERenderer::isActive() {
		return pHGE->System_GetState(HGE_ACTIVE);
	}

	void SoraHGERenderer::setClipping(int32 x, int32 y, int32 w, int32 h) {
		pHGE->Gfx_SetClipping(x, y, w, h);
	}

	void SoraHGERenderer::setTransform(float32 x, float32 y, float32 dx, float32 dy, float32 rot, float32 hscale, float32 vscale) {
		pHGE->Gfx_SetTransform(x, y, dx, dy, rot, hscale, vscale);
	}
	
	void SoraHGERenderer::setTransformWindowSize(float32 w, float32 h) {
		return;
	}

	ulong32 SoraHGERenderer::createTarget(int width, int height, bool zbuffer) {
		return pHGE->Target_Create(width, height, zbuffer);
	}

	void SoraHGERenderer::freeTarget(ulong32 t) {
		pHGE->Target_Free(t);
	}

	ulong32 SoraHGERenderer::getTargetTexture(ulong32 t) {
		HTEXTURE tex = pHGE->Target_GetTexture(t);
		SoraTexture* ptex = new SoraTexture(tex, pHGE->Texture_GetWidth(tex),
												pHGE->Texture_GetHeight(tex),
												pHGE->Texture_GetWidth(tex, true),
												pHGE->Texture_GetHeight(tex, true));
		return (ulong32)ptex;
	}

	SoraWString SoraHGERenderer::videoInfo() {
		SoraWString str(L"Driver: HGE+DX9");
		return str;
	}

	ulong32 SoraHGERenderer::getVideoDeviceHandle() {
		return pHGE->getVideoDeviceHandle();
	}
    
    void SoraHGERenderer::snapshot(const SoraString& path) {
        pHGE->System_Snapshot(path.c_str());
    }
    
    void SoraHGERenderer::setViewPoint(float32 x, float32 y, float32 z) {
        
    }

	void SoraHGERenderer::setIcon(const SoraString& icon) {
		pHGE->System_SetState(HGE_ICON, icon.c_str());
	}

	void SoraHGERenderer::setCursor(const SoraString& cursor) {
		pHGE->System_SetState(HGE_CURSOR, cursor.c_str());
	}
} // namespace sora