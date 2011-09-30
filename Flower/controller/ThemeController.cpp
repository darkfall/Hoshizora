//
//  ThemeController.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "ThemeController.h"
#include "../core/theme.h"
#include "config/SoraConfigParser.h"

namespace flower {
    
    ThemeController::ThemeController():
    mCurrentTheme(0) {
        
    }
    
    ThemeController::~ThemeController() {
        if(mCurrentTheme)
            delete mCurrentTheme;
    }
    
    bool ThemeController::loadConfig(const std::wstring& path) {
        sora::SoraConfigParser parser;
        if(parser.open(path)) {
            if(parser.toNode("/themes")) {
                parser.toFirstChild();
                do {
                    
                    ThemeConfig config;
                    
                    std::string tag = parser.getString("name");
                    config.ThemeConfigPath = sora::s2ws(parser.getString("path"));
                    config.PreviewPath = sora::s2ws(parser.getString("preview"));
                    
                    if(!tag.empty() &&
                       !config.ThemeConfigPath.empty() &&
                       !config.PreviewPath.empty()) {
                        mThemes.insert(std::make_pair(tag, config));
                    }
                    
                } while(parser.toNextChild());
                
                return true;
            }
        }
        return false;
    }
    
    bool ThemeController::setCurrentTheme(const std::string& tag) {
        ThemeMap::const_iterator it = mThemes.find(tag);
        if(it != mThemes.end()) {
            Theme* theme = new Theme;
            if(!theme->loadConfig(it->second.ThemeConfigPath)) {
                return false;
            } else {
                
                if(mCurrentTheme) {
                    delete mCurrentTheme;
                }
            
                mCurrentTheme = theme;
                return true;
            }
        }
        return false;
    }
    
    sora::SoraSprite* ThemeController::getSprite(const std::string& tag) {
        sora_assert(mCurrentTheme);
        return mCurrentTheme->get(tag);
    }
    
    ThemeController::iterator ThemeController::begin() {
        return mThemes.begin();
    }
    
    ThemeController::iterator ThemeController::end() {
        return mThemes.end();
    }
    
    ThemeController::const_iterator ThemeController::begin() const {
        return mThemes.begin();
    }
    
    ThemeController::const_iterator ThemeController::end() const {
        return mThemes.end();
    }
     
} // namespace flower