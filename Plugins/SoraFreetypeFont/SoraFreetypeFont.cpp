#include "SoraFreetypeFont.h"

namespace sora {
	
	FTFace::~FTFace() {
		if(loaded) {
			FT_Done_Face(face);
			if(fontDataBuffer)
				sora::SORA->freeResourceFile(fontDataBuffer);
		}
	}

	bool FTFace::load(const char* name, uint32 size) {
		if(size == 0) {
			if(FT_New_Face(library, name, 0, &face)) {
				THROW_SORA_EXCEPTION("Error creating new face");
				return false;
			}
			fontDataBuffer = NULL;
		} else {
			if(FT_New_Memory_Face(library, (FT_Bytes)name, size, 0, &face)) {
				THROW_SORA_EXCEPTION("Error creating new face");
				return false;
			}
			fontDataBuffer = (void*)name;
		}

		loaded = true;
		return true;
	}

	FTGlyph::FTGlyph() {
		tex = 0;
		image = 0;

		sora = SoraCore::Instance();
	}

	FTGlyph::~FTGlyph() {
		if(image) delete image;
		if(tex) {
			sora->releaseTexture(tex);
		}
	}

	void FTGlyph::cache(unsigned int idx) {
		FT_Set_Pixel_Sizes(*face, 0, size);
		if (!FT_Load_Glyph(*face, idx, FT_LOAD_NO_HINTING | FT_LOAD_NO_BITMAP)) {
			FT_GlyphSlot glyph = (*face)->glyph;
			FT_Bitmap bits;
			if(glyph->format == ft_glyph_format_outline) {
				if(!FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL)) {
					bits = glyph->bitmap;
					uint8_t *pt = bits.buffer;
				//	image = new unsigned char[bits.width * bits.rows];
				//	memcpy(image, pt, bits.width * bits.rows);

					top = glyph->bitmap_top;
					left = glyph->bitmap_left;
					imgw = 1;
					imgh = 1;
					texw = bits.width;
					texh = bits.rows;
					
					for(;;) {
						if(imgw > texw) {
							break;
						} else {
							imgw <<= 1;
						}
					}
					for(;;) {
						if(imgh > texh) {
							break;
						} else {
							imgh <<= 1;
						}
					}
					if(imgw > imgh) {
						imgh = imgw;
					} else {
						imgw = imgh;
					}

					uint32 *texd = (uint32*)malloc(imgw*imgh*4);
 					memset(texd, 0, imgw*imgh*4);
					uint32 *texp = texd;
					offset = size - bits.rows;
					bool cflag = false;
					for(int i = 0; i < bits.rows; ++i) {
						uint32 *rowp = texp;
						for(int j = 0; j < bits.width; ++j){
							if(*pt) {
								if(cflag) {
									*rowp = *pt;
									*rowp *= 0x01010101;
								} else {
									*rowp = *pt << 24;
									*rowp |= 0xffffff;
								}
							} else {
								*rowp = 0;
							}
							pt++;
							rowp++;
						}
						texp += imgw;
					}
                    
					if(tex != 0)
						SORA->releaseTexture(tex);
					tex = createTexture(texd, imgw, imgh);
                    free(texd);
					cached = true;
				}
			}
		}
	}

	HSORATEXTURE FTGlyph::createTexture(unsigned int *data, int w, int h) {
		HSORATEXTURE tex = sora->createTextureFromRawData(data, w, h);
		return tex;
	}

	SoraFTFont::SoraFTFont() {
		attached = false;
		kerningWidth = 0.f;
		kerningHeight = 0.f;
        
		charRotation = 0.f;
		scale = 1.f;
        lineWidth = 0.f;
        lineRotation = 0.f;
        
		sora = SoraCore::Instance();
		sprite = new SoraSprite(NULL);
		if(!sprite)
			THROW_SORA_EXCEPTION("error creating font sprite");
		
	}

	SoraFTFont::~SoraFTFont() {
		attached = false;
		
		delete sprite;
		sprite = 0;
	}

	bool SoraFTFont::attach(FTFace* face, uint32 size) {
		ft_face = face;
		this->size = size;

//		FTGlyph tmp;  
//		int* tmp2;

		try {
			ft_glyphs.resize(ft_face->face->num_glyphs);
		} catch(std::exception& e) {
			THROW_SORA_EXCEPTION(e.what());
		}
		for(int i = 0; i < ft_face->face->num_glyphs; i++) {
			ft_glyphs[i].sora = sora;
			ft_glyphs[i].size = size;
			ft_glyphs[i].face = &(ft_face->face);
			ft_glyphs[i].cached = false;

		//	tmp2 = (int*)&(ft_glyphs[i]);
		//	*tmp2 = *(int*)(&tmp);
			ft_glyphs[i].init(); 
		}
		attached = true;
		return true;			
	}

	uint32 SoraFTFont::getGlyphByChar(wchar_t c) {
		unsigned int idx = FT_Get_Char_Index(ft_face->face, c);

		if((idx && !ft_glyphs[idx-1].cached) || ft_glyphs[idx-1].size != size) {
			ft_glyphs[idx-1].size = size;
			ft_glyphs[idx-1].cache(idx);
		}
		return idx;
	}

	float32 SoraFTFont::getWidthFromCharacter(wchar_t c, bool bOriginal) {
		unsigned int n = getGlyphByChar(c);
		if(n > 0 && n < ft_glyphs.size()) {
			int w = ft_glyphs[n-1].texw;
			int left = ft_glyphs[n-1].left;
			if (w + left > 0) return (float32)(w+left);
		}
		if(c >= 0x2000) {
			return (float32)ft_glyphs[0].size;
		} else {
			return (float32)ft_glyphs[0].size / 2;
		}
	}

	ulong32 SoraFTFont::getColor(int32 i) {
		return sprite->getColor(i);
	}

	void SoraFTFont::setColor(ulong32 c, int32 i) {
		sprite->setColor(c, i);
	}
	
	void SoraFTFont::setKerningHeight(float32 kh) {
		kerningHeight = kh;
	}

	float32 SoraFTFont::getKerningHeight() const {
		return kerningHeight;
	}

	void SoraFTFont::setKerningWidth(float32 kw) {
		kerningWidth = kw;
	}
	
	float32 SoraFTFont::getKerningWidth() const {
		return kerningWidth;
	}

	void SoraFTFont::render(float32 x, float32 y, int32 align, const wchar_t* pwstr) {
		float ox = x;
		if(align != FONT_ALIGNMENT_LEFT) {
			if(align == FONT_ALIGNMENT_RIGHT)
				x = x - getStringWidth(pwstr);
			else if(align == FONT_ALIGNMENT_CENTER )
				x = x - ((int32)getStringWidth(pwstr) >>1 );
		}
        
		uint32 n;
		while(*pwstr) {
			if(*pwstr == L'\n') {
				++pwstr;
                y += getHeight() + kerningHeight;
				x = ox;
				if(align != FONT_ALIGNMENT_LEFT) {
					if(align == FONT_ALIGNMENT_RIGHT)
						x = x - getStringWidth(pwstr);
					else if(align == FONT_ALIGNMENT_CENTER )
						x = x - ((int32)getStringWidth(pwstr) >>1 );
				}
				continue;
			} else if(*pwstr == L'\t') {
				x += getWidthFromCharacter(L' ')*8;
                ++pwstr;
				continue;
			}
            
			n = getGlyphByChar(*pwstr);
			if(n > 0 && n < ft_glyphs.size()) {
				int imgw = ft_glyphs[n-1].imgw - 1;
				int imgh = ft_glyphs[n-1].imgh - 1;
				//int texw = ft_glyphs[n-1].texw;
				//int texh = ft_glyphs[n-1].texh;
				int offx = ft_glyphs[n-1].left;
				int offy = ft_glyphs[n-1].size - ft_glyphs[n-1].top;

				sprite->setTexture(ft_glyphs[n-1].tex);
				sprite->setTextureRect(0, 0, imgw, imgh);
            //    sprite->setCenter(imgw/2.f, imgh/2.f);
                sprite->setRotation(charRotation);
				sprite->setScale(scale, scale);
				sprite->render(x+offx, y+offy);
 
                if(lineRotation == 0.f) {
                    x += (getWidthFromCharacter(*pwstr) + kerningWidth);
                    if(lineWidth != 0.f && x >= lineWidth+ox) {
                        y += getHeight() + kerningHeight;
                        x = ox;
                        if(align != FONT_ALIGNMENT_LEFT) {
                            if(align == FONT_ALIGNMENT_RIGHT)
                                x = x - getStringWidth(pwstr);
                            else if(align == FONT_ALIGNMENT_CENTER )
                                x = x - ((int32)getStringWidth(pwstr) >>1 );
                        }
                    }
                } else {
                    x += (getWidthFromCharacter(*pwstr) + kerningWidth) * cosf(lineRotation);
                    y += (getHeight() + kerningHeight) * sinf(lineRotation);
                }
			} 
			else {
				// skip unknown characters
				if(lineRotation == 0.f) {
                    x += (getWidthFromCharacter(*pwstr) + kerningWidth);
                    if(lineWidth != 0.f && x >= lineWidth+ox) {
                        y += getHeight() + kerningHeight;
                        x = ox;
                        if(align != FONT_ALIGNMENT_LEFT) {
                            if(align == FONT_ALIGNMENT_RIGHT)
                                x = x - getStringWidth(pwstr);
                            else if(align == FONT_ALIGNMENT_CENTER )
                                x = x - ((int32)getStringWidth(pwstr) >>1 );
                        }
                    }
                } else {
                    float32 dist = (getWidthFromCharacter(*pwstr) + kerningWidth);
                    x += dist * cosf(lineRotation);
                    y += dist * sinf(lineRotation);
                }       
			}
			++pwstr;
		}
	}

	void SoraFTFont::print(float32 x, float32 y, int32 align, const wchar_t* format, ...) {
		va_list	ArgPtr;

#ifdef HAS_WSTRING
		wchar_t Message[1024] = {0};
		va_start(ArgPtr, format);
		vswprintf(Message, 1024, format, ArgPtr);
		va_end(ArgPtr);
		render(x, y, align, Message);
		
#else
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, ws2sfast(format).c_str(), ArgPtr);
		va_end(ArgPtr);
		render(x, y, align,  s2wsfast(Message).c_str());
