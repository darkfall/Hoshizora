//
//  SoraMesh.h
//  Sora
//
//  Created by Robert Bu on 11/8/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraMesh_h
#define Sora_SoraMesh_h

#include "SoraPlatform.h"
#include "SoraAutoPtr.h"
#include "SoraVertex.h"
#include "SoraPreDeclare.h"
#include "SoraRenderBuffer.h"
#include "util/SoraArray.h"
#include "util/SoraPointTemplate.h"

namespace sora {
    
    struct SoraVertexN {
        SoraVector3 pos;
        SoraVector3 normal;
        uint32 col;
        float u, v;
    };
    
    static SoraVertexFormat VertexNFormat = SoraVertexFormat(VertexXYZ | VertexNormal | VertexColor | VertexUV).offsetUV(sizeof(SoraVector3)*2+sizeof(uint32))
                 .offsetXYZ(0)
                 .offsetColor(sizeof(SoraVector3)*2)
                 .offsetNormal(sizeof(SoraVector3));
    
    class SORA_API SoraMesh {
    public:
        SoraMesh();
        ~SoraMesh();
        
        typedef SoraAutoPtr<SoraMesh> Ptr;
        
        uint32 getVertexCount() const;
        uint32 getIndexCount() const;
                
        SoraRenderBuffer::Ptr getVertexBuffer() const;
        SoraRenderBuffer::Ptr getIndexBuffer() const;
        
        void beginVertex();
        void pushVertex(const SoraVertexN& vertex);
        void pushVertex(SoraVertexN* pointer, uint32 count);
        void endVertex();
        
        void beginIndex();
        void pushIndex(uint32 index);
        void pushIndex(uint32* index, uint32 count);
        void endIndex();
        
    private:
        friend class SoraModel;
        
        SoraRenderBuffer::Ptr mVertexBuffer;
        SoraRenderBuffer::Ptr mIndexBuffer;
        SoraHandle mMaterialHandle;
        
        SoraArray<SoraVertexN> mVertexBufferArray;
        SoraArray<uint32> mIndexBufferArray;
    };
    
} // namespace sora


#endif
