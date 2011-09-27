//
//  SoraShape.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/23/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraShape.h"
#include "SoraCore.h"


namespace sora {
    
    SoraShape::SoraShape(SoraTextureHandle handle, int32 mode):
    mCompiled(false),
    mCompiledVertex(0),
    mTexture(handle),
    mRenderMode(mode) {
        
    }
    
    void SoraShape::addPoint(float x, float y, uint32 color, float tx, float ty) {
        if(mTexture && tx == 0.0 && ty == 0.0) {
            SoraTexture* tex = (SoraTexture*)mTexture;
            tx = 1.0 / tex->mTextureWidth;
            ty = 1.0 / tex->mTextureHeight;
        }
        SoraVertex vertex(x, y, tx, ty);
        vertex.col = color;
        mVertices.push_back(vertex);
        mCompiled = false;
    }
    
    void SoraShape::addPoint(const SoraVector& pos, uint32 color, float tx, float ty) {
        if(mTexture && tx == 0.0 && ty == 0.0) {
            SoraTexture* tex = (SoraTexture*)mTexture;
            tx = 1.0 / tex->mTextureWidth;
            ty = 1.0 / tex->mTextureHeight;
        }
        SoraVertex vertex(pos.x, pos.y, tx, ty);
        vertex.col = color;
        mVertices.push_back(vertex);
        mCompiled = false;
    }
    
    void SoraShape::setPoint(uint32 index, float x, float y, uint32 color, float tx, float ty) {
        sora_assert(index < mVertices.size());
        
        if(index < mVertices.size()) {
            SoraVertex& vertex = mVertices[index];
            vertex.x = x;
            vertex.y = y;
            vertex.col = color;
            vertex.tx = tx;
            vertex.ty = ty;
        }
    }
    
    SoraVertex* SoraShape::compile() {
        if(mCompiled && mCompiledVertex)
            return mCompiledVertex;
        
        SoraVertex* vertex;
        vertex = new SoraVertex[mVertices.size()];
        sora_assert(vertex);
        
        VertexArray::iterator it = mVertices.begin();
        VertexArray::iterator end = mVertices.end();
        
        //int index = 0;
        for(; it != end; ++it) {
            memcpy(vertex, &(*it), sizeof(SoraVertex));
            ++vertex;
        }
        mCompiledVertex = vertex;
        
        mCompiled = true;
        return mCompiledVertex;
    }
    
    void SoraShape::render() {
        compile();
        
        sora_assert(mCompiled && mCompiledVertex);
        
        sora::SoraCore::Instance()->pushTransformMatrix();
        sora::SoraCore::Instance()->setTransform(0.f, 0.f, getPositionX(), getPositionY());
        
        sora::SoraCore::Instance()->renderWithVertices(mTexture, BLEND_DEFAULT, mCompiledVertex, mVertices.size());
        
        sora::SoraCore::Instance()->popTransformMatrix();
    }
    
    size_t SoraShape::size() const {
        return mVertices.size();
    }
    
    void SoraShape::setTexture(SoraTextureHandle texture) {
        mTexture = texture;
    }
    
    SoraTextureHandle SoraShape::getTexture() const {
        return mTexture;
    }
    
    SoraVector SoraShape::computeNormal(const SoraVector& p1, const SoraVector& p2) {
        SoraVector normal;
        
        normal.x = p1.y - p2.y;
        normal.y = p2.x - p1.x;
        float len = sqrt(normal.x * normal.x + normal.y * normal.y);
        if(len == 0.f) {
            return SoraVector();
        }
        
        normal.x /= len;
        normal.y /= len;
        
        return normal;
    }
    
    
} // namespace sora