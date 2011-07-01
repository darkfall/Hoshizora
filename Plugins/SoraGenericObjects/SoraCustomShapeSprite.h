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
    
    class SoraCustomShapeSpriteAdapter {
    public:
        SoraCustomShapeSpriteAdapter(SoraSprite* sprite);
        
        void setSprite(SoraSprite* mSprite);
        SoraSprite* getSprite() const;
        
        void render();
        
        int32 addVertex(float32 screenpox, float32 screenposy, float32 texposx, float32 texposy, float32 screenz=0.f); 
        int32 addScreenMappingVertex(float32 x, float32 y, float32 screenz=0.f);
        int32 addVertex(const SoraVertex& vertex);
        
        void delVertx(int32 vid);
        
        void setVertexColor(int32 vid, uint32 color);
        uint32 getVertexColor(int32 vid) const;
        
        void setVertexZ(int32 vid, float32 z);
        float32 getVertexZ(int32 vid) const;
        
        void setVertexPos(int32 vid, float32 spox, float32 spoy);
        void getVertexPos(int32 vid, float32* spox, float32* spoy);
        
        void setVertexTexturePos(int32 vid, float32 tpox, float32 tpoy);
        void getVertexTexturePos(int32 vid, float32* tpox, float32 *tpoy);
        
        void exchangeVertex(int32 vid1, int32 vid2);
        
    private:
        SoraSprite* mSprite;
        
        typedef struct {
            int32 mVertexID;
            SoraVertex mVertexData;
        } VertexNode;
        
        typedef std::vector<VertexNode> VERTEX_CONT;
        VERTEX_CONT mVertexList;
        
        /* for render */
        SoraVertex* mVertexBuffer;
    };

} // namespace sora

#endif
