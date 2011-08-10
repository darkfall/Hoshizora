#ifndef SORA_FREETYPE_FONT_H_
#define SORA_FREETYPE_FONT_H_

#include "SoraCore.h"
#include "SoraFont.h"

#include <ft2build.h>
#include <freetype/freetype.h>

#include <vector>

namespace sora {

	/**
		FTFace
	*/
	class FTFace {
	public:
		FTFace(): loaded(false) {}
		~FTFace();
		
		bool loaded;
		FT_Library	library;
		FT_Face		face;
		
		void* fontDataBuffer;

		bool load(const char* filename, uint32 size=0);
	};

	/**
		FTGlyph
	*/
	class FTGlyph {
	public:		
		FTGlyph();
		virtual ~FTGlyph();

		bool cached;
		SoraCore  *sora;

		void init() {}
		HSORATEXTURE createTexture( unsigned int *data, int w, int h );

		void cache(unsigned int idx);
		FT_Face *face;
		unsigned int size;
		unsigned int top;
		unsigned int left;
		unsigned int texw;
		unsigned int texh;
		unsigned int imgw;
		unsigned int imgh;
		HSORATEXTURE tex;
		int offset;
		unsigned char *image;
	};

	/** 
		Font Class using freetype to generate font glyphs and render to screen
	*/

	class SoraFTFont: public SoraFont {
	protected:
		friend class SoraFTFontManager;

	public:
		/**
		 * Render
		 *
		 * renders the text string to the screen
		 * @param x Screen-x coordinate
		 * @param y Screen-y coordinate
		 * @param align Alignment specificator. May be one of these: HGETEXT_LEFT, HGETEXT_RIGHT, HGETEXT_CENTER. 
		 * @param string the string to be rendered
		*/
		void	render(float32 x, float32 y, const wchar_t* text, bool hcenter = false, bool vcenter = false);
		/**
			render with alignments
		*/
		void	print(float32 x, float32 y, int32 align, const wchar_t *format, ...);
		void	render(float32 x, float32 y, int32 align, const wchar_t* text);

		void	setColor(ulong32 dwColor, int32 i = -1);
		ulong32	getColor(int32 i = 0);

		
		void	setKerningWidth(float32 kerning);
		void	setKerningHeight(float32 kerning);

		float32	getKerningWidth() const;
		float32	getKerningHeight() const;

		float32	getStringWidth(const wchar_t* text);
		float32 getStringHeight(const wchar_t* pwstr);
		float32	getHeight() const;

		uint32	getFontSize() const;
		void	setFontSize(uint32 size);
	
		float32	getWidthFromCharacter(wchar_t c, bool original = false);
		
		// font effects
		
		void setScale(float32 scale);
		void setCharRotation(float32 rot);
        
        void setLineWidth(float32 width);
        float32 getLineWidth() const;
        
        float32 getCharRotation() const;
        float32 getScale() const;
        
        void setLineRotation(float32 rot, bool rotateChar=false);
        float32 getLineRotation() const;
		
	private:
		SoraFTFont();
		~SoraFTFont();

		SoraCore* sora;
		SoraSprite* sprite;
				
		float32 kerningWidth;
		float32 kerningHeight;
        float32 lineWidth;

		bool attached;
		uint32 size;		
		// not recommended methods
		bool attach(FTFace *Face, uint32 size); 
		uint32 getGlyphByChar(wchar_t c);

		FTFace* ft_face;
		std::vector<FTGlyph> ft_glyphs;
		
		float32 scale;
		float32 charRotation;
        float32 lineRotation;
	};
} // namespace sora


#endif