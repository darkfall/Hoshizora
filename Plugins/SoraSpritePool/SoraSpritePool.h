//
//  SoraSpritePool.h
//  Sora
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraSpritePool_h_plugin
#define Sora_SoraSpritePool_h_plugin

#include "SoraPlatform.h"
#include "SoraUncopyable.h"
#include "SoraString.h"

#include <map>

namespace sora {

    /**
     * Simple pool that allows manage sprites by tag
     * Useful for sprite reuse and spritesheet
     **/
    
    class SoraSprite;
    
    class SoraSpritePool: SoraUncopyable {
    public:
        void addSprite(const StringType& tag, const StringType& spritePath);
        void addSpriteFromMemory(const StringType& tag, uint32* data, ulong32 size);
        
        void removeSprite(const StringType& tag);
        
        SoraSprite* getSprite(const StringType& tag) const;
        
        void clear();
        
    private:
        typedef std::map<StringType, SoraSprite*> SpriteMap;
        SpriteMap mSprites;
    };

} // namespace sora

#endif
