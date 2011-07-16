//
//  SoraCustomShapeSprite.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/1/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraCustomShapeSprite.h"
#include "SoraCore.h"

#include "common.h"

namespace sora {
    
    SoraCustomShapeSprite::SoraCustomShapeSprite():
    mSprite(NULL),
    mVertexBuffer(NULL),
    mNextVertexId(0),
    mRenderMode(0),
    mVertexBufferBuilt(false) {
        
    }
    
    SoraCustomShapeSprite::SoraCustomShapeSprite(SoraSprite* sprite, int32 renderMode):
    mSprite(sprite),
    mNextVertexId(0),
    mVertexBuffer(NULL),
    mRenderMode(renderMode),
    mVertexBufferBuilt(false) {
       
    }
    
    SoraCustomShapeSprite::~SoraCustomShapeSprite() {
        if(mVertexBuffer)
            delete []mVertexBuffer;
    }
    
    void SoraCustomShapeSprite::setSprite(SoraSprite* sprite) {
        mSprite = sprite;
    }
    
    void SoraCustomShapeSprite::clearVertices() {
        mVertexMap.clear();
        mVertexIndexes.clear();
        
        mNextVertexId = 0;
        if(mVertexBuffer && !mVertexBufferBuilt) {
            delete [] mVertexBuffer;
            mVertexBuffer = NULL;
        }
        mVertexBufferBuilt = false;
    }
    
    SoraSprite* SoraCustomShapeSprite::getSprite() const {
        return mSprite;
    }
    
    uint32 SoraCustomShapeSprite::update(float32 dt) {
        if(mSprite)
            return mSprite->update(dt);
        return 0;
    }
    
    void SoraCustomShapeSprite::render() {
        if(mSprite) {
            if(!mVertexBufferBuilt)
                buildVertexBuffer();
            
            if(mVertexBuffer) 
                mSprite->renderWithVertices(mVertexBuffer, mNextVertexId, mRenderMode);
            else
                mSprite->render();
        }
    }
    
    void SoraCustomShapeSprite::render(int32 renderMode) {
        if(mSprite) {
            if(!mVertexBufferBuilt)
                buildVertexBuffer();
            
            if(mVertexBuffer) 
                mSprite->renderWithVertices(mVertexBuffer, mNextVertexId, renderMode);
            else
                mSprite->render();
        }
    }
    
    void SoraCustomShapeSprite::setRenderMode(int32 renderMode) {
        mRenderMode = renderMode;
    }
    
    int32 SoraCustomShapeSprite::getRenderMode() const {
        return mRenderMode;
    }
    
    int32 SoraCustomShapeSprite::addVertex(float32 screenposx, float32 screenposy, float32 texposx, float32 texposy, float32 screenz) {
        SoraVertex vertex;
        vertex.x = screenposx; vertex.y = screenposy; vertex.z = screenz;
        vertex.tx = texposx; vertex.ty = texposy;
        
        mVertexMap[mNextVertexId] = vertex;
        
        mVertexBufferBuilt = false;
        
        ++mNextVertexId;
        return mNextVertexId-1;
    }
    
    int32 SoraCustomShapeSprite::addScreenMappingVertex(float32 x, float32 y, float32 screenz) {
        int32 screenWidth = SORA->getScreenWidth();
        int32 screenHeight = SORA->getScreenHeight();
        
        SoraVertex vertex;
        vertex.x = x; vertex.y = y; vertex.z = screenz;
        vertex.tx = x / screenWidth;
        vertex.ty = y / screenHeight;
        
        mVertexMap[mNextVertexId] = vertex;
        
        mVertexBufferBuilt = false;
        
        ++mNextVertexId;
        return mNextVertexId-1;
    }
    
    int32 SoraCustomShapeSprite::addVertex(const SoraVertex& vertex) {
        mVertexMap[mNextVertexId] = vertex;
        
        mVertexBufferBuilt = false;
        
        ++mNextVertexId;
        return mNextVertexId-1;
    }
    
