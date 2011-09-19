#include "SoraTexture.h"
#include "SoraCore.h"

namespace sora {

	SoraTexture::SoraTexture(ulong32 texid, int32 width, int32 height, int32 oriwidth, int32 oriheight) {
		mTextureID = texid;
		mTextureWidth = width;
		mTextureHeight = height;
		mOriginalWidth = width;
		mOriginalHeight = height;
        mUserData = 0;
	}

	SoraTexture::SoraTexture() {
		mTextureID = 0;
		mTextureWidth = mTextureHeight = 0;
		mOriginalWidth = mOriginalHeight = 0;
        mUserData = 0;
	}
    
    SoraTextureHandle SoraTexture::LoadFromFile(const StringType& file) {
        return SoraCore::Instance()->createTexture(file);
    }
    
    SoraTextureHandle SoraTexture::LoadFromRawData(uint32* data, int32 w, int32 h) {
        return SoraCore::Instance()->createTextureFromRawData(data, w, h);
    }
    
    SoraTextureHandle SoraTexture::CreateEmpty(int32 w, int32 h) {
        return SoraCore::Instance()->createTextureWH(w, h);
    }
    
    void SoraTexture::Release(SoraTextureHandle handle) {
        SoraCore::Instance()->releaseTexture(handle);
    }
    
} // namespace sora