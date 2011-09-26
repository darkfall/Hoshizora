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
    
    /**
     *   A base custom shape class that can be positioned,
     *   rendered, output it's vertex data for sprite to use
     *   and adapt a texture to render
     **/
    
    class SoraShape: public SoraObject {
    public:
        SoraShape(SoraTextureHandle texture=0, int32 mode=SORA_TRIANGLES_FAN);
        
        /**
         * Add a point
         **/
        void addPoint(float x, float y, uint32 color, float tx=0.f, float ty=0.f);
        void addPoint(const SoraVector& pos, uint32 color, float tx=0.f, float ty=0.f);
        
        void setPoint(uint32 index, float x, float y, uint32 color, float tx=0.f, float ty=0.f);
        SoraVertex getPoint(uint32 index);
        
        void render();
        
        SoraVertex* compile();
        
        size_t size() const;
        
        void setTexture(SoraTextureHandle texture);
        SoraTextureHandle getTexture() const;
        
        static SoraVector computeNormal(const SoraVector& p1, const SoraVector& p2);
        
        static SoraShape Line(float x1, float y1, float x2, float y2, uint32 col);
        static SoraShape Circle(float x, float y, float r, uint32 col);
        
    private:
        typedef std::vector<SoraVertex> VertexArray;
        VertexArray mVertices;
        
        bool mCompiled;
        SoraAutoPtr<SoraVertex> mCompiledVertex;
        
        int32 mRenderMode;
        SoraTextureHandle mTexture;
    };
    
} // namespace sora



#endif