    void SoraCustomShapeSprite::delVertx(int32 vid) {
        VERTEX_MAP::iterator itVertex = mVertexMap.find(vid);
        if(itVertex != mVertexMap.end()) {
            mVertexMap.erase(itVertex);
            
            mVertexBufferBuilt = false;
        }
    }
    
    void SoraCustomShapeSprite::setVertexColor(int32 vid, uint32 color) {
        VERTEX_MAP::iterator itVertex = mVertexMap.find(vid);
        if(itVertex != mVertexMap.end()) {
            itVertex->second.col = color;
            
            if(mVertexBufferBuilt) {
                int32 index = mVertexIndexes[vid];
                mVertexBuffer[index].col = color;
            }
        }
    }
    
    uint32 SoraCustomShapeSprite::getVertexColor(int32 vid) const {
        VERTEX_MAP::const_iterator itVertex = mVertexMap.find(vid);
        if(itVertex != mVertexMap.end())
            return itVertex->second.col;
        return 0;
    }
    
    void SoraCustomShapeSprite::setVertexZ(int32 vid, float32 z) {
        VERTEX_MAP::iterator itVertex = mVertexMap.find(vid);
        if(itVertex != mVertexMap.end()) {
            itVertex->second.z = z;
           
            if(mVertexBufferBuilt) {
                int32 index = mVertexIndexes[vid];
                mVertexBuffer[index].z = z;
            }
        }
    }
    
    float32 SoraCustomShapeSprite::getVertexZ(int32 vid) const {
        VERTEX_MAP::const_iterator itVertex = mVertexMap.find(vid);
        if(itVertex != mVertexMap.end())
            return itVertex->second.z;
        return 0.f;
    }
    
    void SoraCustomShapeSprite::setVertexPos(int32 vid, float32 sposx, float32 sposy) {
        VERTEX_MAP::iterator itVertex = mVertexMap.find(vid);
        if(itVertex != mVertexMap.end()) {
            itVertex->second.x = sposx;
            itVertex->second.y = sposy;
            
            if(mVertexBufferBuilt) {
                int32 index = mVertexIndexes[vid];
                mVertexBuffer[index].x = sposx;
                mVertexBuffer[index].x = sposy;
            }
        }
    }
    
    void SoraCustomShapeSprite::getVertexPos(int32 vid, float32* sposx, float32* sposy) {
        VERTEX_MAP::iterator itVertex = mVertexMap.find(vid);
        if(itVertex != mVertexMap.end()) {
            *sposx = itVertex->second.x;
            *sposy = itVertex->second.y;
        }
    }
    
    void SoraCustomShapeSprite::setVertexTexturePos(int32 vid, float32 tposx, float32 tposy) {
        VERTEX_MAP::iterator itVertex = mVertexMap.find(vid);
        if(itVertex != mVertexMap.end()) {
            itVertex->second.tx = tposx;
            itVertex->second.ty = tposy;
            
            if(mVertexBufferBuilt) {
                int32 index = mVertexIndexes[vid];
                mVertexBuffer[index].tx = tposx;
                mVertexBuffer[index].ty = tposy;
            }
        }
    }
    
    void SoraCustomShapeSprite::getVertexTexturePos(int32 vid, float32* tposx, float32 *tposy) {
        VERTEX_MAP::iterator itVertex = mVertexMap.find(vid);
        if(itVertex != mVertexMap.end()) {
            *tposx = itVertex->second.tx;
            *tposy = itVertex->second.ty;
        }
    }
    
    void SoraCustomShapeSprite::exchangeVertex(int32 vid1, int32 vid2) {
        VERTEX_MAP::iterator itVertex1 = mVertexMap.find(vid1);
        VERTEX_MAP::iterator itVertex2 = mVertexMap.find(vid2);
        
        if(itVertex1 != mVertexMap.end() &&
           itVertex2 != mVertexMap.end()) {
            SoraVertex tmp = itVertex1->second;
            itVertex1->second = itVertex2->second;
            itVertex2->second = tmp;
            
            if(mVertexBufferBuilt) {
                int32 index1 = mVertexIndexes[vid1];
                int32 index2 = mVertexIndexes[vid2];
                
                std::swap(mVertexBuffer[index1],
                          mVertexBuffer[index2]);
            }
        }
    }
    
