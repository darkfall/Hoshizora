//
//  SoraSpritePool.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraSpritePool.h"
#include "SoraSprite.h"
#include "SoraCommon.h"

namespace sora {
    
    void SoraSpritePool::addSprite(const StringType& tag, const StringType& spritePath) {
        SoraSprite* sprite = SoraSprite::LoadFromFile(spritePath);
        
        if(sprite) {
            mSprites.insert(std::make_pair(tag, sprite));
        }
    }
    
    void SoraSpritePool::addSpriteFromMemory(const StringType& tag, uint32* data, ulong32 size) {
        SoraSprite* sprite = SoraSprite::LoadFromMemory(data, size);
        if(sprite) {
            mSprites.insert(std::make_pair(tag, sprite));
        }
    }
    
    void SoraSpritePool::removeSprite(const StringType& tag) {
        SpriteMap::iterator it = mSprites.find(tag);
        
        if(it != mSprites.end()) {
            delete it->second;
            mSprites.erase(it);
        }
    }
    
    SoraSprite* SoraSpritePool::getSprite(const StringType& tag) const {
        SpriteMap::const_iterator it = mSprites.find(tag);
        
        if(it != mSprites.end()) 
            return it->second;
        
        return 0;
    }
    
    void SoraSpritePool::clear() {
        for_each(mSprites.begin(), mSprites.end(), DeleteSTLPairPtr());
        mSprites.clear();
    }
    
} // namespace sora