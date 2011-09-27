/*
 *  SoraRect.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/27/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */
#ifndef SORA_RECT_H
#define SORA_RECT_H

#include "SoraPlatform.h"
#include "SoraVector.h"

namespace sora {
    
    struct SORA_API SoraRect {
        
        float x1, y1;
        float x2, y2;
        
        SoraRect(float _x1, float _y1, float _x2, float _y2, bool asWH=false):
        x1(_x1), y1(_y1),
        x2(_x2), y2(_y2) {
            
        }
        
        SoraRect() {   
        }
        
        SoraRect(const SoraVector& upper, const SoraVector& lower):
        x1(upper.x), y1(upper.y),
        x2(lower.x), y2(lower.y) {
            
        }
        
        SoraRect(const SoraVector& pos, float r) {
            setAsBox(pos.x, pos.y, r);
        }
        
        void set(const SoraVector& upper, const SoraVector& lower) {
            x1 = upper.x;
            y1 = upper.y;
            x2 = upper.x;
            y2 = upper.y;
        }
        
        void set(float _x1, float _y1, float _x2, float _y2, bool asWH=false) {
            x1 = _x1; y1 = _y1;
            x2 = asWH ? _x1 + _x2 : _x2;
            y2 = asWH ? _y1 + _y2 : _y2;
        }
        
        void setAsBox(float x, float y, float r) {
            x1 = x - r;
            y1 = y - r;
            x2 = x + r;
            y2 = y + r;
        }
        
        void encapsulate(float x, float y) {
            if(x<x1) x1 = x;
            if(x>x2) x2 = x;
            if(y<y1) y1 = y;
            if(y>y2) y2 = y;
        }
        
        bool testPoint(float x, float y) const {
            return x>=x1 && x<=x2 && y>=y1 && y<=y2;
        }
        
        bool testPoint(const SoraVector& pos) const {
            return testPoint(pos.x, pos.y);
        }
        
        bool intersect(const SoraRect& rhs) const {
            if(fabs(x1 + x2 - rhs.x1 - rhs.x2) < (x2 - x1 + rhs.x2 - rhs.x1)) {
                if(fabs(y1 + y2 - rhs.y1 - rhs.y2) < (y2 - y1 + rhs.y2 - rhs.y1)) {
                    return true;
                }
            }
            return false;
        }
        
        SoraVector getPosition() const {
            return SoraVector((x1 + x2)/2, (y1+y2)/2);
        }
    };
}



#endif