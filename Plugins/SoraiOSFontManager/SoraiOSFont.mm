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

namespace sora {

	iOSFontGlyph::iOSFontGlyph(): cached(false), tex(NULL), tex2D(NULL) {}
	iOSFontGlyph::~iOSFontGlyph() {
	}
	
	void iOSFontGlyph::release() {
		if(tex2D) {
			[tex2D release];
		}
		if(tex) {
            SoraCore::Ptr->releaseTexture(tex);
		}
	}
    
    NSString* wstring2NSString(const SoraWString& ws) {
        char* data = (char*)ws.data();
		unsigned size = ws.size() * sizeof(wchar_t);
		
#if TARGET_RT_BIG_ENDIAN
		const NSStringEncoding kEncoding_wchar_t = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32BE);
#else
		const NSStringEncoding kEncoding_wchar_t = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
#endif
		
		NSString* result = [[[NSString alloc] initWithBytes:data length:size encoding:kEncoding_wchar_t] autorelease];
		return result;
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
        //    [nsBuffer release];
			
			width = ptex->mTextureWidth;
			tex = (SoraTextureHandle)ptex;
			sid = str2id(str);
			cached = true;
			return sid;
		}
		return sid;
	}

	
	SoraiOSFont::SoraiOSFont(const SoraString& fn, uint32 fontSize):
    kerningWidth(0.f), 
    kerningHeight(0.f),
    lineWidth(0.f), 
    charRotation(0.f), 
    lineRotation(0.f), 
    scale(1.f) {
		CORE = SoraCore::Instance();
		pfSpr = new SoraSprite(0);
		
		size = fontSize;
		fontName = fn;
	}
	
	SoraiOSFont::~SoraiOSFont() {
		if(pfSpr)
			delete pfSpr;
		for(sora_hash_map<stringId, iOSFontGlyph>::iterator itGlyph = ft_glyphs.begin(); itGlyph != ft_glyphs.end(); ++itGlyph)
			itGlyph->second.release();
	}
	
	void SoraiOSFont::render(float x, float y, const wchar_t* str, bool bxCenter, bool byCenter) {
		float ox = x;
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
    
    void SoraiOSFont::print(float x, float y, SoraFont::Alignment align, const wchar_t* pwstr, ...) {
        va_list l;
        va_start(l, pwstr);
        
        wchar_t text[1024];
        vswprintf(text, 1024, pwstr, l);
        
        render(x, y, align, text);
    }
    
    void SoraiOSFont::renderIn3D(float x, float y, Alignment align, const wchar_t* text, const SoraMatrix4& transformMat) {
        
    }
    
    void SoraiOSFont::render(float x, float y, SoraFont::Alignment align, const wchar_t* pwstr) {
        float ox = x;
        if(align != SoraFont::AlignmentLeft) {
            if(align == SoraFont::AlignmentRight)
                x = x - getStringWidth(pwstr);
            else if(align == SoraFont::AlignmentCenter)
                x = x - ((int32)getStringWidth(pwstr) >> 1);
        }
        
        stringId sid;
        while(pwstr && *pwstr) {
            if(*pwstr == L'\n') {
                y += getHeight() + kerningHeight;
                ++pwstr;
                x = ox;
                if(align != SoraFont::AlignmentLeft) {
                    if(align == SoraFont::AlignmentRight)
                        x = x - getStringWidth(pwstr);
                    else if(align == SoraFont::AlignmentCenter)
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
    
    float SoraiOSFont::getStringWidth(const wchar_t* pwstr) {
        float width = 0.f;
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
			width += cglyph.width + kerningWidth;
        }
		return width;
    }
	
	float SoraiOSFont::getStringHeight(const wchar_t* pwstr) {
		float height = getHeight();
		for(const wchar_t* p = pwstr; *p; ++p) {
            if(*p == L'\n')
				height += getHeight() + kerningHeight;
		}
		return height;
	}
    
    SoraVector SoraiOSFont::getStringDimensions(const wchar_t* text) {
        float height = getHeight();
        float width = 0.f;
		float w = 0.f;
		for(const wchar_t* p = text; *p; ++p) {
			if(*p == L'\n') {
				height += getHeight() + kerningHeight;
                if(width < w)
                    width = w;
                w = 0.f;
            }
			else {
                w += getWidthFromCharacter(*text) + kerningWidth;
                
				if(lineWidth != 0.f) {
					if(w >= lineWidth) {
						height += getHeight() + kerningHeight;
						w = 0.f;
					}
				}
			}
		}
        if(width < w)
            width = w;
		return SoraVector(width+10.f, height+10.f);
    }
    
    float SoraiOSFont::getWidthFromCharacter(wchar_t character, bool original) {
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
    
    float SoraiOSFont::getHeight() const {
        return size+kerningHeight;
    }
    
    uint32 SoraiOSFont::getFontSize() const {
        return size;
    }
    
    void SoraiOSFont::setScale(float s) {
        scale = s;
    }
    
    void SoraiOSFont::setCharRotation(float rot) {
        charRotation = rot;
    }
    
    uint32 SoraiOSFont::getColor(int32 i) {
        return pfSpr->getColor(i);
    }
    
    void SoraiOSFont::setColor(uint32 c, int32 i) {
        pfSpr->setColor(c, i);
    }

    void SoraiOSFont::setKerningWidth(float kw) {
        kerningWidth = kw;
    }
    
    void SoraiOSFont::setKerningHeight(float kh) {
        kerningHeight = kh;
    }
    
    float SoraiOSFont::getKerningWidth() const {
        return kerningWidth;
    }
    
    float SoraiOSFont::getKerningHeight() const {
        return kerningHeight;
    }
    
    void SoraiOSFont::setFontSize(uint32 newsize) {
    
    }
    
    void SoraiOSFont::setLineWidth(float width) {
        lineWidth = width;
    }
    
    float SoraiOSFont::getLineWidth() const {
        return lineWidth;
    }
    
    float SoraiOSFont::getCharRotation() const {
        return charRotation;
    }
    
    float SoraiOSFont::getScale() const {
        return scale;
    }
    
    void SoraiOSFont::setLineRotation(float rot, bool rotateChar) {
        lineRotation = rot;
        if(rotateChar)
            setCharRotation(rot);
        else
            setCharRotation(0.f);
    }
    
    float SoraiOSFont::getLineRotation() const {
        return lineRotation;
    }

	
} // namespace sora