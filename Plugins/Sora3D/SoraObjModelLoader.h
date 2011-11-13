//
//  SoraObjMesh.h
//  Sora
//
//  Created by Ruiwei Bu on 11/9/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraObjMeshLoader_h
#define Sora_SoraObjMeshLoader_h

#include "SoraModelLoader.h"
#include "SoraMesh.h"
#include "SoraMaterial.h"

namespace sora {
    
    class SoraObjModelLoader: public SoraModelLoader {
    public:
        virtual SoraModel::Ptr loadModelFromData(const char* data, uint32 size);
        
        void parseLine(const char* str, SoraMesh* mesh);
    };
    
} // namespace sora


#endif
