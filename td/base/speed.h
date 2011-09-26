//
//  speed.h
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
#ifndef HTL_speed_h
#define HTL_speed_h

#include "vector2.h"

namespace atd {
    
    struct Speed {
      
        float mSpeed;
        float mDirection;
        
        Speed():
        mSpeed(0.f),
        mDirection(0.f) {
            
        }
        
        Speed(float speed, float dir=0.f):
        mSpeed(speed),
        mDirection(dir) {
            
        }
        
        Vector2 getSpeedVec() const {
            return Vector2(mSpeed * cosf(mDirection), mSpeed * sinf(mDirection));
        }
        
        void setSpeed(float speed) {
            mSpeed = speed;
        }
        
        void setDirection(float dir) {
            mDirection = dir;
        }
        
        float getSpeed() const {
            return mSpeed;
        }
        
        float getDirection() const {
            return mDirection;
        }
        
        operator float() {
            return mSpeed;
        }
    };
    
    inline Vector2 operator+(const Vector2& lhs, const Speed& rhs) {
        return lhs + rhs.getSpeedVec();
    }
    
    
} // namespace atd



#endif