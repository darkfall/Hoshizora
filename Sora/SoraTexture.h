#ifndef SORA_TEXTURE_H_
#define SORA_TEXTURE_H_

#include "SoraPlatform.h"

namespace sora {

	struct SoraTextureDataRef {
		ulong32* texData;
		
		SoraTextureDataRef(): texData(0) {}
	};
	
	class SoraTexture {
	public:
		SoraTexture();
		SoraTexture(ulong32 id, int32 width, int32 height, int32 oriwidth, int32 oriheight);

		ulong32 mTextureID;
		int32 mTextureWidth, mTextureHeight;
		int32 mOriginalWidth, mOriginalHeight;

		SoraTextureDataRef dataRef;
	};

} // namespace sora

#endif