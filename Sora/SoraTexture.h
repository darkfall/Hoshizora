#ifndef SORA_TEXTURE_H_
#define SORA_TEXTURE_H_

#include "SoraPlatform.h"

namespace sora {

	struct SORA_API SoraTextureDataRef {
		uint32* texData;
		
		SoraTextureDataRef(): texData(0) {}
	};
	
	class SORA_API SoraTexture {
	public:
		SoraTexture();
		SoraTexture(ulong32 id, int32 width, int32 height, int32 oriwidth, int32 oriheight);

		ulong32 mTextureID;
		int32 mTextureWidth, mTextureHeight;
		int32 mOriginalWidth, mOriginalHeight;

		SoraTextureDataRef dataRef;
        
#ifdef OS_IOS
        // if not retina texture and the device supports retina display, we need to scale the texture
        bool mIsRetinaTexture;
#endif
	};

} // namespace sora

#endif