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
    mRenderMode(mode),
    mTexture(handle),
    mOutline(false),
    mOutlineWidth(1.f),
    mOutlineColor(0xFFFFFFFF),
    mOutlineOnly(false),
    mRenderToSprite(false),
    mRenderTarget(0),
    mSprite(0),
    mClosed(false) {
        
    }
    
    SoraShape::~SoraShape() {
        if(mRenderTarget)
            SoraCore::Instance()->freeTarget(mRenderTarget);
        if(mSprite)
            delete mSprite;
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
        if(mVertices.size() == 0)
            return 0;
        
        if(mCompiled && mCompiledVertex)
            return mCompiledVertex;
        
        SoraVertex* vertex;
        vertex = new SoraVertex[mVertices.size()+1];
        sora_assert(vertex);
        
        VertexArray::iterator it = mVertices.begin();
        VertexArray::iterator end = mVertices.end();
        
        int32 textureWidth = 0;
        int32 textureHeight = 0;
        if(mTexture) {
            textureWidth = SoraTexture::GetOriginalWidth(mTexture);
            textureHeight = SoraTexture::GetOriginalHeight(mTexture);
        }
        
        //int index = 0;
        SoraVertex* tmpVertex = vertex;
        for(; it != end; ++it) {
            memcpy(tmpVertex, &(*it), sizeof(SoraVertex));
            
            if(mTexture && tmpVertex->tx == 0.f && tmpVertex->ty == 0.f) {
                tmpVertex->tx = (float)tmpVertex->x / textureWidth;
                tmpVertex->ty = (float)tmpVertex->y / textureHeight;
            }
            
            ++tmpVertex;
        }
        memcpy(tmpVertex, &(*mVertices.begin()), sizeof(SoraVertex));
        
        mCompiledVertex = vertex;
        
        mCompiled = true;
        
        if(mRenderToSprite && mSprite && mRenderTarget)
            spriteRender();

        return mCompiledVertex;
    }
    
    void SoraShape::setRenderMode(int32 mode) {
        mRenderMode = mode;
    }
    
    int32 SoraShape::getRenderMode() const {
        return mRenderMode;
    }
    
    void SoraShape::spriteRender() {
        compile();
        SoraCore* core = SoraCore::Instance();

        core->beginScene(0, mRenderTarget);
        if(!mOutlineOnly) {
            core->renderWithVertices(mTexture, BLEND_DEFAULT, mCompiledVertex, mVertices.size(), mRenderMode);
        }
        
        if(mOutline) {
            SoraVertex* vertex = mCompiledVertex.get();
            int32 end = mVertices.size();
            if(!mClosed) 
                end -= 1;
            
            for(int i=0; i<end; ++i){
                SoraVertex* next = vertex + 1;
                core->renderLine(vertex->x, vertex->y, next->x, next->y, mOutlineColor, mOutlineWidth);
                
                ++vertex;
            }
        }
        core->endScene();
    }
    
    void SoraShape::render() {
        SoraCore* core = SoraCore::Instance();

        if(mRenderToSprite && mSprite && mRenderTarget) {            
            mSprite->render(getPositionX(), getPositionY());
            return;
        }
        
        if(!mCompiled || !mCompiledVertex)
            compile();
        
        sora_assert(mCompiled && mCompiledVertex);
        
        core->pushTransformMatrix();
        core->setTransform(0.f, 0.f, getPositionX(), getPositionY());
        
        if(!mOutlineOnly) {
            core->renderWithVertices(mTexture, BLEND_DEFAULT, mCompiledVertex, mVertices.size(), mRenderMode);
        }
        
        if(mOutline) {
            SoraVertex* vertex = mCompiledVertex.get();
            int32 end = mClosed ? mVertices.size(): mVertices.size() - 1;
            for(int i=0; i<end; ++i){
                SoraVertex* next = vertex + 1;
                core->renderLine(vertex->x, vertex->y, next->x, next->y, mOutlineColor, mOutlineWidth);
                
                ++vertex;
            }
        }

        core->popTransformMatrix();
    }
    
    size_t SoraShape::size() const {
        return mVertices.size();
    }
    
    void SoraShape::enableOutline(float width, uint32 color) {
        mOutline = true;
        mOutlineWidth = width;
        mOutlineColor = color;
    }
    
    void SoraShape::disableOutline() {
        mOutline = false;
    }
    
    void SoraShape::setOutlineOnly(bool flag) {
        mOutlineOnly = flag;
        if(flag)
            mOutline = true;
    }
    
    void SoraShape::setTexture(SoraTextureHandle texture) {
        mTexture = texture;
        
        mCompiled = false;
    }
    
    SoraTextureHandle SoraShape::getTexture() const {
        return mTexture;
    }
    
    SoraShape& SoraShape::operator=(const SoraShape& rhs) {
        if(this != &rhs) {
            mVertices = rhs.mVertices;
            mCompiled = false;
            mCompiledVertex.reset();
            
            mRenderMode = rhs.mRenderMode;
            mTexture = rhs.mTexture;
            
            mOutline = rhs.mOutline;
            mOutlineWidth = rhs.mOutlineWidth;
            mOutlineColor = rhs.mOutlineColor;
            mOutlineOnly = rhs.mOutlineOnly;
            
            if(mRenderTarget) {
                sora::SoraCore::Instance()->freeTarget(mRenderTarget);
                mRenderTarget = 0;
            }
            if(mSprite) {
                delete mSprite;
                mSprite = 0;
            }
        }
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
    
    void SoraShape::enableRenderToSprite(bool flag) {
        mRenderToSprite = flag;
        if(mRenderToSprite) {
            if(!mRenderTarget) {
                float farW = 0.f, farH = 0.f;
                if(!mTexture) {
                    VertexArray::iterator it = mVertices.begin();
                    VertexArray::iterator end = mVertices.end();
                    
                    for(; it != end; ++it) {
                        if(farW < it->x)
                            farW = it->x;
                        if(farH < it->y)
                            farH = it->y;
                    }
                } else {
                    farW = SoraTexture::GetOriginalWidth(mTexture);
                    farH = SoraTexture::GetOriginalHeight(mTexture);
                }
                mRenderTarget = SoraCore::Instance()->createTarget((int32)farW+1, (int32)farH+1);
                
                if(mRenderTarget) {
                    mSprite = new SoraSprite(SoraCore::Instance()->getTargetTexture(mRenderTarget));
                    sora_assert(mSprite);
                }
                
                mCompiled = false;
                spriteRender();
            }
        } else {
            if(mRenderTarget) {
                SoraCore::Instance()->freeTarget(mRenderTarget);
                mRenderTarget = 0;
            }
            if(mSprite) {
                delete mSprite;
                mSprite = 0;
            }
        }
    }
    
    SoraSprite* SoraShape::getSprite() const {
        return mSprite;
    }
    
    void SoraShape::setClosed(bool flag) {
        mClosed = flag;
    }
    
    SoraShape SoraShape::Line(float x1, float y1, float x2, float y2, float width, uint32 col) {
        SoraVector p1(x1, y1);
        SoraVector p2(x2, y2);
        
        // Compute the extrusion direction
        SoraVector normal = computeNormal(p1, p2);
        normal *= width / 2;
        
        // Create the shape's points
        SoraShape shape;
        shape.addPoint(p1 - normal, col);
        shape.addPoint(p2 - normal, col);
        shape.addPoint(p2 + normal, col);
        shape.addPoint(p1 + normal, col);
        
        // Compile it
        shape.compile();
        return shape;
    }
    
    SoraShape SoraShape::Circle(float x, float y, float r, uint32 col, int32 maxSegment) {
        sora_assert(maxSegment != 0);
        
        SoraShape shape;
        
        SoraVector center(x, y);
        for(int i=0; i <= maxSegment; ++i) {
            float a = i * 2 * F_PI / maxSegment;
            SoraVector off(cos(a), sin(a));
            
            shape.addPoint(center + off * r, col);
        }
        
        shape.compile();
        return shape;
    }
    
    SoraShape SoraShape::Ring(float x, float y, float r, float thickness, uint32 col, int32 maxSegment) {
        sora_assert(maxSegment != 0);
        
        SoraShape shape;
        
        SoraVector center(x, y);
        for(int i=0; i <= maxSegment; ++i) {
            float a = i * 2 * F_PI / maxSegment;
            SoraVector off(cos(a), sin(a));
            
            shape.addPoint(center + off * r, col);
        }
        
        shape.compile();
        shape.enableOutline(thickness, col);
        shape.setOutlineOnly(true);
        return shape;
    }
    
    SoraShape SoraShape::Fan(float x, float y, float r, float a, float b, uint32 col, int32 maxSegment) {
        sora_assert(maxSegment != 0);
        
        SoraShape shape;
        
        SoraVector center(x, y);
        float d = b - a;
        
        for(int i=0; i <= maxSegment; ++i) {
            float angle = a + i * (b - a) / maxSegment;
            SoraVector off(cos(angle), sin(angle));
            
            shape.addPoint(center + off * r, col);
        }
        
        shape.compile();
        return shape;
    }
    
    SoraShape SoraShape::Arc(float x, float y, float r, float a, float b, float thickness, uint32 col, int32 maxSegment) {
        sora_assert(maxSegment != 0);
        
        SoraShape shape;
        
        SoraVector center(x, y);
        float d = b - a;
        
        for(int i=0; i <= maxSegment; ++i) {
            float angle = a + i * (b - a) / maxSegment;
            SoraVector off(cos(angle), sin(angle));
            
            shape.addPoint(center + off * r, col);
        }
        
        shape.compile();
        shape.setClosed(false);
        shape.enableOutline(thickness, col);
        shape.setOutlineOnly(true);

        return shape;
    }
    
    SoraShape SoraShape::Box(float x1, float y1, float x2, float y2, uint32 col) {
        SoraShape shape;
        
        shape.addPoint(SoraVector(x1, y1), col);
        shape.addPoint(SoraVector(x2, y1), col);
        shape.addPoint(SoraVector(x2, y2), col);
        shape.addPoint(SoraVector(x1, y2), col);
        
        shape.compile();
        return shape;
    }
    
    
} // namespace sora