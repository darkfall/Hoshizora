/*
 *  SoraiOSFont.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#ifndef SORA_IOS_FONT_H_
#define SORA_IOS_FONT_H_

#include "SoraFont.h"
#include "SoraSprite.h"

#ifdef OS_IOS

// for [Texture2D initWithString]
#include "SoraOGLESRenderer/Apple/Texture2D.h"
#include "stringId.h"

namespace sora {
	
	class iOSFontGlyph {
	public:
		iOSFontGlyph();
		virtual ~iOSFontGlyph();
		
		bool cached;
		// cache a char or str
		// return the stringId
		inline stringId cache(const SoraWString& str, const SoraString& fontName, uint32 fontSize);
		
		void release();
		
		HSORATEXTURE tex;
		Texture2D* tex2D;
		stringId sid;
		int32 width;
	};
	
	class SoraiOSFont: SoraFont {
		friend class SoraiOSFontManager;
		
	protected:
		SoraiOSFont(const SoraString& fontName, uint32 fontSize);
		
	public:
		~SoraiOSFont();
		
		void	render(float32 x, float32 y, const wchar_t* text, bool hcenter = false, bool vcenter = false);
		// render with alignments
		void	print(float32 x, float32 y, int32 align, const wchar_t *format, ...);
		void	render(float32 x, float32 y, int32 align, const wchar_t* text);
		
		void	setColor(ulong32 dwColor, int32 i = -1);
		ulong32	getColor(int32 i);
		
		// set kernings
		void	setKerningWidth(float32 kerning);
		void	setKerningHeight(float32 kerning);
		
		// get kernings
		float32	getKerningWidth();
		float32	getKerningHeight();
		
		// get font dimensions
		float32	getStringWidth(const wchar_t* text);
		float32	getHeight();
		
		// get font size
		uint32	getFontSize();
		
		// get width of a char
		float32	getWidthFromCharacter(wchar_t c, bool original = false);
		
		// font effects
		void setCharRotation(float32 rot);
		void setScale(float32 scale);
		
	private:
		SoraSprite* pfSpr;
		SoraCore* CORE;
		
		float32 kerningWidth;
		float32 kerningHeight;
		
		uint32 size;
		SoraString fontName;
		hash_map<ulong32, iOSFontGlyph> ft_glyphs;
		
		float32 charRotation;
		float32 scale;
	};
	
} // namespace sora

#endif // OS_IOS

#endif // SORA_IOS_FONT_H