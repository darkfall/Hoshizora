//
//  SoraMesh.cpp
//  SoraF
//
//  Created by Ruiwei Bu on 2/25/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "SoraMesh.h"
#include "SoraCore.h"
#include "SoraRenderSystem.h"

namespace sora {
    
    SoraMesh::SoraMesh() {
    }
    
    SoraMesh::~SoraMesh() {
    }
    
    void SoraMesh::beginVertex() {
        mVertexBufferArray.clear();
    }
    
    void SoraMesh::pushVertex(const SoraVertexN& vertex) {
        mVertexBufferArray.push_back(vertex);
    }
    
    void SoraMesh::pushVertex(SoraVertexN* pointer, uint32 count) {
        mVertexBufferArray.append(pointer, count);
    }
    
    void SoraMesh::endVertex() {        
        if(mVertexBufferArray.size() > 0) {
            
            if(!mVertexBuffer) {
                mVertexBuffer = SoraCore::Ptr->getRenderSystem()->createVertexBuffer(SoraRenderBuffer::ReadWrite, 
                                                                                     SoraRenderBuffer::Dynamic, 
                                                                                     mVertexBufferArray.size(), 
                                                                                     mVertexBufferArray.begin(),
                                                                                     VertexNFormat);
            } else {
                mVertexBuffer->resize(mVertexBufferArray.size());
                
                sora_assert(mVertexBuffer->count() == mVertexBufferArray.size());
                
                void* data = mVertexBuffer->map();
                memcpy(data, mVertexBufferArray.begin(), sizeof(SoraVertexN) * mVertexBufferArray.size());
                mVertexBuffer->unmap();
            }
        }
    }
    
    void SoraMesh::beginIndex() {
        mIndexBufferArray.clear();
    }
    
    void SoraMesh::pushIndex(uint32 index) {
        mIndexBufferArray.push_back(index);
    }
    
    void SoraMesh::pushIndex(uint32* index, uint32 count) {
        mIndexBufferArray.append(index, count);
    }
    
    void SoraMesh::endIndex() {        
        if(mIndexBufferArray.size() > 0) {
                     
            if(!mIndexBuffer) {
                mIndexBuffer = SoraCore::Ptr->getRenderSystem()->createIndexBuffer(SoraRenderBuffer::ReadWrite, 
                                                                                   SoraRenderBuffer::Dynamic, 
                                                                                   mIndexBufferArray.size(), 
                                                                                   mIndexBufferArray.begin());
            } else {    
                mIndexBuffer->resize(mIndexBufferArray.size());
                
                sora_assert(mIndexBuffer->count() == mIndexBufferArray.size());
                
                void* data = mIndexBuffer->map();
                memcpy(data, mIndexBufferArray.begin(), sizeof(uint32) * mIndexBufferArray.size());
                mIndexBuffer->unmap();
            }
        }
    }
    
    SoraRenderBuffer::Ptr SoraMesh::getVertexBuffer() const {
        return mVertexBuffer;
    }
  
    SoraRenderBuffer::Ptr SoraMesh::getIndexBuffer() const {
        return mIndexBuffer;
    }
    
    uint32 SoraMesh::getVertexCount() const {
        return mVertexBuffer->count();
    }
    
    uint32 SoraMesh::getIndexCount() const {
        return mIndexBuffer->count();
    }
}
