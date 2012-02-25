//
//  SoraMesh.h
//  Sora
//
//  Created by Ruiwei Bu on 11/8/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraMesh_h
#define Sora_SoraMesh_h

#include "SoraPlatform.h"
#include "SoraAutoPtr.h"
#include "SoraVertex.h"
#include "SoraPreDeclare.h"
#include "util/SoraArray.h"
#include "util/SoraPointTemplate.h"

namespace sora {
    
    struct SoraModelVertex {
        float x, y, z;
        float nx, ny, nz;
        uint32 color;
        float tx, ty;
    };
    
    class SORA_API SoraMesh {
    public:
        typedef SoraAutoPtr<SoraMesh> Ptr;
        
        typedef SoraArray<float3> VertexList;
        typedef SoraArray<float3> NormalList;
        typedef SoraArray<float2> UVList;
        typedef SoraArray<SoraVertex> FaceList;
        
        VertexList mVertices;
        NormalList mNormals;
        UVList     mUVs;
        FaceList   mFaces;
        
        SoraHandle mMaterialHandle;
                
        uint32 vertexCount() const {
            return mVertices.size();
        }
        
        uint32 normalCount() const {
            return mNormals.size();
        }
        
        uint32 UVCount() const {
            return mUVs.size();
        }
        
        uint32 faceCount() const {
            return mFaces.size() / 3;
        }
    };
    
} // namespace sora


#endif
