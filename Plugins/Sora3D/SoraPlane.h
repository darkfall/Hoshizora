//
//  SoraPlane.h
//  Sora
//
//  Created by Robert Bu on 11/10/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPlane_h
#define Sora_SoraPlane_h

#include "SoraPlatform.h"
#include "math/SoraVector4.h"

namespace sora {
    
    class SoraPlane {
    public:
        SoraPlane() { }
        
        SoraPlane(const SoraPlane& rhs):
        mPlane(rhs.mPlane) { }
        
        explicit SoraPlane(const SoraVector4& vec):
        mPlane(vec) { }
        
        SoraPlane(real x, real y, real z, real w) {
            mPlane.set(x, y, z, w);
        }
          
        SoraPlane& operator=(const SoraPlane& rhs) {
            if(this != &rhs) {
                this->mPlane = rhs.mPlane;
            }
            return *this;
        }
        
        bool operator==(const SoraPlane& rhs) const {
            return this->mPlane == rhs.mPlane;
        }
        
        void set(real x, real y, real z, real w) {
            this->mPlane.set(x, y, z, w);
        }
        
        SoraPlane const operator-() const {
            return SoraPlane(-this->mPlane);
        }
        
        SoraVector3 normal() const {
            return SoraVector3(this->mPlane.x, this->mPlane.y, this->mPlane.z);
        }
        
        real getDistance(const SoraVector3 point) {
            real num = this->mPlane * SoraVector4(point);
            real dom = InvSqrt(this->mPlane.x * this->mPlane.x + 
                               this->mPlane.y * this->mPlane.y + 
                               this->mPlane.z * this->mPlane.z);
            return num/dom;
        }
         
        
    private:
        SoraVector4 mPlane;
    };
    
    
} // namespace sora



#endif
