#ifndef SORA_TEXTURE_H_
#define SORA_TEXTURE_H_

#include "SoraPlatform.h"
#include "SoraString.h"

namespace sora {

	struct SORA_API SoraTextureDataRef {
		uint32* texData;
		
		SoraTextureDataRef(): texData(0) {}
	};
	
	struct SORA_API SoraTexture {
		SoraTexture();
		SoraTexture(ulong32 id, int32 width, int32 height, int32 oriwidth, int32 oriheight);

		ulong32 mTextureID;
		int32 mTextureWidth, mTextureHeight;
		int32 mOriginalWidth, mOriginalHeight;
        
        /**
         * For wrapper convinience
         **/
        void* mUserData;
		SoraTextureDataRef dataRef;
        
        static SoraTextureHandle LoadFromFile(const StringType& file);
        static SoraTextureHandle LoadFromRawData(uint32* data, int32 w, int32 h);
        static SoraTextureHandle CreateEmpty(int32 w, int32 h);
        static void Release(SoraTextureHandle h);
	};

} // namespace sora

#endif