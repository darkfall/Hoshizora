//
//  SoraCustomShapeSprite.h
//  Sora
//
//  Created by Ruiwei Bu on 7/1/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraCustomShapeSprite_h
#define Sora_SoraCustomShapeSprite_h

#include "SoraPlatform.h"
#include "SoraSprite.h"

namespace sora {
    
    /**
     *  Helper implemention for render sprite with vertices
     **/
    
    class SoraCustomShapeSprite {
    public:
        SoraCustomShapeSprite();
        SoraCustomShapeSprite(SoraSprite* sprite, int32 renderMode);
        ~SoraCustomShapeSprite();
        
        void setSprite(SoraSprite* mSprite);
        SoraSprite* getSprite() const;
        
        uint32 update(float32 dt);
        void render();
        void render(int32 renderMode);
        
        /*
            SORA_LINE,
            SORA_TRIANGLE,
            SORA_TRIANGLE_FAN,
            SORA_TRIANGLE_STRIP,
            SORA_QUAD
         */
        void setRenderMode(int32 renderMode);
        int32 getRenderMode() const;
        
        int32 addVertex(float32 screenposx, float32 screenposy, float32 texposx, float32 texposy, float32 screenz=0.f); 
        int32 addScreenMappingVertex(float32 x, float32 y, float32 screenz=0.f);
        int32 addVertex(const SoraVertex& vertex);
        
        void delVertx(int32 vid);
        void clearVertices();
        
        void setVertexColor(int32 vid, uint32 color);
        uint32 getVertexColor(int32 vid) const;
        
        void setVertexZ(int32 vid, float32 z);
        float32 getVertexZ(int32 vid) const;
        
        void setVertexPos(int32 vid, float32 sposx, float32 sposy);
        void getVertexPos(int32 vid, float32* sposx, float32* sposy);
        
        void setVertexTexturePos(int32 vid, float32 tposx, float32 tposy);
        void getVertexTexturePos(int32 vid, float32* tposx, float32 *tposy);
        
        void exchangeVertex(int32 vid1, int32 vid2);
        
        int32 getVertexCount() const;
        
        bool saveVertciesToFile(const SoraWString& file);
        bool loadVerticesFromFile(const SoraWString& file);
        
    private:
        inline void buildVertexBuffer();
        
        SoraSprite* mSprite;
        int32 mNextVertexId;
        int32 mRenderMode;
        bool mVertexBufferBuilt;
        
        typedef std::map<int32, SoraVertex> VERTEX_MAP;
        VERTEX_MAP mVertexMap;
        
        typedef std::map<int32, int32> VERTEX_INDEX_MAP;
        VERTEX_INDEX_MAP mVertexIndexes;
        
        /* for render */
        SoraVertex* mVertexBuffer;
    };

} // namespace sora

#endif
