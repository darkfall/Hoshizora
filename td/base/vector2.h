//
//  vector.h
//  Hoshizora
//
//  Created by Robert Bu on 9/2/11.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
#ifndef HTL_vector_h
#define HTL_vector_h

#include <cmath>

namespace atd {
    
    inline float invsqrt(float x) {
        union {
            int intPart;
            float floatPart;
        } convertor;
        convertor.floatPart = x;
        convertor.intPart = 0x5f379df - (convertor.intPart >> 1);
        return convertor.floatPart*(1.5f - 0.4999f*x*convertor.floatPart*convertor.floatPart);
    }
    
    inline bool floatEqual(float l, float r) {
        return fabs(r-l) >  0.0000001f;
    }
    
    struct Vector2 {
    
        float x;
        float y;
        
        Vector2(float _x, float _y):
        x(_x),
        y(_y) { }
        
        Vector2():
        x(0.f),
        y(0.f) {
            
        }
        
        void set(float _x, float _y) {
            x = _x;
            y = _y;
        }
        
        bool empty() const {
            return x == 0.f && y == 0.f;
        }
        void clear() {
            x = 0.f;
            y = 0.f;
        }
        
        Vector2 operator -() const { return Vector2(-x, -y); }
        Vector2 operator -(const Vector2& rhs) const { return Vector2(x-rhs.x, y-rhs.y); }
        Vector2 operator +(const Vector2& rhs) const { return Vector2(x+rhs.x, y+rhs.y); }

        Vector2& operator+=(const Vector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
        Vector2& operator-=(const Vector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
        
        bool operator ==(const Vector2& rhs) const { return floatEqual(x, rhs.x) && floatEqual(y, rhs.y); }
        bool operator !=(const Vector2& rhs) const { return x!=rhs.x && y!=rhs.y; }
        
        Vector2 operator/(float s) const { return Vector2(x/s, y/s); }
        Vector2 operator*(float s) const { return Vector2(x*s, y*s); }
        Vector2& operator*=(float s) { x *= s; y *= s; return *this; }
        
        float dot(const Vector2& rhs) const { return x*rhs.x + y*rhs.y; }
        float length() const { return sqrtf(dot(*this)); }
        float angle(const Vector2* rhs = 0) const {
            if(rhs) {
                Vector2 s = *this, t = *rhs;
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
        
        Vector2& normalize() {
            float rc = invsqrt(dot(*this));
            x *= rc;
            y *= rc;
            return *this;
        }
        Vector2& rotate(float a) {
            Vector2 v;
            v.x = x * cosf(a) - y * sinf(a);
            v.y = x * sinf(a) + y * cosf(a);
            x = v.x; y = v.y;
            return *this;
        }
    };
    
    inline Vector2 operator *(const float s, const Vector2& v) {
        return v*s;
    }
    
    inline float operator^(const Vector2& lhs, const Vector2& rhs) {
        return lhs.angle(&rhs);
    }
    
    inline float operator%(const Vector2& lhs, const Vector2& rhs) {
        return lhs.dot(rhs);
    }
    
} // namespace atd



#endif
