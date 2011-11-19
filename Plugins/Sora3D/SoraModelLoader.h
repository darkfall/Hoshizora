//
//  SoraModelLoader.h
//  Sora
//
//  Created by Ruiwei Bu on 11/6/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraModelLoader_h
#define Sora_SoraModelLoader_h

#include "SoraPlatform.h"
#include "SoraAutoPtr.h"
#include "SoraModel.h"

#include "SoraAABB3.h"
#include "SoraSphere.h"
#include "SoraPlane.h"

#include "factory/SoraDynamicFactory.h"

namespace sora {
    
    class SoraModelLoader;
    
    /**
     * Simple model loader class
     * Currently only support .obj from wavefront without meterials
     **/
    typedef SoraDynamicFactory<SoraModelLoader> SoraModelLoaderFactory;
    
    class SoraModelLoader {
    public:
        virtual ~SoraModelLoader() { }
        
        virtual SoraModel::Ptr loadModelFromData(const char* data, uint32 size) = 0;
        
        static SoraModelLoaderFactory& GetModelLoaderFactory();
        static SoraModel::Ptr LoadModel(const StringType& file);
        static SoraModel::Ptr LoadModelFromRawData(void* data, uint32 size, const char* type);
        
        static SoraModel::Ptr BuildModelFromAABB(const SoraAABB3& aabb, SoraMaterial::Ptr mat);
        static SoraModel::Ptr BuildModelFromSphere(const SoraSphere& sphere, uint32 slices, SoraMaterial::Ptr mat);
        static SoraModel::Ptr BuildModelFromPlane(const SoraPlane& plane, SoraMaterial::Ptr mat);
    };
    
    
} // namespace sora



#endif
