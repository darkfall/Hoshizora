//
//  SpriteController.h
//  Sora
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SpriteController_h
#define Sora_SpriteController_h

#include "SoraSingleton.h"
#include "SoraSpritePool/SoraSpritePool.h"

namespace atd {
    
    class SpriteController: public sora::SoraSingleton<SpriteController> {
    public:
        friend class sora::SoraSingleton<SpriteController>;
        
        sora::SoraSprite* get(const sora::StringType& tag);
        
        bool load(const sora::StringType& path);
        
    private:
        sora::SoraSpritePool mSpritePool;
    };
    
    
} // namespace atd



#endif
