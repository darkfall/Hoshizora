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
        void delSprite(const SoraString& name);
        
        bool parse(const SoraWString& file, bool isFolder);
        void clear();
        
        int32 size() const;
        
        SoraSprite* operator[](const SoraString& name);
        
    private:
        inline bool parseFile(const SoraWString& path);
        
        typedef std::map<stringId, SoraSprite*> SPRITE_MAP;
        SPRITE_MAP mSprites;
    };
}

#endif
