/*
 *  SoraiOSFont.cpp
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#include "SoraiOSFont.h"
#include "SoraCore.h"
#include "SoraiOSGLRenderer/SoraiOSDeviceHelper.h"

namespace sora {

	iOSFontGlyph::iOSFontGlyph(): cached(false), tex(NULL), tex2D(NULL) {}
	iOSFontGlyph::~iOSFontGlyph() {
	}
	
	void iOSFontGlyph::release() {
		if(tex2D) {
			[tex2D release];
		}
		if(tex) {
			SORA->releaseTexture(tex);
		}
	}
	
	stringId iOSFontGlyph::cache(const SoraWString& str, const SoraString& fontName, uint32 fontSize) {
		if(!cached) {
			NSString* nsBuffer = wstring2NSString(str);
			NSString* nsFontName = [[NSString alloc] initWithUTF8String:fontName.c_str()];
			tex2D = [[Texture2D alloc] initWithString: nsBuffer fontName: nsFontName fontSize: fontSize];
			if(!tex2D) {
				width = 0;
				[nsFontName release];
				return 0;
			}
			SoraTexture* ptex = new SoraTexture([tex2D name], tex2D.contentSize.width, tex2D.contentSize.height,
								  tex2D.pixelsWide, tex2D.pixelsHigh);
			
			[nsFontName release];
			
			width = ptex->mTextureWidth;
			tex = (HSORATEXTURE)ptex;
			sid = str2id(str);
			cached = true;
			return sid;
		}
		return sid;
	}

	
	SoraiOSFont::SoraiOSFont(const SoraString& fn, uint32 fontSize) {
		kerningWidth = 0.f;
		kerningHeight = 0.f;
		
		CORE = SoraCore::Instance();
		pfSpr = new SoraSprite(0);
		
		size = fontSize;
		fontName = fn;
		
		charRotation = 0.f;
		scale = 1.f;
	}
	
	SoraiOSFont::~SoraiOSFont() {
		if(pfSpr)
			delete pfSpr;
		for(hash_map<ulong32, iOSFontGlyph>::iterator itGlyph = ft_glyphs.begin(); itGlyph != ft_glyphs.end(); ++itGlyph)
			itGlyph->second.release();
	}
	
	void SoraiOSFont::render(float32 x, float32 y, const wchar_t* str, bool bxCenter, bool byCenter) {
		float32 ox = x;
		if(bxCenter || byCenter) {
			if(bxCenter)
				x = x - ((int)getStringWidth(str) >> 1);
			if(byCenter)
				y = y - ((int)getHeight() >> 1);
		}
		
		stringId sid;
		while(str && *str) {
			if(*str == L'\n') {
				y += getHeight() + kerningHeight;
				++str;
				x = ox;
				if(bxCenter || byCenter) {
					if(bxCenter)
						x = x - ((int)getStringWidth(str) >> 1);
					if(byCenter)
						y = y - ((int)getHeight() >> 1);
				}
				continue;
			} else if(*str == L'\t') {
				x += getWidthFromCharacter(L' ')*8;
                ++str;
				continue;
			}
			sid = crc32(&*str, 1);
			iOSFontGlyph cglyph = ft_glyphs[sid];
			if(!cglyph.cached) {
				std::wstring sstr; sstr += *str;
				if((sid = cglyph.cache(sstr.c_str(), fontName, size)) != 0) {
					// cache the string
					ft_glyphs[sid] = cglyph;
				}
			}
			if(cglyph.cached) {
				if(!cglyph.tex)
					continue;
				pfSpr->setTexture(cglyph.tex);
				pfSpr->setRotation(charRotation);
				pfSpr->setScale(scale, scale);
				pfSpr->render(x, y);
				
				x += (getWidthFromCharacter(*str) + kerningWidth);
			} else {
				// cannot cache str, maybe unknown
				x += getWidthFromCharacter(*str) + kerningWidth;
			}
			
			++str;
		}
	}
    
    void SoraiOSFont::print(float32 x, float32 y, int32 align, const wchar_t* pwstr, ...) {
        va_list l;
        va_start(l, pwstr);
        
        wchar_t text[1024];
        vswprintf(text, 1024, pwstr, l);
        
        render(x, y, align, text);
    }
    
    void SoraiOSFont::render(float32 x, float32 y, int32 align, const wchar_t* pwstr) {
        float32 ox = x;
        if(align != FONT_ALIGNMENT_LEFT) {
            if(align == FONT_ALIGNMENT_RIGHT)
                x = x - getStringWidth(pwstr);
            else if(align == FONT_ALIGNMENT_CENTER)
                x = x - ((int32)getStringWidth(pwstr) >> 1);
        }
        
        stringId sid;
        while(pwstr && *pwstr) {
            if(*pwstr == L'\n') {
                y += getHeight() + kerningHeight;
                ++pwstr;
                x = ox;
                if(align != FONT_ALIGNMENT_LEFT) {
                    if(align == FONT_ALIGNMENT_RIGHT)
                        x = x - getStringWidth(pwstr);
                    else if(align == FONT_ALIGNMENT_CENTER)
                        x = x - ((int32)getStringWidth(pwstr) >> 1);
                }
                continue;
                
            } else if(*pwstr == L'\t') {
                x += getWidthFromCharacter(L' ')*8;
                ++pwstr;
                continue;
            }
            
            sid = crc32(&*pwstr, 1);
            iOSFontGlyph cglyph = ft_glyphs[sid];
			if(!cglyph.cached) {
				std::wstring sstr; sstr += *pwstr;
				if((sid = cglyph.cache(sstr, fontName, size)) != 0) {
					// cache the string
					ft_glyphs[sid] = cglyph;
				}
			}
			if(cglyph.cached) {
				if(!cglyph.tex)
					continue;
				pfSpr->setTexture(cglyph.tex);
				pfSpr->setRotation(charRotation);
				pfSpr->setScale(scale, scale);
				pfSpr->render(x, y);
				
				x += (getWidthFromCharacter(*pwstr) + kerningWidth);
			} else {
				// cannot cache str, maybe unknown
				x += getWidthFromCharacter(*pwstr) + kerningWidth;
			}
			
			++pwstr;
        } 
    }
    
    float32 SoraiOSFont::getStringWidth(const wchar_t* pwstr) {
        float32 width = 0.f;
        for(const wchar_t* p = pwstr; *p; ++p) {
            if(*p == L'\n')
                break;
            stringId sid = crc32(&*p, 1);
            iOSFontGlyph cglyph = ft_glyphs[sid];
			if(!cglyph.cached) {
				std::wstring sstr; sstr += *p;
				if((sid = cglyph.cache(sstr, fontName, size)) != 0) {
					// cache the string
					ft_glyphs[sid] = cglyph;
				}
			}
			width += cglyph.width;
        }
		return width;
    }
	
	float32 SoraiOSFont::getStringHeight(const wchar_t* pwstr) {
		float32 height = getHeight();
		for(const wchar_t* p = pwstr; *p; ++p) {
            if(*p == L'\n')
				height += getHeight();
		}
		return height;
	}
    
    float32 SoraiOSFont::getWidthFromCharacter(wchar_t character, bool original) {
        stringId sid = crc32(&character, 1);
        iOSFontGlyph cglyph = ft_glyphs[sid];
        if(!cglyph.cached) {
			std::wstring sstr; sstr += character;
            cglyph.cache(sstr, fontName, size);
			ft_glyphs[sid] = cglyph;
        }
		SoraTexture* ptex = ((SoraTexture*)cglyph.tex);
		if(ptex)
			return original?ptex->mTextureWidth:ptex->mOriginalWidth;
		else {
			return 0.f;
		}

    }
    
    float32 SoraiOSFont::getHeight() {
        return size+kerningHeight;
    }
    
    uint32 SoraiOSFont::getFontSize() {
        return size;
    }
    
    
    void SoraiOSFont::setScale(float32 s) {
        scale = s;
    }
    
    void SoraiOSFont::setCharRotation(float32 rot) {
        charRotation = rot;
    }
    
    ulong32 SoraiOSFont::getColor(int32 i) {
        return pfSpr->getColor(i);
    }
    
    void SoraiOSFont::setColor(ulong32 c, int32 i) {
        pfSpr->setColor(c, i);
    }

    void SoraiOSFont::setKerningWidth(float32 kw) {
        kerningWidth = kw;
    }
    
    void SoraiOSFont::setKerningHeight(float32 kh) {
        kerningHeight = kh;
    }
    
    float32 SoraiOSFont::getKerningWidth() {
        return kerningWidth;
    }
    
    float32 SoraiOSFont::getKerningHeight() {
        return kerningHeight;
    }
	
} // namespace sora