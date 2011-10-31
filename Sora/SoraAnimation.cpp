//
//  SoraAnimation.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraAnimation.h"
#include "SoraSprite.h"
#include "SoraConfig.h"

namespace sora {
    
    SoraAnimation::SoraAnimation() {
        
    }
    
    SoraAnimation::SoraAnimation(const StringType& configPath) {
        
    }
    
    SoraAnimation::SoraAnimation(void* pdata, uint32 size) {
        
    }
    
    void SoraAnimation::setFromFile(const StringType& configPath) {
   
    }
    
    void SoraAnimation::setFromMemory(void* pdata, uint32 size) {
        
    }
    
    void SoraAnimation::render() {

    }
    
    int32 SoraAnimation::update(float dt) {
        
    }
    
    void SoraAnimation::play(const StringType& name) {
        
    }
    
    void SoraAnimation::setDefault(const StringType& name) {
        int32 index = getIndexByName(name);
        if(index != -1) {
            mDefaultAnime = name;
        }
    }
    
    int32 SoraAnimation::getIndexByName(const StringType& name) {
        AnimationArray::iterator it = mAnimations.begin();
        AnimationArray::iterator end = mAnimations.end();
        int32 index = 0;
        for(; it != end; ++it) {
            if(it->mName == name) {
                return index;
            }
            ++index;
        }
        return -1;
    }
} // namespace sora
