/*
 *  SoraPSPGLRenderer.cpp
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/23/10.
 *  Copyright 2010 Griffin Bu. All rights reserved.
 *
 */

#include "SoraPSPGLRenderer.h"
#include "SoraPSPGLRenderTarget.h"

#include "SoraStringConv.h"
#include "SoraFileUtility.h"
#include "SoraMath.h"
#include "Rect4V.h"
#include "SoraColor.h"

#include "SoraCore.h"
#include "SoraInfiniteRendererCallback.h"
#include "Debug/SoraInternalLogger.h"
#include "SOIL/SOIL.h"


#ifdef OS_PSP
#include "JGE/include/JGE.h"
#include "JGE/include/JTypes.h"
#include "JGE/include/JRenderer.h"

// bridge to jge
#include "SoraJGEApp.h"

#endif

namespace sora{

	SoraPSPGLRenderer::SoraPSPGLRenderer() {
		pCurTarget = 0;
		
		currShader = 0;
		iFrameStart = 1;
	}

	SoraPSPGLRenderer::~SoraPSPGLRenderer() {
	//	shutdown();
		JGE::GetInstance()->End();
		JGE::GetInstance()->Destroy();
	}

	void SoraPSPGLRenderer::shutdown() {
		std::list<SoraRenderTarget*>::iterator itt = liTargets.begin();
		while(itt != liTargets.end()) {
			delete (*itt);
			++itt;
		}
		liTargets.clear();
		
		//delete mainWindow;
	}
	
	bool SoraPSPGLRenderer::update() {
		//clearPoll();
		return false;
	}
	
	void SoraPSPGLRenderer::start(SoraTimer* timer) {
		JGEApp->SetTimer(timer);
		JGE::GetInstance()->Run();	
	}

	void SoraPSPGLRenderer::beginFrame() {
	}
	
	void SoraPSPGLRenderer::endFrame() {
	}
	
	void SoraPSPGLRenderer::flush() {
	}
	
	void SoraPSPGLRenderer::beginScene(ulong32 color, ulong32 target) {
		JGERenderer->BeginScene();
		//JGERenderer->ClearScreen((PIXEL_TYPE)color);
	}

	void SoraPSPGLRenderer::endScene() {
		JGERenderer->EndScene();
	}

	SoraWindowHandle SoraPSPGLRenderer::createWindow(SoraWindowInfoBase* windowInfo) {
		mainWindow = windowInfo;
		
		JGEApp = new SoraJGEApp();
		JGE* jge = JGE::GetInstance();
		
		JGERenderer = JRenderer::GetInstance();
		JRenderer::Set3DFlag(false);
		
		jge->Init();
		jge->SetApp(JGEApp);
		
		
		windowInfo->init();		

		return (ulong32)windowInfo;
		
		return 0;
	}
	
	void SoraPSPGLRenderer::setWindowSize(int32 w, int32 h) {
	//	glfwSetWindowSize(w, h);
	}
	
	void SoraPSPGLRenderer::setWindowTitle(const SoraWString& title) {
	//	glfwSetWindowTitle(ws2s(title).c_str());
	}
	
	void SoraPSPGLRenderer::setWindowPos(int32 px, int32 py) {
	//	glfwSetWindowPos(px, py);
	}
	
	void SoraPSPGLRenderer::setFullscreen(bool flag) {
	}
	
	bool SoraPSPGLRenderer::isFullscreen() {
		return true;
	}
	
	ulong32 SoraPSPGLRenderer::getVideoDeviceHandle() {
		return (ulong32)this;
	}
	
	SoraTexture* SoraPSPGLRenderer::createTexture(const SoraWString& sTexturePath, bool bMipmap) {
		JTexture* jtex = JGERenderer->LoadTexture(ws2sfast(sTexturePath).c_str(), bMipmap?1:0);
		SoraTexture* stex = new SoraTexture(jtex->mTexId,
											jtex->mWidth,
											jtex->mHeight,
											jtex->mTexWidth,
											jtex->mTexHeight);
											
		mTextureMap[stex] = jtex;
		return stex;
	}
	
	// to do
	SoraTexture* SoraPSPGLRenderer::createTextureFromMem(void* ptr, ulong32 size, bool bMipmap) {
		return 0;
	}

	SoraTexture* SoraPSPGLRenderer::createTextureWH(int w, int h) {
		JTexture* jtex = JGERenderer->CreateTexture(w, h, false);
		SoraTexture* stex = new SoraTexture(jtex->mTexId,
											jtex->mWidth,
											jtex->mHeight,
											jtex->mTexWidth,
											jtex->mTexHeight);
											
		mTextureMap[stex] = jtex;
		return stex;
	}
	
	SoraTexture* SoraPSPGLRenderer::createTextureFromRawData(unsigned int* data, int32 w, int32 h) {
		return 0;
	}

