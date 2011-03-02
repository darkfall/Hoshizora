#include "hgeCNFont.hpp"
#include "hgeFont.h"

#include "guichan/color.hpp"
#include "guichan/graphics.hpp"


namespace gcn {
	HGECNFont::HGECNFont(const std::string& fontName, int iFontSize) {
		mHGEFont = new GfxFont(fontName.c_str(), iFontSize);
	}

	HGECNFont::~HGECNFont() {
		delete mHGEFont;
	}

	GfxFont* HGECNFont::getFont() const {
		return mHGEFont;
	}

	int HGECNFont::getWidth(const std::string& text) const {
		return static_cast<int>(mHGEFont->GetStringWidth(text.c_str())) + 1;
	}

	int HGECNFont::getWidth(char character) const {
		char buf[2];
		sprintf(buf, "%c", character);

		return static_cast<int>(mHGEFont->GetStringWidth(buf));
	}

	int HGECNFont::getHeight() const {
		return static_cast<int>(mHGEFont->GetHeight());
	}

	int HGECNFont::getStringIndexAt(const std::string& text, int x) const {
		unsigned int i;
		int size = 0;
		for(i = 0; i < text.size(); ++i) {
			size = getWidth(text.substr(0, i));
			if(size > x) {
				return i;
			}
		}

		return text.size();
	}

	void HGECNFont::drawString(Graphics* graphics, const std::string& text, int x, int y) {
		ClipRectangle const& top = graphics->getCurrentClipArea();

		x += top.xOffset;
		y += top.yOffset;

		Color color = graphics->getColor();
		mHGEFont->SetColor(ARGB(color.a, color.r, color.g, color.b));
		mHGEFont->Render((float)x, (float)y, mHGEFont->StringAsWideStr(text).c_str());
	}
}