#endif
	}

	void SoraFTFont::render(float32 x, float32 y, const wchar_t* pwstr, bool bhcenter, bool bvcenter) {
		float ox = x;
		if(bhcenter || bvcenter) {
			if(bhcenter)
				x = x - ((int)getStringWidth(pwstr) >> 1 );

			if(bvcenter)
				y = y - ((int)getHeight() >> 1 );
		}

		uint32 n;
		while(pwstr && *pwstr) {
			if(*pwstr == L'\n') {
				++pwstr;
                y += getHeight() + kerningHeight;
				x = ox;
				if(bhcenter || bvcenter) {
					if(bhcenter)
						x = x - ((int)getStringWidth(pwstr) >> 1 );
					
					if(bvcenter)
						y = y - ((int)getHeight() >> 1 );
				}
				continue;
			} else if(*pwstr == L'\t') {
				x += getWidthFromCharacter(L' ')*8;
                ++pwstr;
				continue;
			}
            
			n = getGlyphByChar(*pwstr);
			if(n > 0 && n < ft_glyphs.size()) {
				int imgw = ft_glyphs[n-1].imgw - 1;
				int imgh = ft_glyphs[n-1].imgh - 1;
				//int texw = ft_glyphs[n-1].texw;
				//int texh = ft_glyphs[n-1].texh;
				int offx = ft_glyphs[n-1].left;
				int offy = ft_glyphs[n-1].size - ft_glyphs[n-1].top;

				sprite->setTexture(ft_glyphs[n-1].tex);
				sprite->setTextureRect(0, 0, imgw, imgh);
            //    sprite->setCenter(imgw/2.f, imgh/2.f);
				sprite->setRotation(charRotation);
				sprite->setScale(scale, scale);
				sprite->render(x+offx, y+offy);
 
                if(lineRotation == 0.f) {
                    x += (getWidthFromCharacter(*pwstr) + kerningWidth);
                    if(lineWidth != 0.f && x >= lineWidth+ox) {
                        y += getHeight() + kerningHeight;
                        x = ox;
                        if(bhcenter || bvcenter) {
                            if(bhcenter)
                                x = x - ((int)getStringWidth(pwstr) >> 1 );
                            
                            if(bvcenter)
                                y = y - ((int)getHeight() >> 1 );
                        }
                    }
                } else {
                    x += (getWidthFromCharacter(*pwstr) + kerningWidth) * cosf(lineRotation);
                    y += (getHeight() + kerningHeight) * sinf(lineRotation);
                }
			} 
			else {
				// skip unknown characters
				if(lineRotation == 0.f) {
                    x += (getWidthFromCharacter(*pwstr) + kerningWidth);
                    if(lineWidth != 0.f && x >= lineWidth+ox) {
                        y += getHeight() + kerningHeight;
                        x = ox;
                        if(bhcenter || bvcenter) {
                            if(bhcenter)
                                x = x - ((int)getStringWidth(pwstr) >> 1 );
                            
                            if(bvcenter)
                                y = y - ((int)getHeight() >> 1 );
                        }
                    }
                } else {
                    float32 dist = (getWidthFromCharacter(*pwstr) + kerningWidth);
                    x += dist * cosf(lineRotation);
                    y += dist * sinf(lineRotation);
                }
			}
			++pwstr;
		}
	}

	float32 SoraFTFont::getStringWidth(const wchar_t* pwstr) {
		float width = 0.f;
		for(const wchar_t* p = pwstr; *p; ++p) {
			if(*p == L'\n')
				break;
			width += getWidthFromCharacter(*p) + kerningWidth;
		}
		return width;
	}
	
	float32 SoraFTFont::getStringHeight(const wchar_t* pwstr) {
		float height = getHeight();
		for(const wchar_t* p = pwstr; *p; ++p) {
			if(*p == L'\n')
				height += getHeight();
		}
		return height;
	}

	float32 SoraFTFont::getHeight() const {
		if(!ft_glyphs.empty())
			return (float32)size+kerningHeight;
		return 0.f;
	}

	uint32 SoraFTFont::getFontSize() const {
		return size;
	}
	
	void SoraFTFont::setFontSize(uint32 s) {
		size = s;
	}
	
	void SoraFTFont::setCharRotation(float32 rot) {
		charRotation = DGR_RAD(rot);
	}
	
	void SoraFTFont::setScale(float32 s) {
		scale = s;
	}
    
    void SoraFTFont::setLineWidth(float32 width) {
        lineWidth = width;
    }
    
    float32 SoraFTFont::getLineWidth() const {
        return lineWidth;
    }
    
    float32 SoraFTFont::getCharRotation() const {
        return charRotation;
    }
    
    float32 SoraFTFont::getScale() const {
        return scale;
    }

    void SoraFTFont::setLineRotation(float32 rot, bool rotateChar) {
        lineRotation = DGR_RAD(rot);
        if(rotateChar)
            setCharRotation(rot);
        else
            setCharRotation(0.f);
    }
    
    float32 SoraFTFont::getLineRotation() const {
        return lineRotation;
    }
} // namespace sora