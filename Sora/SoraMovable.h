//
//  SoraMovable.h
//  Sora
//
//  Created by Robert Bu on 8/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
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
        
        inline void setPosition(float x, float y) {
            setPosition(SoraVector3(x, y, mPosition.z));
        }
        
        inline float getPositionX() {
            return getPosition().x;
        }
        
        inline float getPositionY() {
            return getPosition().y;
        }
        
        inline float getPositionZ() {
            return getPosition().z;
        }
        
        virtual void setRotation(float r) {
            mRotation = r;
        }
        
        virtual float getRotation() const {
            return mRotation;
        }
        
        inline void translate(const SoraVector3& delta) {
            setPosition(mPosition + delta);
        }
        
    private:
        float mRotation;
        // SoraQuaternion mRotation;
        
        SoraVector3 mPosition;
    };
    
    
} // namespace sora



#endif
