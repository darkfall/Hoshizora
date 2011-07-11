/*
 *  SoraSpriteFont.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/11/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_SPRITE_FONT_H_
#define SORA_SPRITE_FONT_H_

#include "../SoraFont.h"

namespace sora {
	
	class SORA_API SoraSpriteFont: public SoraFont {
	public:
		SoraSpriteFont(const SoraWString& font, const SoraString& sequance, int32 width, int32 height);
		~SoraSpriteFont();
		
		void	render(float32 x, float32 y, const wchar_t* text, bool hcenter = false, bool vcenter = false);
		void	print(float32 x, float32 y, int32 align, const wchar_t *format, ...);
		void	render(float32 x, float32 y, int32 align, const wchar_t* text);
		
		void	setColor(ulong32 dwColor, int32 i = -1);
		ulong32	getColor(int32 i);
		
		void	setKerningWidth(float32 kerning);
		void	setKerningHeight(float32 kerning);
		
		float32	getKerningWidth();
		float32	getKerningHeight();
		
		float32	getStringWidth(const wchar_t* text);
		float32 getStringHeight(const wchar_t* text);
		float32	getHeight();
		
		uint32	getFontSize();
		
		float32	getWidthFromCharacter(wchar_t c, bool original = false);
		
		void setCharRotation(float32 rot);
		void setScale(float32 scale);

	private:
		inline void buildPosMap(const SoraString&);
		
		SoraSprite* mFontSprite;
		
		int32 mCharPos[128];
		
		int32 mWidth, mHeight;
		float32 mRotation, mScale;
		float32 mKerningWidth, mKerningHeight;
	}; 
	
	
} // namespace sora


#endif // SORA_SPRITE_FONT_H_