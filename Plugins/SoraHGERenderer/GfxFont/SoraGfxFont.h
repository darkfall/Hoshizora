#ifndef SORA_GFX_FONT_H_
#define SORA_GFX_FONT_H_

#include "SoraCore.h"
#include "SoraFont.h"
#include "GfxFont.h"
#include <vector>

namespace sora {

	class SoraGfxFont: public SoraFont {
	protected:
		friend class SoraGfxFontManager;

	public:
		~SoraGfxFont();

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

		
		// 设置字间距
		void	setKerningWidth(float32 kerning);
		void	setKerningHeight(float32 kerning);

		// 获取字间距
		float32	getKerningWidth();
		float32	getKerningHeight();

		float32	getStringWidth(const wchar_t* text);
		float32	getHeight();

		// 字体大小
		uint32	getFontSize();
	
		// 获取字符宽度
		float32	getWidthFromCharacter(wchar_t c, bool original = false);

	private:
		SoraGfxFont();

		void loadFont(const char* pstrFont, int size);

		GfxFont* pFont;
	};
} // namespace sora


#endif