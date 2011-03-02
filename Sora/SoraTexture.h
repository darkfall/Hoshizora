#ifndef SORA_TEXTURE_H_
#define SORA_TEXTURE_H_

#include "SoraPlatform.h"

namespace sora {
	#define MAX_TEXTURE_NAME_SIZE 512

	struct SoraTextureDataRef {
		uint32 x;
		uint32 y;
		uint32 w;
		uint32 h;
		
		ulong32* texData;
		
		SoraTextureDataRef(): texData(0), x(0), y(0), w(0), h(0) {}
	};
	
	class SoraTexture {
	public:
		SoraTexture();
		SoraTexture(ulong32 id, int32 width, int32 height, int32 oriwidth, int32 oriheight);

		ulong32 mTextureID;
		int32 mTextureWidth, mTextureHeight;
		int32 mOriginalWidth, mOriginalHeight;
		wchar_t name[MAX_TEXTURE_NAME_SIZE];
		//SoraTextureDataRef dataRef;
	};

} // namespace sora

#endif