#ifndef HGE_CN_FONT_HPP
#define HGE_CN_FONT_HPP

#include <string>

#include "GfxFont.h"

#include "guichan/font.hpp"
#include "guichan/platform.hpp"

namespace gcn {
	class Graphics;

	class GCN_EXTENSION_DECLSPEC HGECNFont: public Font {
	public:
		HGECNFont(const std::string& fontName, int iFontSize);
		~HGECNFont();

		virtual GfxFont* getFont() const;

		int getWidth(char character) const;

		int getWidth(const std::string& text) const;
		
		int getHeight() const;

		int getStringIndexAt(const std::string& text, int x) const;

		void drawString(Graphics* graphics, const std::string& text, int x, int y);

	protected:
		GfxFont* mHGEFont;

	};
};

#endif