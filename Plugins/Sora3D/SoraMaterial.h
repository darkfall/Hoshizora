//
//  SoraMaterial.h
//  Sora
//
//  Created by Ruiwei Bu on 11/8/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraMaterial_h
#define Sora_SoraMaterial_h

#include "SoraPlatform.h"
#include "SoraPreDeclare.h"
#include "SoraColor.h"
#include "SoraShaderable.h"

namespace sora {
    
    class SoraMaterial: public SoraShaderable {
    public:
        enum { MaxTextureSize = 8; }
        
        enum MaterialType {
            WireFrame,
            PointCloud,
            Solid,
            NormalMap,
        };
        
        /**
         * Set/Get
         **/
        SORA_DEF_SET_GET(MaterialType, MaterialType, mMaterialType);
        
        SORA_DEF_SET_GET(SoraColorRGBA, AmbientColor, mAmbientColor);
        SORA_DEF_SET_GET(SoraColorRGBA, DiffuseColor, mDiffuseColor);
        SORA_DEF_SET_GET(SoraColorRGBA, SpecularColor, mSpecularColor);
        SORA_DEF_SET_GET(SoraColorRGBA, EmissiveColor, mEmissiveColor);
        
        // textures
        void              setTexture(int32 index, SoraTextureHandle tex);
        SoraTextureHandle getTexture(int32 index) const;
        
    private:
        MaterialType mMaterialType;
        
        SoraColorRGBA mAmbientColor;
        SoraColorRGBA mDiffuseColor;
        SoraColorRGBA mSpecularColor;
        SoraColorRGBA mEmissiveColor;
        
        uint32 mTextureSize;
        SoraTextureHandle mTextures[MaxTextureSize];
    };
    
    
} // namespace sora


#endif
