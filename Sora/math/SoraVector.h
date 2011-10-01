/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
** s
** SoraVector helper class
*/


#ifndef SORA_VECTOR_H_
#define SORA_VECTOR_H_

#include "SoraPlatform.h"
#include <cmath>
    
namespace sora {
    
    inline float InvSqrt(float x) {
        union
        {
            int intPart;
            float floatPart;
        } convertor;
        
        convertor.floatPart = x;
        convertor.intPart = 0x5f3759df - (convertor.intPart >> 1);
        return convertor.floatPart*(1.5f - 0.4999f*x*convertor.floatPart*convertor.floatPart);
    }
    
    inline bool FloatEqual(float l, float r) {
        return fabs(r-l) < 0.00000001f;
    }
    
    struct SORA_API SoraVector {
        float x;
        float y;
        
        SoraVector(float _x, float _y):
        x(_x),
        y(_y) { }
        
        SoraVector():
        x(0.f),
        y(0.f) {
            
        }
        
        void set(float _x, float _y) {
            x = _x;
            y = _y;
        }
        
        SoraVector operator -() const { return SoraVector(-x, -y); }
        SoraVector operator -(const SoraVector& rhs) const { return SoraVector(x-rhs.x, y-rhs.y); }
        SoraVector operator +(const SoraVector& rhs) const { return SoraVector(x+rhs.x, y+rhs.y); }
        
        SoraVector& operator+=(const SoraVector& rhs) { x += rhs.x; y += rhs.y; return *this; }
        SoraVector& operator-=(const SoraVector& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
        
        bool operator ==(const SoraVector& rhs) const { return FloatEqual(x, rhs.x) && FloatEqual(y, rhs.y); }
        bool operator !=(const SoraVector& rhs) const { return x!=rhs.x && y!=rhs.y; }
        
        SoraVector operator/(float s) const { return SoraVector(x/s, y/s); }
        SoraVector operator*(float s) const { return SoraVector(x*s, y*s); }
        SoraVector& operator*=(float s) { x *= s; y *= s; return *this; }
        
        float dot(const SoraVector& rhs) const { return x*rhs.x + y*rhs.y; }
        float length() const { return sqrtf(dot(*this)); }
        float lengthsq() const { return dot(*this); }
        float angle(const SoraVector* rhs = 0) const {
            if(rhs) {
                SoraVector s = *this, t = *rhs;
                s.normalize();
                t.normalize();
                return acosf(s.dot(t));
            } else
                return atan2f(y, x);
        }
        
        void clamp(const float max) {
            if(length() > max) {
                normalize();
                x *= max;
                y *= max;
            }
        }
        
        SoraVector& normalize() {
            float rc = InvSqrt(dot(*this));
            x *= rc;
            y *= rc;
            return *this;
        }
        SoraVector& rotate(float a) {
            SoraVector v;
            v.x = x * cosf(a) - y * sinf(a);
            v.y = x * sinf(a) + y * cosf(a);
            x = v.x; y = v.y;
            return *this;
        }
        
        SoraVector normal(const SoraVector& rhs){
            SoraVector normal;
            
            normal.x = this->y - rhs.y;
            normal.y = rhs.x - this->x;
            float len = sqrt(normal.x * normal.x + normal.y * normal.y);
            if(len == 0.f) {
                return SoraVector();
            }
            
            normal.x /= len;
            normal.y /= len;
            
            return normal;
        }
    };
    
    inline SoraVector operator *(const float s, const SoraVector& v) {
        return v*s;
    }
    
    inline float operator^(const SoraVector& lhs, const SoraVector& rhs) {
        return lhs.angle(&rhs);
    }
    
    inline float operator%(const SoraVector& lhs, const SoraVector& rhs) {
        return lhs.dot(rhs);
    }

    
}

#endif
