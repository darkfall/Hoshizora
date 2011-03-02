#include "SoraGfxFontManager.h"

#include "SoraGfxFont.h"

namespace sora {

	SoraGfxFontManager::~SoraGfxFontManager() {
	
	}


	SoraFont* SoraGfxFontManager::getFont(const char* filename, uint32 size, uint32 bsize, const char* pr) {
		SoraGfxFont* font = new SoraGfxFont;
		font->loadFont(filename, size);
		return font;
	}

} // namespace sora