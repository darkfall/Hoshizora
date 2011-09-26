//
//  SpriteController..cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SpriteController.h"
#include "config/SoraConfigParser.h"

namespace atd {
    
    bool SpriteController::load(const sora::StringType& path) {
        sora::SoraConfigParser parser;
        if(parser.open(path)) {
            parser.toNode("/sprites");
            parser.toFirstChild();
    
            do {
                std::string tag = parser.getString("tag");
                std::string path = parser.getString("path");
                
                if(!tag.empty() && !path.empty()) {
                    mSpritePool.addSprite(tag, path);
                }
            } while(parser.toNextChild());
        }
        return false;
    }
    
    sora::SoraSprite* SpriteController::get(const sora::StringType& tag) {
        return mSpritePool.getSprite(tag);
    }
    
}
