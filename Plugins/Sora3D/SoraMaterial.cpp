//
//  SoraMaterial.cpp
//  Sora
//
//  Created by Robert Bu on 11/12/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraMaterial.h"

namespace sora {
    
    SoraMaterial::SoraMaterial() {
        for(int i=0; i<MaxTextureSize; ++i)
            mTextures[i] = 0;
    }
    
    SoraMaterial::SoraMaterial(MaterialType type): mMaterialType(type) { 
        for(int i=0; i<MaxTextureSize; ++i)
            mTextures[i] = 0;
    }
    
    void SoraMaterial::setTexture(int32 index, SoraTextureHandle tex) {
        mTextures[index] = tex;
    }
    
    SoraTextureHandle SoraMaterial::getTexture(int32 index) const {
        sora_assert(index >= 0 && index < MaxTextureSize);
        
        return mTextures[index];
    }
    
} // namespace sora
