//
//  ThemeController.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_ThemeController_h
#define Sora_iPhone_ThemeController_h

#include "SoraPlatform.h"
#include "SoraSprite.h"
#include "SoraSingleton.h"
#include <map>

namespace flower {
    
    class Theme;
    
    class ThemeController: public sora::SoraSingleton<ThemeController> {
    public:
        friend class sora::SoraSingleton<ThemeController>;
       
        ThemeController();
        ~ThemeController();
        
        bool loadConfig(const std::string& path);
        
        bool setCurrentTheme(const std::string& tag);
        
        sora::SoraSprite* getSprite(const std::string& tag);
        
        struct ThemeConfig {
            std::string ThemeConfigPath;
            std::string PreviewPath;
        };
        typedef std::map<std::string, ThemeConfig> ThemeMap;

        typedef ThemeMap::iterator iterator;
        typedef ThemeMap::const_iterator const_iterator;
        
        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        
    private:
        ThemeMap mThemes;
        Theme* mCurrentTheme;
    };
    
    
} // namespace flower


#endif
