//
//  SoraAnimation.h
//  Sora
//
//  Created by Ruiwei Bu on 9/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraAnimation_h
#define Sora_SoraAnimation_h

#include "SoraPlatform.h"
#include "SoraString.h"
#include "SoraRect.h"
#include "SoraObject.h"
#include "SoraSprite.h"

#include <map>

namespace sora {
        
    class SORA_API SoraAnimation: public SoraObject {
    public:
        SoraAnimation();
        SoraAnimation(const StringType& configPath);
        SoraAnimation(void* data, uint32 size);
        
        void setFromFile(const StringType& configPath);
        void setFromMemory(void* data, uint32 size);
        
        void render();
        int32 update(float dt);
        
        struct AnimationNode {
            AnimationNode():
            mCol(0),
            mCount(0),
            mName(StringType()),
            mTexWidth(0),
            mTexHeight(0) {
                
            }
            AnimationNode(int32 col, int32 count, const StringType& name):
            mCol(col),
            mCount(count),
            mName(name),
            mTexWidth(0),
            mTexHeight(0)  {
                
            }
            
            int32 mCol;
            int32 mCount;
            
            StringType mName;
            
            int32 mTexWidth;
            int32 mTexHeight;
            
            inline void get(int32 index, SoraSprite* sprite) {
                if(index < 0)
                    index = mCount + index;
                if(index > mCount)
                    index = 0;
                
                int32 row = index / mCol;
                int32 col = index % mCol;
                sprite->setTextureRect((col-1)*mTexWidth, (row-1)*mTexHeight, mTexWidth, mTexHeight);
            }
        };
        
        typedef std::vector<AnimationNode> AnimationArray;
        const AnimationArray& getAnimations() const;
        
        void play(const StringType& name=StringType());
        void setDefault(const StringType& name);
        
    private:
        int32 getIndexByName(const StringType& name);
        
        SoraSprite* mSprite;
        AnimationArray mAnimations;
        
        StringType mDefaultAnime;
        
        int32 mCurrentIndex;
        int32 mCurrentAnimation;
    };
    
} // namespace sora



#endif
