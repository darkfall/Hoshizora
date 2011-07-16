/*
 *  SoraPlatformerGeomtry.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_PLATFORMER_GEOMETRY_H_
#define SORA_PLATFORMER_GEOMETRY_H_

#include "SoraGeometry.h"
#include "SoraBox2D/SoraPhysicalObject.h"
#include "SoraGenericObjects/SoraCustomShapeSprite.h"

namespace sora {
	
	class SoraPlatformerGeometry: public SoraGeometry {
	public:
		SoraPlatformerGeometry(HSORATEXTURE geoTex, float32 x=0.f, float32 y=0.f, float32 width=0.f, float32 height=0.f);
		~SoraPlatformerGeometry();
		
		int32 addVertex(float32 x, float32 y);
        void delVertex(int32 vid);
        void setVertexMode(int32 mode);
        void clearVertices();
        
        void create();
		
		void render();
		uint32 update(float32 dt);
        
        SoraStream saveToStream();
        bool loadFromData(void* data, ulong32 size);
        
    private:
        SoraSprite* mSprite;
        SoraPhysicalObject* mGeometry;
        SoraCustomShapeSprite mRenderer;
        
        bool mBuilt;
	};
	
	
} // namespace sora


#endif // SORA_PLATFORMER_GEOMETRY_H_