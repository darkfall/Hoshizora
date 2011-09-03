/*
 *  SoraCompressedTexture.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraCompressedTexture.h"
#include "SoraPNGOptimizer.h"

namespace sora {

	SoraCompressedTexture::SoraCompressedTexture(int32 maxWidth, int32 maxHeight):
	mTexture(0),
	mMaxWidth(maxWidth),
	mMaxHeight(maxHeight),
	mTextureWidth(0),
	mTextureHeight(0) {
	}
	
	SoraCompressedTexture::~SoraCompressedTexture() {
		if(mTexture != 0) {
			sora::SORA->releaseTexture(mTexture);
		}
		SPRITE_MAP::iterator itSpr = mSpriteMap.begin();
		while(itSpr != mSpriteMap.end()) {
			if(itSpr->second.mBuilt) {
				if(itSpr->second.mSprite) {
					delete itSpr->second.mSprite;
					itSpr->second.mSprite = 0;
				}
			}
			++itSpr;
		}
	}
	
	int32 SoraCompressedTexture::getTextureWidth() const {
		if(mTextureWidth == 0)
			return mPlacementRect.GetW();
		return mTextureWidth;
	}
	
	int32 SoraCompressedTexture::getTextureHeight() const {
		if(mTextureHeight != 0)
			return mPlacementRect.GetH();
		return mTextureHeight;
	}
	
	void SoraCompressedTexture::setMaxSize(int32 maxW, int32 maxH) {
		mMaxWidth = maxW;
		mMaxHeight = maxH;
	}
	
	bool SoraCompressedTexture::addSprite(SoraSprite* spr, const SoraString& descriptor) {
		if(!spr)
			return false;
		
		CRectPlacement::TRect rect(0.f, 0.f, spr->getSpriteWidth(), spr->getSpriteHeight());
		if(mPlacementRect.AddAtEmptySpotAutoGrow(&rect, mMaxWidth, mMaxHeight)) {
			PLACED_SPRITE place;
			place.mSprite = spr;
			place.mDestRect = rect;
			place.mBuilt = false;
			mSpriteMap[descriptor] = place;
			
			return true;
		}
		return false;
	}
	
	void SoraCompressedTexture::delSprite(const SoraString& descriptor) {
		SPRITE_MAP::iterator itSprite = mSpriteMap.find(descriptor);
		if(itSprite != mSpriteMap.end()) {
			mSpriteMap.erase(itSprite);
		}
	}
	
	SoraSprite* SoraCompressedTexture::getSprite(const SoraString& descriptor) {
		SPRITE_MAP::iterator itSprite = mSpriteMap.find(descriptor);
		if(itSprite != mSpriteMap.end()) {
			if(itSprite->second.mBuilt)
				return itSprite->second.mSprite;
			else if(mTexture != 0) {
				CRectPlacement::TRect rect = itSprite->second.mDestRect;
				SoraSprite* newspr = new SoraSprite(mTexture,
													rect.x, 
													rect.y, 
													rect.w,
													rect.h);
				itSprite->second.mSprite = newspr;
				itSprite->second.mBuilt = true;
				return newspr;
			}
		}
		return NULL;
	}
	
	SoraRect SoraCompressedTexture::getSpriteRect(const SoraString& descriptor) {
		SPRITE_MAP::iterator itSprite = mSpriteMap.find(descriptor);
		if(itSprite != mSpriteMap.end()) {
			CRectPlacement::TRect rect = itSprite->second.mDestRect;
			return SoraRect(rect.x, rect.y, rect.w, rect.h);
		}
		return SoraRect();
	}
	
	HSORATEXTURE SoraCompressedTexture::getTexture() const {
		return mTexture;
	}
	
	HSORATEXTURE SoraCompressedTexture::compress() {
		mTextureWidth = 1; 
		while(mTextureWidth < mPlacementRect.GetW()) {
			mTextureWidth *= 2;
		}
		mTextureHeight = 1; 
		while(mTextureHeight < mPlacementRect.GetH()) {
			mTextureHeight *= 2;
		}
		
		if(mTexture)
			sora::SORA->releaseTexture(mTexture);
		mTexture = sora::SORA->createTextureWH(mTextureWidth, mTextureHeight);
		if(!mTexture)
			return 0;
		
		uint32* destCol = (sora::SORA->textureLock(mTexture));
		memset(destCol, 0, mTextureWidth*mTextureHeight*sizeof(uint32));
		
		SPRITE_MAP::iterator itSpr = mSpriteMap.begin();
		while(itSpr != mSpriteMap.end()) {
			SoraSprite* spr = itSpr->second.mSprite;
			CRectPlacement::TRect rect = itSpr->second.mDestRect;
			
			uint32* srcCol = spr->getPixelData();
			
			int32 destX = rect.x;
			int32 destY = rect.y;
			
			int32 pitch = spr->getTextureWidth(false);
			hgeRect srcRect = spr->getTextureRect();
			
			int32 srcX = (int32)srcRect.x1;
			int32 srcY = (int32)srcRect.y1;
			
			for(int j=0; j<spr->getSpriteHeight(); ++j) {
				memcpy(&destCol[destY*mTextureWidth+destX], 
					   &srcCol[srcY*pitch+srcX], 
					   sizeof(uint32)*(int)rect.w);
				
				destY++;
				srcY++;
			}
			spr->unlockPixelData();
			
			++itSpr;
		}
		sora::SORA->textureUnlock(mTexture);
		return mTexture;
	}
	
	bool SoraCompressedTexture::compressToFile(const SoraWString& file) {
		compress();
		if(mTexture != 0) {
			sora::SoraPNGOptimizer::optimizeTexAndWriteToFile(mTexture, file);
			
			FILE* desFile = sora_fopenw(file+L"des", "wb");
			if(desFile) {
				fwrite("CMPT", 4, 1, desFile);
				
				std::string strFile = ws2s(file);
				uint32 sfilesize = strFile.size();
				fwrite(&sfilesize, 4, 1, desFile);
				fwrite(strFile.c_str(), sfilesize, 1, desFile);
				
				uint32 size = mSpriteMap.size();
				fwrite(&size, 4, 1, desFile);
				
				SPRITE_MAP::iterator itSpr = mSpriteMap.begin();
				while(itSpr != mSpriteMap.end()) {
					CRectPlacement::TRect rect = itSpr->second.mDestRect;
					
					uint32 desSize = itSpr->first.size();
					fwrite(&desSize, 4, 1, desFile);
					fwrite(itSpr->first.c_str(), desSize, 1, desFile);
					
					fwrite(&rect.x, 4, 1, desFile);
					fwrite(&rect.y, 4, 1, desFile);
					fwrite(&rect.w, 4, 1, desFile);
					fwrite(&rect.h, 4, 1, desFile);
					
					++itSpr;
				}
				
				fclose(desFile);
				return true;
			}
		}
		return false;
	}
	
	bool SoraCompressedTexture::loadCompressedFile(const SoraWString& file) {
		FILE* desFile = sora_fopenw(file, "rb");
		if(desFile) {
			char header[5];
			fread(header, 4, 1, desFile); header[4] = '\0';
			if(strcmp(header, "CMPT") != 0)  {
				fclose(desFile);
				return false;
			}
			
			uint32 sfilesize;
			fread(&sfilesize, 4, 1, desFile);
			char* sfile = new char[sfilesize+1];
			fread(sfile, sfilesize, 1, desFile); sfile[sfilesize] = '\0';
			
			if(mTexture)
				sora::SORA->releaseTexture(mTexture);
			mTexture = sora::SORA->createTexture(s2ws(sfile));
			delete sfile;
			
			if(!mTexture) {
				fclose(desFile);
				return false;
			}
			
			uint32 size;
			fread(&size, 4, 1, desFile);
			for(uint32 i=0; i<size; ++i) {
				uint32 desSize;
				fread(&desSize, 4, 1, desFile);
				char* descriptor = new char[desSize+1];
				fread(descriptor, desSize, 1, desFile); descriptor[desSize] = '\0';
				
				CRectPlacement::TRect rect;
				fread(&rect.x, 4, 1, desFile);
				fread(&rect.y, 4, 1, desFile);
				fread(&rect.w, 4, 1, desFile);
				fread(&rect.h, 4, 1, desFile);
				
				PLACED_SPRITE slot;
				slot.mSprite = new SoraSprite(mTexture, rect.x, rect.y, rect.w, rect.h);
				slot.mDestRect = rect;
				slot.mBuilt = true;
				
				mSpriteMap[descriptor] = slot;
				delete descriptor;
			}
			
			fclose(desFile);
			return true;
		}
		return false;
	}


} // namespace sora