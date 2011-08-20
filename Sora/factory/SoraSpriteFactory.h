//
//  SoraSpriteFactory.h
//  Sora
//
//  Created by Ruiwei Bu on 8/19/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraSpriteFactory_h
#define Sora_SoraSpriteFactory_h

#include "SoraFactory.h"

#include "../json/json.h"
#include "../SoraSprite.h"

namespace sora {
    
    /**
     * Global Factories for Creating Sora Objects
     * Users can register their types
     * Some plugins such as SoraSpriteAnimation and SoraGifSprite 
     *  would register their type into the factory 
     **/
    
    /**
     * Global Sprite type factory
     * All sprite creation based on json in core goes there
     **/
    
    typedef SoraAbstractFactory<SoraSprite, SoraSprite*(const std::wstring&, Json::Value*)> SoraSpriteFactory;
        
#define GlobalSpriteFactory \
    SoraSpriteFactory::Instance()
    
#define REGISTER_SPRITE_PRODUCT(name, fn) \
    GlobalSpriteFactory->reg(name, fn);
    
    static SORA_API SoraSprite* CreateSpriteType(const std::string& type, const SoraWString& path, Json::Value* val=NULL) {
        return GlobalSpriteFactory->createInstance(type, path, val);
    }
    
    
    SORA_API void ParseJsonSpriteDef(SoraSprite* sprite, const Json::Value& val);
    
}

#endif
