#include "SoraTexture.h"

namespace sora {

	SoraTexture::SoraTexture(ulong32 texid, int32 width, int32 height, int32 oriwidth, int32 oriheight) {
		mTextureID = texid;
		mTextureWidth = width;
		mTextureHeight = height;
		mOriginalWidth = width;
		mOriginalHeight = height;
	}

	SoraTexture::SoraTexture() {
		mTextureID = 0;
		mTextureWidth = mTextureHeight = 0;
		mOriginalWidth = mOriginalHeight = 0;
	}

} // namespace sora