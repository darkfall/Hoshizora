#include "SoraFreetypeFont.h"

namespace sora {
	
	FTFace::~FTFace() {
		if(loaded) {
			FT_Done_Face(face);
		}
	}

	bool FTFace::load(const char* name, uint32 size) {
		if(size == 0) {
			if(FT_New_Face(library, name, 0, &face)) {
				throw SORA_EXCEPTION("Error creating new face");
				return false;
			}
		} else {
			if(FT_New_Memory_Face(library, (FT_Bytes)name, size, 0, &face)) {
				throw SORA_EXCEPTION("Error creating new face");
				return false;
			}
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
					unsigned char *pt = bits.buffer;
					image = new unsigned char[bits.width * bits.rows];
					memcpy(image, pt, bits.width * bits.rows);

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

					unsigned int *texd = new unsigned int[imgw*imgh];
					memset(texd, 0, imgw*imgh*sizeof(unsigned int));
					unsigned int *texp = texd;
					offset = size - bits.rows;
					bool cflag = true;
					for(int i = 0; i < bits.rows; ++i) {
						unsigned int *rowp = texp;
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
					char name[128];
					sprintf(name,"FTFontGlyph%d",idx);
				
					if(tex){
						sora->releaseTexture(tex);
					}
					tex = createTexture(texd, imgw, imgh);
					delete texd;
					cached = true;
				}
			}
		}
	}

	HSORATEXTURE FTGlyph::createTexture(unsigned int *data, int w, int h) {
		HSORATEXTURE tex = sora->createTextureFromRawData(data, w, h);
		/*ulong32* color = sora->textureLock(tex);
		if(color)
			memcpy(color, data, sizeof( unsigned int ) * w * h);
		else
			throw SORA_EXCEPTION("error locking texture");
		sora->textureUnlock(tex);*/
		return tex;
	}

	SoraFTFont::SoraFTFont() {
		attached = false;
		kerningWidth = 0;
		kerningHeight = 0;

		sora = SoraCore::Instance();
		if(!sora)
			throw SORA_EXCEPTION("error getting core instance");
		sprite = sora->createSpriteTex(0);
		if(!sprite)
			throw SORA_EXCEPTION("error creating font sprite");
		
		charRotation = 0.f;
		scale = 1.f;
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
			throw SORA_EXCEPTION(e.what());
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

		if(idx && !ft_glyphs[idx-1].cached)	ft_glyphs[idx-1].cache(idx);
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

	float32 SoraFTFont::getKerningHeight() {
		return kerningHeight;
	}

	void SoraFTFont::setKerningWidth(float32 kw) {
		kerningWidth = kw;
	}
	
	float32 SoraFTFont::getKerningWidth() {
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
				y += getHeight() + kerningHeight;
				++pwstr;
				x = ox;
				if(align != FONT_ALIGNMENT_LEFT) {
					if(align == FONT_ALIGNMENT_RIGHT)
						x = x - getStringWidth(pwstr);
					else if(align == FONT_ALIGNMENT_CENTER )
						x = x - ((int32)getStringWidth(pwstr) >>1 );
				}
				continue;
			}
			n = getGlyphByChar(*pwstr);
			if(n > 0 && n < ft_glyphs.size()) {
				int imgw = ft_glyphs[n-1].imgw;
				int imgh = ft_glyphs[n-1].imgh;
				//int texw = ft_glyphs[n-1].texw;
				//int texh = ft_glyphs[n-1].texh;
				int offx = ft_glyphs[n-1].left;
				int offy = ft_glyphs[n-1].size - ft_glyphs[n-1].top;

				sprite->setTexture((SoraTexture*)ft_glyphs[n-1].tex);
				sprite->setTextureRect(0, 0, imgw-1.0f, imgh-1.0f);
				sprite->render(x+offx, y+offy);
 
				x += (getWidthFromCharacter(*pwstr) + kerningWidth);
			} 
			else {
				// skip unknown characters
				x += getWidthFromCharacter(*pwstr) + kerningWidth;
			}
			++pwstr;
		}
	}

	void SoraFTFont::print(float32 x, float32 y, int32 align, const wchar_t* pwstr, ...) {
		va_list l;
		va_start(l, pwstr);

		wchar_t text[1024];
		vswprintf(text, 1024, pwstr, l);

		render(x, y, align, text);
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
				y += getHeight() + kerningHeight;
				++pwstr;
				x = ox;
				if(bhcenter || bvcenter) {
					if(bhcenter)
						x = x - ((int)getStringWidth(pwstr) >> 1 );
					
					if(bvcenter)
						y = y - ((int)getHeight() >> 1 );
				}
				continue;
			}
			n = getGlyphByChar(*pwstr);
			if(n > 0 && n < ft_glyphs.size()) {
				int imgw = ft_glyphs[n-1].imgw;
				int imgh = ft_glyphs[n-1].imgh;
				//int texw = ft_glyphs[n-1].texw;
				//int texh = ft_glyphs[n-1].texh;
				int offx = ft_glyphs[n-1].left;
				int offy = ft_glyphs[n-1].size - ft_glyphs[n-1].top;

				sprite->setTexture((SoraTexture*)ft_glyphs[n-1].tex);
				sprite->setTextureRect(0, 0, imgw-1.0f, imgh-1.0f);
				sprite->setRotation(charRotation);
				sprite->setScale(scale, scale);
				sprite->render(x+offx, y+offy);
 
				x += (getWidthFromCharacter(*pwstr) + kerningWidth);
			} 
			else {
				// skip unknown characters
				x += getWidthFromCharacter(*pwstr) + kerningWidth;
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

	float32 SoraFTFont::getHeight() {
		if(!ft_glyphs.empty())
			return (float32)ft_glyphs[0].size+kerningHeight;
		return 0.f;
	}

	uint32 SoraFTFont::getFontSize() {
		return size;
	}
	
	void SoraFTFont::setCharRotation(float32 rot) {
		charRotation = rot;
	}
	
	void SoraFTFont::setScale(float32 s) {
		scale = s;
	}

} // namespace sora