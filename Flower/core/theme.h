//
//  theme.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_theme_h
#define Sora_iPhone_theme_h

#include "SoraPlatform.h"
#include "SoraSprite.h"


namespace flower {
    
    class Theme {
    public:
        ~Theme();
        
        bool loadConfig(const std::string& path);
        
        sora::SoraSprite* get(const std::string& tag);
        
    private:
        typedef std::map<std::string, sora::SoraSprite*> SpriteMap;
        SpriteMap mSprites;
    };
    
} // namespace flower


#endif
