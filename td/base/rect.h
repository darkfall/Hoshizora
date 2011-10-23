//
//  rect.h
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
#ifndef HTL_rect_h
#define HTL_rect_h

#include "vector2.h"
#include "collidor.h"

namespace atd {
    
    struct Rect: public Collidor {
    
        Vector2 mUpperBound;
        Vector2 mLowerBound;
        
        Rect(float _x1, float _y1, float _x2, float _y2, bool asWH=false):
        mUpperBound(_x1, _y1),
        mLowerBound(asWH?_x1+_x2:_x2, asWH?_y1+_y2:_y2) {
            
        }
        
        Rect() {   
        }
        
        Rect(const Vector2& upper, const Vector2& lower):
        mUpperBound(upper),
        mLowerBound(lower) {
            
        }
        
        Rect(const Vector2& pos, float r) {
            setAsBox(pos.x, pos.y, r);
        }
        
        void set(const Vector2& upper, const Vector2& lower) {
            mUpperBound = upper;
            mLowerBound = lower;
        }
        
        void set(float x1, float y1, float x2, float y2, bool asWH=false) {
            mUpperBound.set(x1, y1);
            mLowerBound.set(asWH?x1+x2:x2, asWH?y1+y2:y2);
        }
        
        void setAsBox(float x, float y, float r) {
            mUpperBound.set(x-r, y-r);
            mLowerBound.set(x+r, y+r);
        }
        
        void encapsulate(float x, float y) {
            if(x<mUpperBound.x) mUpperBound.x = x;
            if(x>mLowerBound.x) mLowerBound.x = x;
            if(y<mUpperBound.y) mUpperBound.y = y;
            if(y>mLowerBound.y) mLowerBound.y = y;
        }
        
        bool testPoint(float x, float y) const {
            return x>=mUpperBound.x && x<=mLowerBound.x && y>=mUpperBound.y && y<=mLowerBound.y;
        }
        
        bool testPoint(const Vector2& pos) const {
            return testPoint(pos.x, pos.y);
        }
        
        bool intersect(const Rect& rhs) const {
            if(fabs(mUpperBound.x + mLowerBound.x - rhs.mUpperBound.x - rhs.mLowerBound.x) < (mLowerBound.x - mUpperBound.x + rhs.mLowerBound.x - rhs.mUpperBound.x)) {
                if(fabs(mUpperBound.y + mLowerBound.y - rhs.mUpperBound.y - rhs.mLowerBound.y) < (mLowerBound.y - mUpperBound.y + rhs.mLowerBound.y - rhs.mUpperBound.y)) {
                    return true;
                }
            }
            return false;
        }
        
        Vector2 getPosition() const {
            return Vector2((mUpperBound.x + mLowerBound.x)/2, (mUpperBound.y+mLowerBound.y)/2);
        }
        
        void setCollisionRadius(float r) {
            Vector2 pos = getPosition();
            setAsBox(pos.x, pos.y, r);
        }
        
        void transform(const Vector2& distance) {
            mUpperBound += distance;
            mLowerBound += distance;
        }
        
        void setPosition(const Vector2& pos) {
            Vector2 center = (mUpperBound + mLowerBound)/2;
            mUpperBound += (pos - center);
            mLowerBound += (pos - center);
        }
        
        void draw() {
            
        }
        
        Collidor* clone() const {
            return new Rect(mUpperBound, mLowerBound);
        }
    };
    
} // namespace atd



#endif