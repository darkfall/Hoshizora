//
//  SoraFastRenderer.h
//  Sora
//
//  Created by Ruiwei Bu on 8/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraFastRenderer_h
#define Sora_SoraFastRenderer_h

#include "SoraPlatform.h"
#include "SoraSprite.h"
#include "SoraAutoUpdate.h"
#include "stringId.h"

#include <map>

namespace sora {
    
    const float32 DefaultAutoReleaseTime = 10.f;
    
    class SoraFastRenderer: public SoraAutoUpdate {
    protected:
        SoraFastRenderer();
        ~SoraFastRenderer();
        
    public:        
        static SoraFastRenderer* Instance();
     
        void renderSprite(const SoraWString& path, float32 x=0.f, float32 y=0.f);
        
        /**
         * Set the auto release of the cached sprites
         * If a sprite have not been used for a long time (larger than auto release time)
         * It would be purged from the memory
         **/
        void    setAutoReleaseTime(float32 time);
        float32 getAutoReleaseTime() const;
        
        void onUpdate(float32 dt);
        
        const SoraString getName() const {
            return "FastRenderer";
        }
        
    private:
        float32 mAutoReleaseTime;
        
        struct SpriteInfo {
            SpriteInfo():
            mSprite(NULL),
            mIdleTime(0.f) {
            }
            
            SoraSprite* mSprite;
            float32 mIdleTime;
        };
        typedef std::map<stringId, SpriteInfo> SpriteCacheMap;
        SpriteCacheMap mSpriteCache;
    };
    
} // namespace sora


#endif
