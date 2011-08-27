//
//  SoraFastRenderer.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraFastRenderer.h"
#include "SoraCore.h"
#include "SoraException.h"

namespace sora {
    
    SoraFastRenderer::SoraFastRenderer():
    mAutoReleaseTime(DefaultAutoReleaseTime) {
    }
    
    SoraFastRenderer::~SoraFastRenderer() {
        SpriteCacheMap::iterator itSprite = mSpriteCache.begin();
        while(itSprite != mSpriteCache.end()) {
            if(itSprite->second.mSprite != NULL) {
                delete itSprite->second.mSprite;
            }
            ++itSprite;
        }
    }
    
    SoraFastRenderer* SoraFastRenderer::Instance() {
        static SoraFastRenderer renderer;
        return &renderer;
    }
    
    void SoraFastRenderer::renderSprite(const SoraWString& path, float32 x, float32 y) {
        SoraStringId sid = GetUniqueStringId(path);
        
        SpriteCacheMap::iterator itSprite = mSpriteCache.find(sid);
        if(itSprite != mSpriteCache.end()) {
            if(itSprite->second.mSprite != NULL) {
                // render and reset idle time
                itSprite->second.mSprite->render(x, y);
                itSprite->second.mIdleTime = 0.f;
            } else {
                SoraSprite* sprite = SoraCore::Instance()->createSprite(path);
                if(sprite) {
                    itSprite->second.mSprite = sprite;
                    itSprite->second.mIdleTime = 0.f;
                    
                    sprite->render(x, y);
                }
                    
            }
        } else {
            SoraSprite* sprite = SoraCore::Instance()->createSprite(path);
            if(sprite) {
                SpriteInfo info;
                info.mSprite = sprite;
                info.mIdleTime = 0.f;
                
                mSpriteCache.insert(std::make_pair(sid, info));
                sprite->render(x, y);
            }
        }
    }
    
    void SoraFastRenderer::setAutoReleaseTime(float32 time) {
        mAutoReleaseTime = time;
    }
    
    float32 SoraFastRenderer::getAutoReleaseTime() const {
        return mAutoReleaseTime;
    }
    
    void SoraFastRenderer::onUpdate(float32 dt) {
        SpriteCacheMap::iterator itSprite = mSpriteCache.begin();
        while(itSprite != mSpriteCache.end()) {
            if(itSprite->second.mSprite != NULL) {
                itSprite->second.mIdleTime += dt;
                if(itSprite->second.mIdleTime >= mAutoReleaseTime) {
                    delete itSprite->second.mSprite;
                    itSprite->second.mSprite = NULL;
                }
            }
            ++itSprite;
        }
    }
} // namespace sora
