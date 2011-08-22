#ifndef SORA_GRAPHIC_EFFECT_H
#define SORA_GRAPHIC_EFFECT_H

#include "SoraPlatform.h"
#include "SoraException.h"

#include "SoraSprite.h"
#include "SoraTexture.h"

#include "SoraCore.h"

#include "graphicEffects.h"

namespace sora {

	class SoraGraphicEffect {
	public:
		static SoraSprite* gaussBlurNew(SoraSprite* in, double sigma, int radius) {
			SoraTexture* tex = in->getTexture();
			
			SoraTexture* outtex = (SoraTexture*)SoraCore::Instance()->createTextureWH(tex->mTextureWidth, tex->mTextureHeight);
			uint32* outputData = (uint32*)malloc(tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
			uint32* data = in->getPixelData();
			memcpy(outputData, data, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
			in->unlockPixelData();
			if(!::gaussBlur(outputData, (int32)tex->mTextureWidth, (int32)tex->mTextureHeight, sigma, radius))
				THROW_SORA_EXCEPTION(RuntimeException, "failed gauss blur sprite");
			SoraSprite* out = (SoraSprite*)SoraCore::Instance()->createSpriteTex((HSORATEXTURE)outtex);
			uint32* odata = out->getPixelData(false);
			memcpy(odata, outputData, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
			free(outputData);
			out->unlockPixelData();
			if(!out)
				THROW_SORA_EXCEPTION(RuntimeException, "cannot alloc sprite for output");
			
			return out;
		}

		static void gaussBlur(SoraSprite* in, double sigma, int radius) {
			uint32* data = in->getPixelData(false);
			if(!::gaussBlur(data, (int32)in->getTextureWidth(), (int32)in->getTextureHeight(), sigma, radius))
				THROW_SORA_EXCEPTION(RuntimeException, "failed gauss blur sprite");
			in->unlockPixelData();
		}

		static SoraSprite* grayNew(SoraSprite* in) {
			SoraTexture* tex = in->getTexture();
			
			SoraTexture* outtex = (SoraTexture*)SoraCore::Instance()->createTextureWH(tex->mTextureWidth, tex->mTextureHeight);
			uint32* outputData = (uint32*)malloc(tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
			uint32* data = in->getPixelData();
			memcpy(outputData, data, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
			in->unlockPixelData();
			if(!::gray(outputData, (int32)tex->mTextureWidth, (int32)tex->mTextureHeight))
				THROW_SORA_EXCEPTION(RuntimeException, "failed gray sprite");
			SoraSprite* out = (SoraSprite*)SoraCore::Instance()->createSpriteTex((HSORATEXTURE)outtex);
			uint32* odata = out->getPixelData(false);
			memcpy(odata, outputData, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
			free(outputData);
			out->unlockPixelData();
			if(!out)
				THROW_SORA_EXCEPTION(RuntimeException, "cannot alloc sprite for output");
			
			return out;
		}

		static void gray(SoraSprite* in) {
			uint32* data = in->getPixelData(false);
			if(!::gray(data, (int32)in->getTextureWidth(), (int32)in->getTextureHeight()))
				THROW_SORA_EXCEPTION(RuntimeException, "failed gray sprite");
			in->unlockPixelData();
		}

		static SoraSprite* reverseNew(SoraSprite* in) {
			SoraTexture* tex = in->getTexture();
			
			SoraTexture* outtex = (SoraTexture*)SoraCore::Instance()->createTextureWH(tex->mTextureWidth, tex->mTextureHeight);
			uint32* outputData = (uint32*)malloc(tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
			uint32* data = in->getPixelData();
			memcpy(outputData, data, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
			in->unlockPixelData();
			if(!::reverse(outputData, (int32)tex->mTextureWidth, (int32)tex->mTextureHeight))
				THROW_SORA_EXCEPTION(RuntimeException, "failed reverse sprite");
			SoraSprite* out = (SoraSprite*)SoraCore::Instance()->createSpriteTex((HSORATEXTURE)outtex);
			uint32* odata = out->getPixelData(false);
			memcpy(odata, outputData, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
			free(outputData);
			out->unlockPixelData();
			if(!out)
				THROW_SORA_EXCEPTION(RuntimeException, "cannot alloc sprite for output");
			
			return out;
		}

		static void reverse(SoraSprite* in) {
			uint32* data = in->getPixelData(false);
			if(!::reverse(data, (int32)in->getTextureWidth(), (int32)in->getTextureHeight()))
				THROW_SORA_EXCEPTION(RuntimeException, "failed reverse sprite");
			in->unlockPixelData();
		}

		static SoraSprite* alphaMixNew(SoraSprite* in, SoraSprite* src, double a, int posx=0, int posy=0) {
			SoraTexture* tex = in->getTexture();
			
			SoraTexture* outtex = (SoraTexture*)SoraCore::Instance()->createTextureWH(tex->mTextureWidth, tex->mTextureHeight);
			uint32* outputData = (uint32*)malloc(tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
			uint32* data = in->getPixelData();
			memcpy(outputData, data, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
			in->unlockPixelData();
			if(!::alphaMix(outputData, src->getPixelData(), (int32)tex->mTextureWidth, (int32)tex->mTextureHeight, 
															(int32)src->getTextureWidth(), (int32)src->getTextureHeight(), a, posx, posy))
				THROW_SORA_EXCEPTION(RuntimeException, "failed alphaMix sprite");
			SoraSprite* out = (SoraSprite*)SoraCore::Instance()->createSpriteTex((HSORATEXTURE)outtex);
			uint32* odata = out->getPixelData(false);
			memcpy(odata, outputData, tex->mTextureHeight*tex->mTextureWidth*sizeof(uint32));
			free(outputData);
			out->unlockPixelData();
			src->unlockPixelData();
			if(!out)
				THROW_SORA_EXCEPTION(RuntimeException, "cannot alloc sprite for output");
			
			return out;
		}

		static void alphaMix(SoraSprite* in, SoraSprite* src, double a, int posx=0, int posy=0) {
			uint32* data1 = in->getPixelData(false);
			uint32* data2 = src->getPixelData();
			if(!::alphaMix(data1, data2, (int32)in->getTextureWidth(), (int32)in->getTextureHeight(), 
										 (int32)src->getTextureWidth(), (int32)src->getTextureHeight(),
										  a, posx, posy))
				THROW_SORA_EXCEPTION(RuntimeException, "failed reverse sprite");
			in->unlockPixelData();
			src->unlockPixelData();
		}
	};

} // namespace sora

#endif