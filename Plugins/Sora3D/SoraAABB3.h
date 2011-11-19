//
//  SoraAABB3.h
//  Sora
//
//  Created by Ruiwei Bu on 11/10/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraAABB3_h
#define Sora_SoraAABB3_h

#include "SoraPlatform.h"
#include "SoraVertex.h"

#include "math/SoraVector3.h"
#include "math/SoraMatrix4.h"

namespace sora {
    
    /**
     * AABB Box in 3 dimension
     **/
    struct SORA_API SoraAABB3 {
        
        SoraVector3 min;
        SoraVector3 max;
        real boundingRadius;
        
        SoraAABB3(): min(SoraVector3()), max(SoraVector3()), boundingRadius(0.f) { }
        SoraAABB3(const SoraVector3& u, const SoraVector3& l): min(u), max(l), boundingRadius((max - min).length() / 2.0) { }
        SoraAABB3(const SoraAABB3& rhs): min(rhs.min), max(rhs.max), boundingRadius(rhs.boundingRadius) { }
        ~SoraAABB3() { }
        
        SoraAABB3& operator=(const SoraAABB3& rhs) {
            this->min = rhs.min;
            this->max = rhs.max;
            this->boundingRadius = rhs.boundingRadius;
        }
        
        static SoraAABB3 AABB3FromVertices(const SoraVertex* vertices, uint32 size) {
            SoraVector3 min;
            SoraVector3 max;
            
            min.x = vertices->x;
            min.y = vertices->y;
            min.z = vertices->z;
            
            max.x = vertices->x;
            max.x = vertices->y;
            max.x = vertices->z;
            
            for(int i=0; i<size; ++i) {
                if(vertices[i].x < min.x) min.x = vertices[i].x;
                if(vertices[i].x > max.x) max.x = vertices[i].x;
                
                if(vertices[i].y < min.y) min.y = vertices[i].y;
                if(vertices[i].y > max.y) max.y = vertices[i].y;
                
                if(vertices[i].z < min.z) min.z = vertices[i].z;
                if(vertices[i].z > max.z) max.z = vertices[i].z;
            }
            
            return SoraAABB3(min, max);
        }
                
        void set(const SoraVector3& min, const SoraVector3& max);
        void set(const SoraVector3& min, const SoraVector3& max, real boundingRadius);
        
        real getBoudingRadius() const;
        SoraVector3 getCenter() const;
        
        void transform(const SoraMatrix4& mat);
        
        const SoraVector3& getMin() const;
        const SoraVector3& getMax() const;
        
        SoraVector3& getMin();
        SoraVector3& getMax();
        
        real getWidth() const;
        real getHeight() const;
        real getDepth() const;
        
        void scale(float scale);
    };
    
    inline void SoraAABB3::set(const SoraVector3& min, const SoraVector3& max) {
        this->min = min;
        this->max = max;
        this->boundingRadius = (this->max - this->min).length() / 2.0f;
    }
    
    inline void SoraAABB3::set(const SoraVector3& min, const SoraVector3& max, real boundingR) {
        this->min = min;
        this->max = max;
        this->boundingRadius = boundingR;
    }
    
    inline real SoraAABB3::getBoudingRadius() const {
        return this->boundingRadius;
    }
    
    inline SoraVector3 SoraAABB3::getCenter() const {
        return (this->max - this->min) / 2.0f;
    }
    
    inline void SoraAABB3::transform(const SoraMatrix4& mat) {
        SoraVector4 box[8];
        SoraVector4 newMin, newMax;
        
        box[0].x = this->min.x;
        box[0].y = this->min.y;
        box[0].z = this->min.z;
        
        box[1].x = this->min.x;
        box[1].y = this->min.y;
        box[1].z = this->max.z;
        
        box[2].x = this->min.x;
        box[2].y = this->max.y;
        box[2].z = this->min.z;
        
        box[3].x = this->min.x;
        box[3].y = this->max.y;
        box[3].z = this->max.z;
        
        box[4].x = this->max.x;
        box[4].y = this->min.y;
        box[4].z = this->min.z;
        
        box[5].x = this->max.x;
        box[5].y = this->min.y;
        box[5].z = this->max.z;
        
        box[6].x = this->max.x;
        box[6].y = this->max.y;
        box[6].z = this->min.z;
        
        box[7].x = this->max.x;
        box[7].y = this->max.y;
        box[7].z = this->max.z;
        
        newMin = mat * box[0];
        newMax = newMin;
        
        for(int i=1; i<8; ++i) {
            box[i] = mat * box[i];
            
            newMin.x = SORA_MIN(box[i].x, newMin.x);
            newMin.y = SORA_MIN(box[i].x, newMin.y);
            newMin.z = SORA_MIN(box[i].x, newMin.z);
            
            newMax.x = SORA_MAX(box[i].x, newMax.x);
            newMax.y = SORA_MAX(box[i].x, newMax.y);
            newMax.z = SORA_MAX(box[i].x, newMax.z);
        }
        
        set(min, max);
    }
    
    inline const SoraVector3& SoraAABB3::getMin() const {
        return min;
    }
    
    inline const SoraVector3& SoraAABB3::getMax() const {
        return max;
    }
    
    inline SoraVector3& SoraAABB3::getMin() {
        return min;
    }
    
    inline SoraVector3& SoraAABB3::getMax() {
        return max; 
    }
    
    inline real SoraAABB3::getWidth() const {
        return SORA_ABS(max.x - min.x);
    }
    
    inline real SoraAABB3::getHeight() const {
        return SORA_ABS(max.y - min.y);
    }
    
    inline real SoraAABB3::getDepth() const {
        return SORA_ABS(max.z - min.z);
    }
    
    inline void SoraAABB3::scale(float scale) {
        SoraVector3 center = getCenter();
        SoraVector3 dist = (max - center) / 2.0 * scale;
        
        set(center - dist, center + dist);
    }
    
} // namespace sora


#endif
