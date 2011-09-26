//
//  SoraCEGUITexture.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/24/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraCEGUITexture.h"
#include "SoraTexture.h"

namespace sora {
    
    using namespace CEGUI;
    
    const CEGUI::Size& SoraCEGUITexture::getSize() const {
        return mSize;
    }
    
    const CEGUI::Size& SoraCEGUITexture::getOriginalDataSize() const {
        return mOriginalSize;
    }
    
    const Vector2& SoraCEGUITexture::getTexelScaling() const {
        return Vector2(1.f / mOriginalSize.d_width, 1.f/ mOriginalSize.d_height);
    }
    
    void SoraCEGUITexture::loadFromFile(const String& filename, const String& resourceGroup) {
        mTexture = SoraTexture::LoadFromFile(filename.c_str());
    }
    
    void SoraCEGUITexture::loadFromMemory(const void* buffer,
                                          const CEGUI::Size& buffer_size,
                                          PixelFormat format) {
        mTexture = SoraTexture::LoadFromMemory((uint32*)buffer, buffer_size.d_width * buffer_size.d_height);
    }
    
    void SoraCEGUITexture::saveToMemory(void* buffer) {
        buffer = SoraTexture::GetData(mTexture);
    }
    
} // namespace sora
