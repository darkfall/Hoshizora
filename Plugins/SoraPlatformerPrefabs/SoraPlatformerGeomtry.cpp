/*
 *  SoraPlatformerGeomtry.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraPlatformerGeomtry.h"
#include "SoraBox2D/SoraPhysicalWorld.h"

namespace sora {
    
    SoraPlatformerGeometry::SoraPlatformerGeometry(HSORATEXTURE tex, float32 x, float32 y, float32 w, float32 h):
    mSprite(NULL),
    mGeometry(NULL),
    mBuilt(false) {
        mSprite = new SoraSprite(tex, x, y, w, h);
        mRenderer.setSprite(mSprite);
        mRenderer.setRenderMode(SORA_TRIANGLES_STRIP);
    }
    
    SoraPlatformerGeometry::~SoraPlatformerGeometry() {
        if(mSprite)
            delete mSprite;
        if(mGeometry)
            delete mGeometry;
    }
    
    void SoraPlatformerGeometry::setVertexMode(int32 mode) {
        mRenderer.setRenderMode(mode);
    }
    
    int32 SoraPlatformerGeometry::addVertex(float32 x, float32 y) {
        float32 tx, ty;
        float32 width = mSprite->getSpriteWidth();
        float32 height = mSprite->getSpriteHeight();
        
        if( x <= width ) 
            tx = x / width;
        else { 
            float32 rx = x;
            while( rx > width ) 
                rx -= width;
            tx = rx / width;
        }
        
        if( y <= height )
            ty = y / height;
        else {
            float32 ry = y;
            while( ry > height )
                ry -= height;
            ty = ry / height;
        }
        
        mBuilt = false;
        return mRenderer.addVertex(getPositionX() + x, 
                                   getPositionY() + y, 
                                   tx, 
                                   ty);
    }
    
    void SoraPlatformerGeometry::delVertex(int32 vid) {
        mRenderer.delVertx(vid);
    }
    
    void SoraPlatformerGeometry::create() {
        if(mRenderer.getVertexCount() == 0 || mBuilt)
            return;
        
        if(mGeometry) {
            delete mGeometry;
            mGeometry = NULL;
        }
        
        b2Vec2* vertices = new b2Vec2[mRenderer.getVertexCount()];
        SoraVertex* vertexBuffer = mRenderer.buildAndGetVertexList();
        if(vertexBuffer) {
            for(int i=0; i<mRenderer.getVertexCount(); ++i) {
                vertices[i].x = PixelToB2Cor(vertexBuffer[i].x);
                vertices[i].y = PixelToB2Cor(vertexBuffer[i].y);
            }
        }
        
        mBuilt = true;
        mGeometry = new SoraPhysicalObject(getPositionX(),
                                           getPositionY(),
                                           B2CreatePolygen(vertices, 
                                                           mRenderer.getVertexCount()),
                                           1.f,
                                           false);
        delete vertices;
        if(!mGeometry)
            THROW_SORA_EXCEPTION("Error creating geometry");
    }
    
    void SoraPlatformerGeometry::render() {
        if(!mBuilt)
            create();
        mRenderer.render();
    }
    
    uint32 SoraPlatformerGeometry::update(float32 dt) {
        mRenderer.update(dt);
        if(mGeometry) {
            mGeometry->update(dt);
            setPosition(mGeometry->getPositionX(), mGeometry->getPositionY());
            mSprite->setPosition(mGeometry->getPositionX(), mGeometry->getPositionY());
        }
    }
    
    void SoraPlatformerGeometry::clearVertices() {
        if(mGeometry)
            delete mGeometry;
        mRenderer.clearVertices();
    }

    SoraStream SoraPlatformerGeometry::saveToStream() {
        
    }
    
    bool SoraPlatformerGeometry::loadFromData(void* data, ulong32 size) {
        
    }
} // namespace sora