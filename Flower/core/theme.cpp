//
//  theme.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "theme.h"
#include "SoraCore.h"
#include "config/SoraConfigParser.h"

namespace flower {
    
    Theme::~Theme() {
        SpriteMap::iterator it = mSprites.begin();
        SpriteMap::iterator end = mSprites.end();
        for(; it != end; ++it) {
            delete it->second;
        }
        
        mSprites.clear();
    }
    
    bool Theme::loadConfig(const std::string& path) {
        sora::SoraConfigParser parser;
        if(parser.open(path)) {
            if(parser.toNode("/theme")) {
                parser.toFirstChild();
                do {
                    std::string tag = parser.getString("name");
                    std::string path = parser.getString("path");
                    
                    if(!tag.empty() &&
                       !path.empty()) {
                        sora::SoraSprite* spr = sora::SoraCore::Instance()->createSprite(path);
                        if(spr)
                            mSprites.insert(std::make_pair(tag, spr)); 
                    }
                } while(parser.toNextChild());
                return true;
            }
        }
        return false;
    }
    
    sora::SoraSprite* Theme::get(const std::string& tag) {
        SpriteMap::const_iterator it = mSprites.find(tag);
        if(it != mSprites.end()) {
            return it->second;
        }
        return 0;
    }
    
    
} // namespace flower
