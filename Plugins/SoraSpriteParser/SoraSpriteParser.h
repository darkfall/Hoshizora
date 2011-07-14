//
//  SoraSpriteParser.h
//  Sora
//
//  Created by Ruiwei Bu on 7/4/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraSpriteParser_h
#define Sora_SoraSpriteParser_h

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "stringId.h"
#include "json/json.h"

#include "SoraSpriteAnimation/SoraSpriteAnimation.h"

#include <map>

namespace sora {
    
    class SoraSprite;
    
    class SoraSpriteParser: public SoraSingleton<SoraSpriteParser> {
    protected:
        friend class SoraSingleton<SoraSpriteParser>;
        
        SoraSpriteParser();
        ~SoraSpriteParser();
    
    public:
        SoraSprite* getSprite(const SoraString& name);
        SoraSpriteAnimation* getAnimation(const SoraString& name);
        void delSprite(const SoraString& name);
        
        bool parse(const SoraWString& file, bool isFolder);
        bool parse(const Json::Value& val);
        void clear();
        
        int32 size() const;
        
        SoraSprite* operator[](const SoraString& name);
        
    private:
        inline bool parseFile(const SoraWString& path);
        
        struct SpriteStore {
            SoraSprite* mSprite;
            bool mIsAnimation;
            
            SpriteStore(): 
            mSprite(NULL), 
            mIsAnimation(false) {}
            
            SpriteStore(SoraSprite* sprite, bool animation):
            mSprite(sprite),
            mIsAnimation(animation) {}
        };
        
        typedef std::map<stringId, SpriteStore> SPRITE_MAP;
        SPRITE_MAP mSprites;
    };
    
    static SoraSprite* GetSpriteFromParser(const SoraString& name) {
        return SoraSpriteParser::Instance()->getSprite(name);
    }
    
    static SoraSprite* GetAnimationFromParser(const SoraString& name) {
        return SoraSpriteParser::Instance()->getAnimation(name);
    }
}

#endif
