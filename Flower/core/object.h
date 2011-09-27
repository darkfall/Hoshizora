//
//  object.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_object_h
#define Sora_iPhone_object_h

#include "SoraObject.h"
#include "SoraVector.h"
#include "SoraSprite.h"

namespace flower {
    
    class Object: public sora::SoraObject {
    public:
        Object(float weight);
        
        uint32 update(float dt);
        void render();
        
        void applyForce(const sora::SoraVector& force);
        
        void setSprite(sora::SoraSprite* sprite);
        
        float getWidth() const;
        float getHeight() const;
        
        virtual void die() = 0;
        
        bool isDied() const;
        
        static void setSpeedThreshold(float threshold);
        static float getSpeedThreshold();
        
    protected:
        sora::SoraSprite* mTexture;
        
        float mWeight;
        float mRotation;
        
        bool mIsDied;

        sora::SoraVector mSpeed;
        
        static float mSpeedThreshold;
    };
    
}


#endif
