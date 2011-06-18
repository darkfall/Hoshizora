/*
 *  SoraMutableSprite.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/10/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "SoraMutableSprite.h"

#include "../SoraCore.h"

namespace sora {

	SoraMutableSprite::SoraMutableSprite(HSORATEXTURE tex): SoraSprite(tex) {
		mChanged = false;
		
		mPixelData = sora::SORA->textureLock(tex);
	};
	
	uint32 SoraMutableSprite::getPixel(int32 x, int32 y) {
		if(!mPixelData)
			return 0;
		
		if(x >= 0 && x < getTextureWidth() &&  y >= 0 && y < getTextureHeight())  {
			return mPixelData[y*getTextureWidth() + x];
		}
		return 0;
	}
	
	void SoraMutableSprite::putPixel(int32 x, int32 y, uint32 col) {
		if(!mPixelData)
			return;
		
		if(x >= 0 && x < getTextureWidth() &&  y >= 0 && y < getTextureHeight())  {
			mPixelData[y*getTextureWidth() + x] = col;
			mChanged = true;
		}
	}
	
	void SoraMutableSprite::putPixelArray(int32 x, int32 y, int32 width, int32 height, uint32* col) {
		if(!mPixelData)
			return;
		
		if(x >= 0 && x+width < getTextureWidth() &&  y >= 0 && y+height < getTextureHeight())  {
			for(int i=0; i<height; ++i) {
				memcpy(&mPixelData[(y+i)*getTextureWidth()+x], &col[i*width], sizeof(uint32)*width);
				mChanged = true;
			}
		}
	}
	
	void SoraMutableSprite::applyChange() {
		mChanged = false;
		SORA->textureUnlock((HSORATEXTURE)quad.tex);
		mPixelData = SORA->textureLock((HSORATEXTURE)quad.tex);
	}
	
	void SoraMutableSprite::render() {
		if(mChanged)
			applyChange();
		
		SoraSprite::render();
	}
	
	void SoraMutableSprite::render(float32 x, float32 y) {
		if(mChanged)
			applyChange();
		
		SoraSprite::render(x, y);
	}
	
	void SoraMutableSprite::render4V(float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3, float32 x4, float32 y4) {
		if(mChanged)
			applyChange();
		
		SoraSprite::render4V(x1, y1,
							 x2, y2,
							 x3, y3,
							 x4, y4);
	}
	
	void SoraMutableSprite::renderWithVertices(SoraVertex* vertices, uint32 size, int32 mode) {
		if(mChanged)
			applyChange();
		
		SoraSprite::renderWithVertices(vertices, size, mode);
	}
	

} // namespace sora