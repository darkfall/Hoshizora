//
//  SoraPhysicShape.h
//  Sora
//
//  Created by Ruiwei Bu on 10/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPhysicShape_h
#define Sora_SoraPhysicShape_h

#include "SoraPlatform.h"
#include "SoraMath.h"
#include "SoraVertex.h"

namespace sora {
    
    class SoraPhysicShape {
    public:
        enum ShapeType {
            ShapeEdge,
            ShapeBox,
            ShapeCircle,
            ShapePolygon, 
        };
        
        virtual void setAsEdge(float x1, float y1, float x2, float y2) = 0;
        virtual void setAsBox(float w, float h, float centerX, float centerY, float rotation) = 0;
        virtual void setAsCircle(float r) = 0;
        virtual void setAsPolygon(SoraVertex* vertices, uint32 size) = 0;
    };
    
} // namespace sora



#endif
