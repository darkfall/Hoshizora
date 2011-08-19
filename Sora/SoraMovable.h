//
//  SoraMovable.h
//  Sora
//
//  Created by Ruiwei Bu on 8/18/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraMovable_h
#define Sora_SoraMovable_h

#include "SoraMath.h"

namespace sora {
    
    // base class for all movable things
    
    class SoraMovable {
    public:
        SoraMovable():
        mRotation(0.f), 
        mPosition(SoraVector3()) {
            
        }
        
        virtual void setPosition(const SoraVector3& position) {
            mPosition = position;
        }
        
        virtual SoraVector3 getPosition() const {
            return mPosition;
        }
        
        inline void setPosition(float32 x, float32 y) {
            setPosition(SoraVector3(x, y, mPosition.z));
        }
        
        inline float32 getPositionX() {
            return getPosition().x;
        }
        
        inline float32 getPositionY() {
            return getPosition().y;
        }
        
        inline float32 getPositionZ() {
            return getPosition().z;
        }
        
        virtual void setRotation(float32 r) {
            mRotation = r;
        }
        
        virtual float32 getRotation() const {
            return mRotation;
        }
        
        inline void translate(const SoraVector3& delta) {
            setPosition(mPosition + delta);
        }
        
    private:
        float32 mRotation;
        // SoraQuaternion mRotation;
        
        SoraVector3 mPosition;
    };
    
    
} // namespace sora



#endif
