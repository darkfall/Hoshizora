//
//  SoraSphere.h
//  Sora
//
//  Created by Ruiwei Bu on 11/10/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraSphere_h
#define Sora_SoraSphere_h

#include "SoraPlatform.h"
#include "SoraVertex.h"
#include "math/SoraVector3.h"

namespace sora {
    
    class SORA_API SoraSphere {
    public:
        SoraSphere():
        mRadius(0.f),
        mCenter(SoraVector3()) { }
        
        SoraSphere(const SoraVector3& pos, real radius):
        mRadius(radius),
        mCenter(pos) { }
        
        explicit SoraSphere(const SoraVector4& vec) {
            this->mCenter.set(vec.x, vec.y, vec.z);
            this->mRadius = vec.w;
        }
        
        const real& radius() const {
            return this->mRadius;
        }
        
        real& radius() {
            return this->mRadius;
        }
        
        const SoraVector3& center() const {
            return this->mCenter;
        }
        
        SoraVector3& center() {
            return this->mCenter;
        }
        
        bool isEmpty() {
            return FloatEqual(this->mRadius, 0.f);
        }
        
        SoraSphere& operator+=(const SoraVector3& trans) {
            this->mCenter += trans;
            return *this;
        }
        
        SoraSphere& operator-=(const SoraVector3& trans) {
            this->mCenter -= trans;
            return *this;
        }
        
        SoraSphere& operator*=(real s) {
            this->mRadius *= s;
            return *this;
        }
        
        SoraSphere& operator/=(real s) {
            sora_assert(s != 0.0f);
            
            this->mRadius /= s;
            return *this;
        }
        
        SoraSphere& operator=(const SoraSphere& rhs) {
            if(this != &rhs) {
                this->mRadius = rhs.mRadius;
                this->mCenter = rhs.mCenter;
            }
            return *this;
        }
        
        SoraVector4 asVector4() const {
            return SoraVector4(mCenter.x, mCenter.y, mCenter.z, mRadius);
        }
        
        bool operator==(const SoraSphere& rhs) const {
            return this->mCenter == rhs.mCenter && this->mRadius == rhs.mRadius;
        }
        
        bool vecInBound(const SoraVector3& point) {
            real distantce = (point - this->mCenter).sqrLength();
            
            return distantce <= mRadius * mRadius;
        }
        
        bool testPoint(const SoraVector3& point) {
            return vecInBound(point);
        }
        
    private:
        
        real mRadius;
        SoraVector3 mCenter;
    };
    
    
} // naemspace sora



#endif
