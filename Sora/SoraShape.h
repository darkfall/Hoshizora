//
//  SoraShape.h
//  Sora
//
//  Created by Ruiwei Bu on 9/23/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraShape_h
#define Sora_SoraShape_h

#include "SoraPlatform.h"
#include "SoraMath.h"
#include "SoraObject.h"
#include "SoraTexture.h"
#include "SoraAutoPtr.h"

namespace sora {
    
    class SoraVertex;
    class SoraSprite;
    
    /**
     *   A base custom shape class that can be positioned,
     *   rendered, output it's vertex data for sprite to use
     *   and adapt a texture to render
     **/
    
    class SoraShape: public SoraMovable {
    public:
        SoraShape(SoraTextureHandle texture=0, int32 mode=SORA_TRIANGLES_FAN);
        ~SoraShape();
        
        /**
         * Add a point
         **/
        void addPoint(float x, float y, uint32 color, float tx=0.f, float ty=0.f);
        void addPoint(const SoraVector& pos, uint32 color, float tx=0.f, float ty=0.f);
        
        void setPoint(uint32 index, float x, float y, uint32 color, float tx=0.f, float ty=0.f);
        SoraVertex getPoint(uint32 index);
        
        void setRenderMode(int32 mode);
        int32 getRenderMode() const;
        
        void enableOutline(float width, uint32 color);
        void disableOutline();
        void setOutlineOnly(bool flag);
        
        void render();
        
        SoraVertex* compile();
        
        size_t size() const;
        
        void setTexture(SoraTextureHandle texture);
        SoraTextureHandle getTexture() const;
        
        SoraShape& operator=(const SoraShape& rhs);
        
        /**
         * If enabled, the shape would try to render itself to a sprite first then render the sprite
         * If the shape does not change dynamically, this would dramaticaly increase the render speed
         * Not that if you enabled this and then changed the shape
         * You need to compile the vertices by yourself to apply the change to sprite
         **/
        void enableRenderToSprite(bool flag);
        SoraSprite* getSprite() const;
        
        /**
         * Set the shape is closed shape or not
         * If it's not closed, then would not repeat to the start vertex when drawing the outline
         * default: yes
         **/
        void setClosed(bool flag);
        
        static SoraVector computeNormal(const SoraVector& p1, const SoraVector& p2);
        
        static SoraShape Line(float x1, float y1, float x2, float y2, float width, uint32 col);
        static SoraShape Box(float x1, float y1, float x2, float y2, uint32 col);
        
        static SoraShape Circle(float x, float y, float r, uint32 col, int32 maxSegment=40);
        static SoraShape Ring(float x, float y, float r, float thickness, uint32 col, int32 maxSegment=40);

        static SoraShape Fan(float x, float y, float r, float a, float b, uint32 col, int32 maxSegment=40);
        static SoraShape Arc(float x, float y, float r, float a, float b, float thickness, uint32 col, int32 maxSegment=40);
                
    private:
        void spriteRender();
        
        typedef std::vector<SoraVertex> VertexArray;
        VertexArray mVertices;
        
        bool mCompiled;
        SoraAutoPtr<SoraVertex> mCompiledVertex;
        
        int32 mRenderMode;
        SoraTextureHandle mTexture;
        
        bool mOutline;
        float mOutlineWidth;
        uint32 mOutlineColor;
        
        bool mOutlineOnly;
        
        bool mRenderToSprite;
        SoraTargetHandle mRenderTarget;
        SoraSprite* mSprite;
        
        bool mClosed;
    };
    
} // namespace sora



#endif
