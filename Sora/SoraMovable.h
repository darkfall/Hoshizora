//
//  SoraMovable.h
//  Sora
//
//  Created by Robert Bu on 8/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraMovable_h
#define Sora_SoraMovable_h

#include "SoraMath.h"

namespace sora {
    
    // base class for all movable things
    
    struct SORA_API SoraMovable {
        SoraMovable():
        x(0.f),
        y(0.f),
        z(0.f) {
            
        }
        
        explicit SoraMovable(const SoraVector3& pos):
        x(pos.x),
        y(pos.y),
        z(pos.z) {
        
        }
        
        SoraMovable(float _x, float _y, float _z):
        x(_x),
        y(_y),
        z(_z) {
            
        }
        
        float x;
        float y;
        float z;
        
        void setPosition(const SoraVector3& pos) {
            x = pos.x;
            y = pos.y;
            z = pos.z;
        }
        
        void setPosition(float x, float y) {
            this->x = x;
            this->y = y;
        }
        
        float getPositionX() const {
            return x;
        }
        
        float getPositionY() const {
            return y;
        }
        
        float getPositionZ() const {
            return z;
        }
        
        void translate(const SoraVector3& v, float t) {
            x += v.x * t;
            y += v.y * t;
            z += v.z * t;
        }
        
        void translate(float vx, float vy, float t) {
            x += vx * t;
            y += vy * t;
        }
        
    };
    
    
} // namespace sora



#endif
