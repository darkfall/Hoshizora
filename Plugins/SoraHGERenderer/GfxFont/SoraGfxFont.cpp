#include "SoraGfxFont.h"

namespace sora {
	
	SoraGfxFont::SoraGfxFont() {
		pFont = 0;
	}

	void SoraGfxFont::loadFont(const char* pstrFont, int size) {
		pFont = new GfxFont(pstrFont, size);
	}

	SoraGfxFont::~SoraGfxFont() {
		delete pFont;
	}

	float32 SoraGfxFont::getWidthFromCharacter(wchar_t c, bool bOriginal) {
		if(pFont)
			return pFont->GetWidthFromCharacter(c, bOriginal);
		return 0.f;
	}

	ulong32 SoraGfxFont::getColor(int32 i) {
		if(pFont)
			return pFont->GetColor(i);
		return 0;
	}

	void SoraGfxFont::setColor(ulong32 c, int32 i) {
		if(pFont)
			pFont->SetColor(c, i);
	}
	
	void SoraGfxFont::setKerningHeight(float32 kh) {
		if(pFont)
			pFont->SetKerningHeight(kh);
	}

	float32 SoraGfxFont::getKerningHeight() {
		if(pFont)
			return pFont->GetKerningHeight();
		return 0.f;
	}

	void SoraGfxFont::setKerningWidth(float32 kw) {
		if(pFont)
			pFont->SetKerningWidth(kw);
	}
	
	float32 SoraGfxFont::getKerningWidth() {
		if(pFont)
			return pFont->GetKerningWidth();
		return 0.f;
	}

	void SoraGfxFont::render(float32 x, float32 y, int32 align, const wchar_t* pwstr) {
		if(pFont)
			pFont->Render(x, y, pwstr);
	}

	void SoraGfxFont::print(float32 x, float32 y, int32 align, const wchar_t* pwstr, ...) {
		va_list l;
		va_start(l, pwstr);

		wchar_t text[1024];
		vswprintf(text, 1024, pwstr, l);

		if(pFont)
			pFont->Render(x, y, text);
	}

	void SoraGfxFont::render(float32 x, float32 y, const wchar_t* pwstr, bool bhcenter, bool bvcenter) {
		if(pFont)
			pFont->Render(x, y, pwstr);
	}

	float32 SoraGfxFont::getStringWidth(const wchar_t* pwstr) {
		if(pFont)
			return pFont->GetStringWidth(ws2s(pwstr).c_str());
		return 0.f;
	}

	float32 SoraGfxFont::getHeight() {
		if(pFont)
			return pFont->GetHeight();
		return 0.f;
	}

	uint32 SoraGfxFont::getFontSize() {
		if(pFont)
			return pFont->GetFontSize();
		return 0;
	}

} // namespace sora