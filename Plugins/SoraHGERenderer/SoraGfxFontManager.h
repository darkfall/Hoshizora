#ifndef SORA_GFX_FONT_MANAGER_H_
#define SORA_GFX_FONT_MANAGER_H_

#include <vector>
#include <string>
#include "SoraPlatform.h"
#include "GfxFont.h"
#include "SoraFontManager.h"

#include <map>

namespace sora {
	

	class SoraGfxFontManager: public SoraFontManager {
	public:
		SoraGfxFontManager() {}
		~SoraGfxFontManager();

		SoraFont* getFont(const char* filename, uint32 fontsize, uint32 bsize=0, const char* pr=0);

	private:
		
		bool inited;
	};
};

#endif