#include "soraguifont.hpp"

#include "guichan/color.hpp"
#include "guichan/graphics.hpp"

#include "SoraStringConv.h"
#include "SoraFont.h"
#include "SoraColor.h"

namespace gcn
{
    SoraGUIFont::SoraGUIFont(const char* lpsFontName, int nFaceSize)
    {
		sora = sora::SoraCore::Instance();
        mSoraFont = (sora::SoraFont*)sora->createFont(sora::s2ws(lpsFontName), nFaceSize);
    }

    SoraGUIFont::~SoraGUIFont()
    {
        delete mSoraFont;
    }

    sora::SoraFont *SoraGUIFont::getFont() const
    {
        return mSoraFont;
    }

    int SoraGUIFont::getWidth(const std::string &text) const
    {
		if(text.size() > 0)
		  return static_cast<int>(mSoraFont->getStringWidth(sora::s2ws(text).c_str())) + 1;
		return 1;
    }
  
    int SoraGUIFont::getWidth(char character) const
    {
        char buf[2];

        sprintf(buf, "%c", character);

		if(mSoraFont)
			return (int)mSoraFont->getStringWidth(sora::s2ws(buf).c_str());
		return 0;
    }

    int SoraGUIFont::getHeight() const
    {
        return  static_cast<int>(mSoraFont->getHeight());
    }

    int SoraGUIFont::getStringIndexAt(const std::string& text, int x) const
    {
        unsigned int i;
        int size = 0;

        for (i = 0; i < text.size(); ++i)
        {
            size += getWidth(text.at(i));

            if (size > x)
            {
                return i;
            }
        }

        return text.size();
    }

    void SoraGUIFont::drawString(Graphics *graphics, const std::string &text, int x, int y)
    {
        ClipRectangle const &top = graphics->getCurrentClipArea();

        x += top.xOffset;
        y += top.yOffset;

        Color color = graphics->getColor();
		
		if(mSoraFont) {
			mSoraFont->setColor(CARGB(color.a, (float)color.r, (float)color.g, (float)color.b));
			//mSoraFont->Render(x, y, HGETEXT_LEFT, text.c_str());
			mSoraFont->render((float)x, (float)y, sora::s2ws(text+" \0").c_str());
		}
    }
}
