//
//  SoraFastRenderer.h
//  Sora
//
//  Created by Robert Bu on 8/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraFastRenderer_h
#define Sora_SoraFastRenderer_h

#include "SoraPlatform.h"
#include "SoraSprite.h"
#include "SoraAutoUpdate.h"
#include "SoraStringId.h"

#include <map>

namespace sora {
    
    const float DefaultAutoReleaseTime = 10.f;
    
    class SoraFastRenderer: public SoraAutoUpdate {
    protected:
        SoraFastRenderer();
        ~SoraFastRenderer();
        
    public:        
        static SoraFastRenderer* Instance();
     
        void renderSprite(const SoraWString& path, float x=0.f, float y=0.f);
        
        /**
         * Set the auto release of the cached sprites
         * If a sprite have not been used for a long time (larger than auto release time)
         * It would be purged from the memory
         **/
        void    setAutoReleaseTime(float time);
        float getAutoReleaseTime() const;
        
        void onUpdate(float dt);
        
        const SoraString getName() const {
            return "FastRenderer";
        }
        
    private:
        float mAutoReleaseTime;
        
        struct SpriteInfo {
            SpriteInfo():
            mSprite(NULL),
            mIdleTime(0.f) {
            }
            
            SoraSprite* mSprite;
            float mIdleTime;
        };
        typedef hash_map<SoraStringId, SpriteInfo> SpriteCacheMap;
        SpriteCacheMap mSpriteCache;
    };
    
} // namespace sora


#endif
