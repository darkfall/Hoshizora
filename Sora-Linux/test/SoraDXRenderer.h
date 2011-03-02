#ifndef SORA_DX_RENDERER_H_
#define SORA_DX_RENDERER_H_

#include "SoraRenderSystem.h"
#include "SoraSprite.h"
#include "SoraTexture.h"

namespace sora {
	/** 
		DirectX Renderer Implemention
	*/

	class SoraDXRenderSystem {
	public:
		SoraDXRenderSystem();
		virtual ~SoraDXRenderSystem();

		bool update();

		void beginScene(ulong32 c, ulong32 t);
		void endScene();

		void start(SoraTimer* timer);

		ulong32 createTarget(int width, int height, bool zbuffer=true);
		void	freeTarget(ulong32 t);
		ulong32 getTargetTexture(ulong32 t);
		
		SoraWindowHandle createWindow(SoraWindowInfoBase* windowInfo);
		
		SoraTexture* createTexture(const SoraWString& sTexturePath, bool bMipmap=false);
		SoraTexture* createTextureWH(int32 w, int32 h);
		SoraTexture* createTextureFromMem(void* ptr, ulong32 size, bool bMipmap=false);
		SoraTexture* createTextureFromRawData(unsigned int* data, int32 w, int32 h);

		ulong32*  	 textureLock(SoraTexture* ht, bool bReadOnly=true, uint32 x=0, uint32 y=0, uint32 w=0, uint32 h=0);
		void		 textureUnlock(SoraTexture*);
		void		 releaseTexture(SoraTexture* pTexture);

		SoraSprite* createSprite(const SoraWString& sPath);
		SoraSprite* createSprite (SoraTexture* tex);
		void renderSprite(SoraSprite* pSprite, float32 x, float32 y);
		void renderSprite4V(SoraSprite* pSprite, float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3, float32 x4, float32 y4);
		void renderQuad(SoraQuad& quad);
		
		void renderRect(float32 x1, float32 y1, float32 x2, float32 y2, float32 fWidth, DWORD color=0xFFFFFFFF, float32 z=0.5f);
		void setClipping(int32 x=0, int32 y=0, int32 w=0, int32 h=0) { }
		void setTransform(float32 x=0.f, float32 y=0.f, float32 dx=0.f, float32 dy=0.f, float32 rot=0.f, float32 hscale=0.f, float32 vscale=0.f);

		void shutdown();
		ulong32 getMainWindowHandle();
		SoraWindowInfoBase* getMainWindow();
		
		bool isActive();
		
		SoraString videoInfo();
	};

} // namespace sora


#endif // SORA_DX_RENDERER_H