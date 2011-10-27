//
//  SoraVector4.h
//  Sora
//
//  Created by Ruiwei Bu on 10/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraVector4_h
#define Sora_SoraVector4_h

#include "SoraPlatform.h"
#include "SoraVector3.h"

namespace sora {
    
    class SORA_API SoraVector4 {
    public:
        float x, y, z, w;
        
        SoraVector4():
        x( 0 ), 
        y( 0 ), 
        z( 0 ), 
        w( 0 ) { }
        
        explicit SoraVector4(const float x, const float y, const float z, const float w):
		x( x ), 
        y( y ), 
        z( z ), 
        w( w ) { }
        
        explicit SoraVector4(const SoraVector3& v):
        x( v.x ), 
        y( v.y ), 
        z( v.z ),
        w( 1.0f ) { }
        
        void set(float _x, float _y, float _z, float _w) {
            this->x = _x;
            this->y = _y;
            this->z = _z;
            this->w = _w;
        }
        
        SoraVector4 operator+(const SoraVector4 &v) const {
            return SoraVector4(this->x + v.x, 
                               this->y + v.y, 
                               this->z + v.z, 
                               this->w + v.w);
        }
        
        SoraVector4 operator*(const float f) const {
            return SoraVector4(this->x * f, 
                               this->y * f, 
                               this->z * f, 
                               this->w * f);
        }
    };
    
} // namespace sora


#endif
