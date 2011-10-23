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
            this->x1 = upper.x;
            this->y1 = upper.y;
            this->x2 = upper.x;
            this->y2 = upper.y;
        }
        
        void set(float _x1, float _y1, float _x2, float _y2, bool asWH=false) {
            this->x1 = _x1; 
            this->y1 = _y1;
            this->x2 = asWH ? _x1 + _x2 : _x2;
            this->y2 = asWH ? _y1 + _y2 : _y2;
        }
        
        void setAsBox(float x, float y, float r) {
            this->x1 = x - r;
            this->y1 = y - r;
            this->x2 = x + r;
            this->y2 = y + r;
        }
        
        void encapsulate(float x, float y) {
            if(x<this->x1) this->x1 = x;
            if(x>this->x2) this->x2 = x;
            if(y<this->y1) this->y1 = y;
            if(y>this->y2) this->y2 = y;
        }
        
        bool testPoint(float x, float y) const {
            return x>=x1 && x<=x2 && y>=y1 && y<=y2;
        }
        
        bool testPoint(const SoraVector& pos) const {
            return testPoint(pos.x, pos.y);
        }
        
        bool intersect(const SoraRect& rhs) const {
            if(fabs(this->x1 + this->x2 - rhs.x1 - rhs.x2) < (this->x2 - this->x1 + rhs.x2 - rhs.x1)) {
                if(fabs(this->y1 + this->y2 - rhs.y1 - rhs.y2) < (this->y2 - this->y1 + rhs.y2 - rhs.y1)) {
                    return true;
                }
            }
            return false;
        }
        
        SoraVector getPosition() const {
            return SoraVector((this->x1 + this->x2)/2,
                              (this->y1+this->y2)/2);
        }
    };
}



#endif