    void SoraCustomShapeSprite::buildVertexBuffer() {
        if(mVertexMap.size() == 0)
            return;
        
        VERTEX_MAP::iterator itVertex = mVertexMap.begin();
                
        if(mVertexBufferBuilt && mVertexBuffer) {
            delete []mVertexBuffer;
        }
        
        mVertexBuffer = new SoraVertex[mVertexMap.size()];
        if(!mVertexBuffer) {
            mVertexBufferBuilt = false;
            return;
        }
        
        int index = 0;
        while(itVertex != mVertexMap.end()) {
            mVertexBuffer[index] = itVertex->second;
            mVertexIndexes[itVertex->first] = index;
            
            ++index;
            ++itVertex;
        }
        mVertexBufferBuilt = true;
    }
    
    int32 SoraCustomShapeSprite::getVertexCount() const {
        return mVertexMap.size();
    }
    
    bool SoraCustomShapeSprite::saveVertciesToFile(const SoraWString& path) {
        FILE* file = sora_fopenw(path.c_str(), "wb");
        if(!file)
            return false;
        
        cFileWriteT(file, (int32)mVertexMap.size());
        cFileWriteT(file, mRenderMode);
        
        VERTEX_MAP::iterator itVertex = mVertexMap.begin();
        while(itVertex != mVertexMap.end()) {
            //cFileWriteT(file, itVertex->first);
            
            SoraVertex vertex = itVertex->second;
            cFileWriteT(file, vertex.x);
            cFileWriteT(file, vertex.y);
            cFileWriteT(file, vertex.z);
            cFileWriteT(file, vertex.tx);
            cFileWriteT(file, vertex.ty);
            cFileWriteT(file, vertex.col);
            
            ++itVertex;
        }
        fclose(file);
        return true;
    }
    
    SoraStream SoraCustomShapeSprite::writeToStream() {
        SoraStream stream;
        stream << (int32)mVertexMap.size();
        stream << mRenderMode;
        VERTEX_MAP::iterator itVertex = mVertexMap.begin();
        while(itVertex != mVertexMap.end()) {            
            SoraVertex vertex = itVertex->second;
            stream << vertex.x;
            stream << vertex.y;
            stream << vertex.z;
            stream << vertex.tx;
            stream << vertex.ty;
            stream << vertex.col;
            
            ++itVertex;
        }
        return stream;
    }
    
    bool SoraCustomShapeSprite::loadFromData(void* data, ulong32 datasize) {
        SoraMemoryBuffer buffer(data, datasize);
        
        clearVertices();
        
        int32 size;
        bool error = false;
        
        buffer.read(&size);
        buffer.read(&mRenderMode);
        for(int32 i=0; i<size; ++i) {
            SoraVertex vertex;            
            //int32 hid = cFileReadT<int32>(file, error);
            
            error = !buffer.read(&vertex.x);
            error = !buffer.read(&vertex.y);
            error = !buffer.read(&vertex.z);
            error = !buffer.read(&vertex.tx);
            error = !buffer.read(&vertex.ty);
            error = !buffer.read(&vertex.col);
            
            if(error)
                THROW_SORA_EXCEPTION("Invalid vertex data file");
            
            addVertex(vertex);
        }
        
        return true;
    }
    
    bool SoraCustomShapeSprite::loadVerticesFromFile(const SoraWString& path) {
        SoraResourceFileAuto data(path);
        if(!data.isValid())
            return false;
        
        return loadFromData(data, data.size());
    }
    
    SoraVertex* SoraCustomShapeSprite::buildAndGetVertexList() {
        if(!mVertexBufferBuilt)
            buildVertexBuffer();
        return mVertexBuffer;
    }
    
} // namespace sora