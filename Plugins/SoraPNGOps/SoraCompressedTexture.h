/*
 *  SoraCompressedTexture.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_COMPRESSED_TEXTURE_H_
#define SORA_COMPRESSED_TEXTURE_H_

#include "SoraCore.h"
#include "RectPlacement.h"
#include "SoraMath.h"

namespace sora {
	
	/*
	 a texture that compresses multiple sprites into one texture and a description file
	 can be saved and load
	 */
	class SoraCompressedTexture {
	public:
		SoraCompressedTexture(int32 maxWidth, int32 maxHeight);
		~SoraCompressedTexture();
		
		void setMaxSize(int32 maxW, int32 maxH);
		
		int32 getTextureWidth() const;
		int32 getTextureHeight() const;
		
		/*
		 only data within spr's texturerect would be compressed
		 @param spr, the sprite to be added
		 @param descriptor, the description of the sprite, 
			use to get sprite from this texture when load a description file
		 */
		bool addSprite(SoraSprite* spr, const SoraString& descriptor);
		
		SoraRect getSpriteRect(const SoraString& descriptor);
		void delSprite(const SoraString& descriptor);
		
		SoraSprite* getSprite(const SoraString& descriptor);
		
		HSORATEXTURE compress();
		HSORATEXTURE getTexture() const;
		
		bool compressToFile(const SoraWString& file);
		bool loadCompressedFile(const SoraWString& file);
		
	private:
		typedef struct {
			SoraSprite* mSprite;
			CRectPlacement::TRect mDestRect;
			
			bool mBuilt;
		} PLACED_SPRITE;
		typedef std::map<SoraString, PLACED_SPRITE> SPRITE_MAP;
		SPRITE_MAP mSpriteMap;
		
		HSORATEXTURE mTexture;
		int32 mTextureWidth;
		int32 mTextureHeight;
		
		int32 mMaxWidth;
		int32 mMaxHeight;
		
		CRectPlacement mPlacementRect;
	};
	
}


#endif // SORA_COMPRESSES_TEXTURE_H_