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
#include "SoraiOSGLRenderer/Apple/Texture2D.h"
#include "SoraStringId.h"

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
		
		SoraTextureHandle tex;
		Texture2D* tex2D;
		stringId sid;
		int32 width;
	};
	
	class SoraiOSFont: public SoraFont {
		friend class SoraiOSFontManager;
		
	protected:
		SoraiOSFont(const SoraString& fontName, uint32 fontSize);
		
	public:
		~SoraiOSFont();
		
		void	render(float x, float y, const wchar_t* text, bool hcenter = false, bool vcenter = false);
		// render with alignments
		void	print(float x, float y, SoraFont::Alignment align, const wchar_t *format, ...);
		void	render(float x, float y, SoraFont::Alignment align, const wchar_t* text);
		
        void    renderIn3D(float x, float y, Alignment align, const wchar_t* text, const SoraMatrix4& transformMat);
        
		void	setColor(uint32 dwColor, int32 i = -1);
		uint32	getColor(int32 i);
		
		// set kernings
		void	setKerningWidth(float kerning);
		void	setKerningHeight(float kerning);
		
		// get kernings
		float	getKerningWidth() const;
		float	getKerningHeight() const;
		
		// get font dimensions
		float	getStringWidth(const wchar_t* text);
		float	getHeight() const;
		
		float getStringHeight(const wchar_t* text);
        SoraVector getStringDimensions(const wchar_t* text);
		
		// get font size
		uint32	getFontSize() const;
        void    setFontSize(uint32 newsize);
		
		// get width of a char
		float	getWidthFromCharacter(wchar_t c, bool original = false);
		
		// font effects
		void setCharRotation(float rot);
		void setScale(float scale);
		
        // get/set line width
        void setLineWidth(float width);
        float getLineWidth() const;
        
        float getCharRotation() const;
        float getScale() const;
        
        // set/get line rotation
        void setLineRotation(float rot, bool rotateChar=false);
        float getLineRotation() const;
                
	private:
		SoraSprite* pfSpr;
		SoraCore* CORE;
		
		float kerningWidth;
		float kerningHeight;
		
		uint32 size;
		SoraString fontName;
		sora_hash_map<stringId, iOSFontGlyph> ft_glyphs;
		
		float charRotation;
		float scale;
        float lineWidth;
        float lineRotation;
	};
	
} // namespace sora

#endif // OS_IOS

#endif // SORA_IOS_FONT_H