	uint32* SoraPSPGLRenderer::textureLock(SoraTexture* ht) {
		TEXTURE_MAP::iterator itJTex = mTextureMap.find(ht);
		if(itJTex != mTextureMap.end()) {
			return (uint32*)itJTex->second->mBits;
		}
		return 0;
	}
	
	void SoraPSPGLRenderer::textureUnlock(SoraTexture* ht) {
		
	}

	void SoraPSPGLRenderer::releaseTexture(SoraTexture* tex) {
		TEXTURE_MAP::iterator itJTex = mTextureMap.find(tex);
		if(itJTex != mTextureMap.end()) {
			delete itJTex->second;
			delete itJTex->first;
			mTextureMap.erase(itJTex);
		}
	}
	
	void SoraPSPGLRenderer::renderTriple(SoraTriple& trip) {
		TEXTURE_MAP::iterator itJTex = mTextureMap.find(trip.tex);
		if(itJTex != mTextureMap.end()) { 
			Vertex3D vertices[3];
			
			SoraTexture* stex = trip.tex;
			for(int i=0; i<3; ++i) {
				vertices[i].x = trip.v[i].x;
				vertices[i].x = trip.v[i].y;
				vertices[i].x = trip.v[i].z;
				vertices[i].u = trip.v[i].tx;
				vertices[i].v = trip.v[i].ty;
			}
			
			JGERenderer->RenderTriangles(itJTex->second, vertices, 0, 1);
		}	
	}
	
	void SoraPSPGLRenderer::renderWithVertices(SoraTexture* tex, int32 blendMode,  SoraVertex* vertices, uint32 vsize, int32 mode) {		
		TEXTURE_MAP::iterator itJTex = mTextureMap.find(tex);
		if(itJTex != mTextureMap.end()) { 
			Vertex3D* jvert = new Vertex3D[vsize];
			
			for(int i=0; i<vsize; ++i) {
				jvert[i].x = vertices[i].x;
				jvert[i].x = vertices[i].y;
				jvert[i].x = vertices[i].z;
				jvert[i].u = vertices[i].tx;
				jvert[i].v = vertices[i].ty;
			}
			
			switch(mode) {
				case SORA_TRIANGLES:
					JGERenderer->RenderTriangles(itJTex->second, jvert, 0, vsize/3); break;
				case SORA_TRIANGLES_STRIP:
				case SORA_TRIANGLES_FAN:
				case SORA_QUAD:
					JGERenderer->RenderTriangles(itJTex->second, jvert, 0, vsize-2); break;
			}
		}
	}
	
	void SoraPSPGLRenderer::renderQuad(SoraQuad& quad) {
		
	}

	bool SoraPSPGLRenderer::isActive() {
		return true;
	}

	void SoraPSPGLRenderer::setClipping(int32 x, int32 y, int32 w, int32 h) {
		JGERenderer->SetClip(x, y, w, h);
	}
	
	void SoraPSPGLRenderer::setTransformWindowSize(float32 w, float32 h) {
		
	}

	void SoraPSPGLRenderer::setTransform(float32 x, float32 y, float32 dx, float32 dy, float32 rot, float32 hscale, float32 vscale) {
		
	}

	ulong32 SoraPSPGLRenderer::createTarget(int width, int height, bool zbuffer) {
		SoraRenderTargetPSPGL* t = new SoraRenderTargetPSPGL(width, height, zbuffer);
		liTargets.push_back((SoraRenderTarget*)t);
		return (ulong32)t;
	}

	void SoraPSPGLRenderer::freeTarget(ulong32 t) {
		SoraRenderTarget* pt = (SoraRenderTarget*)t;
		if(!pt) return;
		std::list<SoraRenderTarget*>::iterator itt = liTargets.begin();
		while(itt != liTargets.end()) {
			if((*itt) == pt) {
				delete pt;
				liTargets.erase(itt);
				break;
			}
			++itt;
		}
	}

	ulong32 SoraPSPGLRenderer::getTargetTexture(ulong32 t) {
		SoraRenderTargetPSPGL* pt = (SoraRenderTargetPSPGL*)t;
        assert(pt != NULL);
		
		return pt->getTexture();
	}
	
	SoraWString SoraPSPGLRenderer::videoInfo() {
		SoraWString info(L"Driver=JGE Version=1.0");
		return info;
	}
	
	SoraShaderContext* SoraPSPGLRenderer::createShaderContext() {
		return NULL;
	}
	
	void SoraPSPGLRenderer::snapshot(const SoraString& file) {
		JGERenderer->ScreenShot(file.c_str());
	}
	
	void SoraPSPGLRenderer::setViewPoint(float, float, float) {
	}

	void SoraPSPGLRenderer::attachShaderContext(SoraShaderContext* context) {
		currShader = context;
	}

	void SoraPSPGLRenderer::detachShaderContext() {
		if(!currShader->detachShaderList())
			SoraCore::Instance()->log("SoraPSPGLRenderer: error detaching shader list");
		flush();
		currShader = 0;
	}

} // namespace sora